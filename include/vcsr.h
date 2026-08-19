/*
 * MIT License
 *
 * Copyright (c) 2026 Maksym Matskevich
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * ============================================================================
 * #                         ATTRIBUTION & REFERENCES                         #
 * ============================================================================
 * The core mathematical derivations for the implicit binary tree, segment
 * capacities, and in-place memory redistribution used in this Vertex-Centric
 * Compressed Sparse Row (VCSR) implementation were adapted from:
 * Islam, A. A. R., Dai, D., & Cheng, D. (2022). "VCSR: Mutable CSR Graph Format
 * Using Vertex-Centric Packed Memory Array." In 2022 22nd IEEE International
 * Symposium on Cluster, Cloud and Internet Computing (CCGrid) (pp. 71-80). IEEE.
 * DOI: 10.1109/CCGrid54584.2022.00016
 * Original Reference Implementation: https://github.com/DIR-LAB/VCSR
 * Research paper: https://webpages.charlotte.edu/ddai/data/dong-ccgrid-22.pdf
 * ============================================================================
 */

#ifndef DAGPP_VCSR_H
#define DAGPP_VCSR_H

#include <algorithm>
#include <bit>
#include <cmath>
#include <expected>
#include <span>
#include <vector>
#include <utility>
#include "detail.h"

namespace dagpp::vcsr {
    struct vertex_t {
        std::size_t degree;
        std::size_t start;
    };

    template<typename T>
    concept trivial_struct = std::is_standard_layout_v<T> && std::is_trivial_v<T>;

    template<typename T>
    concept density_policy = trivial_struct<T> && requires {
        { T::up_h } -> std::convertible_to<double>;
        { T::up_0 } -> std::convertible_to<double>;
        { T::low_h } -> std::convertible_to<double>;
        { T::low_0 } -> std::convertible_to<double>;
    };

    struct default_thresholds {
        // Parameters below are ones originally used by the researchers in DIR-LAB
        // Height-based (as opposed to depth-based) tree thresholds
        // Upper density thresholds
        static constexpr double up_h = 0.75; // root
        static constexpr double up_0 = 1.00; // leaves
        // Lower density thresholds
        static constexpr double low_h = 0.50; // root
        static constexpr double low_0 = 0.25; // leaves
    };

    template<detail::node TNode, typename TWeight = void, density_policy TDensityPolicy = default_thresholds>
    class pma_edge_list;

    template<detail::node TNode, density_policy TDensityPolicy>
    class pma_edge_list<TNode, void, TDensityPolicy> {
    public:
        using size_type = std::size_t;
        using difference_type = std::int64_t;
        using allocator_type = std::pmr::polymorphic_allocator<TNode>;
        using edge_input = std::pair<nodeid_t, nodeid_t>;

        explicit pma_edge_list(allocator_type alloc = {}) :
            m_vertices(alloc), m_edges(alloc), m_tree_actual(alloc), m_tree_total(alloc) {}

        std::pmr::vector<vertex_t> m_vertices;
        std::pmr::vector<nodeid_t> m_edges;

        size_type m_segment_size = 0;
        size_type m_segment_count = 0;
        size_type m_tree_height = 0;
        size_type m_num_edges = 0;
        size_type m_num_vertices = 0;

        double m_delta_up = 0.0;
        // May be used for rebalance after deletions.
        double m_delta_low = 0.0; // NOTE: unused currently.

        std::pmr::vector<size_type> m_tree_actual;
        std::pmr::vector<size_type> m_tree_total;

        [[nodiscard]]
        constexpr size_type get_segment_id(const size_type vertex_id) const {
            return vertex_id / m_segment_size + m_segment_count;
        }

        constexpr void update_segment_edge_total(const size_type vertex_id, const difference_type count) {
            size_type j = get_segment_id(vertex_id);
            while (j > 0) {
                m_tree_total[j] += count;
                j >>= 1;
            }
        }

        constexpr void recount_segment_total(const size_type start_vertex, const size_type end_vertex) {
            const size_type start_seg = get_segment_id(start_vertex) - m_segment_count;
            const size_type end_seg = get_segment_id(end_vertex) - m_segment_count;

            for (size_type i = start_seg; i < end_seg; ++i) {
                const size_type next_start =
                        i == m_segment_count - 1 ? m_edges.size() : m_vertices[(i + 1) * m_segment_size].start;
                const size_type segment_total_p = next_start - m_vertices[i * m_segment_size].start;
                size_type j = i + m_segment_count;
                const difference_type diff = static_cast<difference_type>(segment_total_p) - gsl::narrow_cast<difference_type>(m_tree_total[j]);
                while (j > 0) {
                    m_tree_total[j] += diff;
                    j >>= 1;
                }
            }
        }

        constexpr void recount_segment_total() {
            m_tree_total.assign(m_segment_count * 2, 0);
            for (size_type i = 0; i < m_segment_count; ++i) {
                const size_type next_start =
                        i == m_segment_count - 1 ? m_edges.size() : m_vertices[(i + 1) * m_segment_size].start;
                const size_type segment_total_p = next_start - m_vertices[i * m_segment_size].start;
                size_type j = i + m_segment_count;
                while (j > 0) {
                    m_tree_total[j] += segment_total_p;
                    j >>= 1;
                }
            }
        }

        constexpr void recount_segment_actual() {
            m_tree_actual.assign(m_segment_count * 2, 0);
            for (size_type seg = 0; seg < m_segment_count; ++seg) {
                size_type actual = 0;
                const size_type v_start = seg * m_segment_size;
                const size_type v_end = std::min(v_start + m_segment_size, m_vertices.size());
                for (size_type v = v_start; v < v_end; ++v) {
                    actual += m_vertices[v].degree;
                }
                size_type j = seg + m_segment_count;
                while (j > 0) {
                    m_tree_actual[j] += actual;
                    j >>= 1;
                }
            }
        }

        constexpr void resize() {
            const size_type new_capacity = std::max<size_type>(m_vertices.size(), m_edges.size() * 2);
            const size_type gaps = new_capacity - m_num_edges;
            const size_type nv = m_vertices.size();
            const double step = static_cast<double>(gaps) / static_cast<double>(m_num_edges + nv);

            double total_d = 0.0;
            for (size_type i = 0; i < nv; ++i) {
                const size_type deg = m_vertices[i].degree;
                total_d += deg + step * (deg + 1);
            }

            m_edges.resize(new_capacity);

            for (size_type i = nv; i-- > 0;) {
                const size_type deg = m_vertices[i].degree;
                total_d -= deg + step * (deg + 1);
                const auto new_start = static_cast<size_type>(total_d);
                if (deg > 0 && new_start != m_vertices[i].start) {
                    std::move_backward(m_edges.data() + m_vertices[i].start, m_edges.data() + m_vertices[i].start + deg,
                                       m_edges.data() + new_start + deg);
                }
                m_vertices[i].start = new_start;
            }

            recount_segment_total();
        }

        constexpr void rebalance_weighted(const size_type start_vertex, const size_type end_vertex,
                                          const size_type pma_idx) {
            const size_type from = m_vertices[start_vertex].start;
            const size_type to = end_vertex >= m_vertices.size() ? m_edges.size() : m_vertices[end_vertex].start;
            const size_type capacity = to - from;
            const size_type gaps = capacity - m_tree_actual[pma_idx];
            const size_type total_degree = m_tree_actual[pma_idx] + (end_vertex - start_vertex);
            const double step = static_cast<double>(gaps) / gsl::narrow_cast<double>(total_degree);

            size_type compact_pos = from;
            auto index_d = static_cast<double>(from);
            for (size_type i = start_vertex; i < end_vertex; ++i) {
                const size_type deg = m_vertices[i].degree;
                if (deg > 0 && m_vertices[i].start != compact_pos) {
                    for (size_type r = 0; r < deg; ++r) {
                        m_edges[compact_pos + r] = m_edges[m_vertices[i].start + r];
                    }
                }
                m_vertices[i].start = static_cast<size_type>(index_d);
                compact_pos += deg;
                index_d += deg + step * (deg + 1);
            }

            for (size_type i = end_vertex; i-- > start_vertex;) {
                const size_type deg = m_vertices[i].degree;
                compact_pos -= deg;
                if (const size_type new_start = m_vertices[i].start; deg > 0 && new_start != compact_pos) {
                    for (size_type r = deg; r-- > 0;) {
                        m_edges[new_start + r] = m_edges[compact_pos + r];
                    }
                }
            }

            recount_segment_total(start_vertex, end_vertex);
        }

        constexpr void rebalance_wrapper(const size_type src) {
            int height = 0;
            size_type window = get_segment_id(src);
            double density =
                    m_tree_total[window] == 0 ? 1.0 : static_cast<double>(m_tree_actual[window]) / m_tree_total[window];
            double up_height = TDensityPolicy::up_0 - height * m_delta_up;

            while (window > 0 && density >= up_height) {
                window /= 2;
                ++height;
                up_height = TDensityPolicy::up_0 - height * m_delta_up;
                if (window > 0) {
                    density = m_tree_total[window] == 0
                                      ? 1.0
                                      : static_cast<double>(m_tree_actual[window]) / m_tree_total[window];
                }
            }

            if (height == 0)
                return;

            if (window > 0 && density < up_height) {
                const size_type window_size = m_segment_size * (1 << height);
                const size_type left_index = src / window_size * window_size;
                const size_type right_index = std::min(left_index + window_size, m_vertices.size());
                rebalance_weighted(left_index, right_index, window);
            } else {
                resize();
            }
        }

        constexpr void insert(const nodeid_t src, const nodeid_t dst) {
            const size_type current_segment = get_segment_id(src);
            const size_type loc = m_vertices[src].start + m_vertices[src].degree;

            auto right_free_slot = static_cast<size_type>(-1);
            auto left_free_slot = static_cast<size_type>(-1);
            size_type left_vertex = src, right_vertex = 0;
            size_type left_vertex_boundary, right_vertex_boundary;

            if (m_tree_total[current_segment] > m_tree_actual[current_segment]) {
                left_vertex_boundary = src / m_segment_size * m_segment_size;
                right_vertex_boundary = std::min(left_vertex_boundary + m_segment_size, m_vertices.size() - 1);
            } else {
                size_type curr_seg_size = m_segment_size;
                size_type j = current_segment;
                while (j > 0) {
                    if (m_tree_total[j] > m_tree_actual[j])
                        break;
                    j >>= 1;
                    curr_seg_size <<= 1;
                }
                left_vertex_boundary = src / curr_seg_size * curr_seg_size;
                right_vertex_boundary = std::min(left_vertex_boundary + curr_seg_size, m_vertices.size() - 1);
            }

            for (size_type i = src; i < right_vertex_boundary; ++i) {
                if (m_vertices[i + 1].start > m_vertices[i].start + m_vertices[i].degree) {
                    right_free_slot = m_vertices[i].start + m_vertices[i].degree;
                    right_vertex = i;
                    break;
                }
            }

            if (right_free_slot == static_cast<size_type>(-1) && right_vertex_boundary == m_vertices.size() - 1) {
                if (const size_type last_v = m_vertices.size() - 1;
                    m_edges.size() > m_vertices[last_v].start + m_vertices[last_v].degree) {
                    right_free_slot = m_vertices[last_v].start + m_vertices[last_v].degree;
                    right_vertex = last_v;
                }
            }

            if (right_free_slot == static_cast<size_type>(-1)) {
                for (size_type i = src; i > left_vertex_boundary; --i) {
                    if (m_vertices[i].start > m_vertices[i - 1].start + m_vertices[i - 1].degree) {
                        left_free_slot = m_vertices[i].start - 1;
                        left_vertex = i;
                        break;
                    }
                }
            }

            if (right_free_slot != static_cast<size_type>(-1)) {
                if (right_free_slot >= loc) {
                    for (size_type i = right_free_slot; i > loc; --i) {
                        m_edges[i] = m_edges[i - 1];
                    }
                    for (size_type i = src + 1; i <= right_vertex; ++i) {
                        m_vertices[i].start += 1;
                    }
                    if (current_segment != get_segment_id(right_vertex)) {
                        update_segment_edge_total(src, 1);
                        update_segment_edge_total(right_vertex, -1);
                    }
                }
                m_edges[loc] = dst;
                m_vertices[src].degree += 1;
            } else if (left_free_slot != static_cast<size_type>(-1)) {
                if (left_free_slot < loc) {
                    for (size_type i = left_free_slot; i < loc - 1; ++i) {
                        m_edges[i] = m_edges[i + 1];
                    }
                    for (size_type i = left_vertex; i <= src; ++i) {
                        m_vertices[i].start -= 1;
                    }
                    if (current_segment != get_segment_id(left_vertex)) {
                        update_segment_edge_total(src, 1);
                        update_segment_edge_total(left_vertex, -1);
                    }
                }
                m_edges[loc - 1] = dst;
                m_vertices[src].degree += 1;
            } else {
                std::unreachable();
            }

            size_type j = current_segment;
            while (j > 0) {
                m_tree_actual[j] += 1;
                j >>= 1;
            }
            m_num_edges += 1;

            rebalance_wrapper(src);
        }

        constexpr void expand_vertices() {
            m_segment_count <<= 2;
            ++m_tree_height;
            m_delta_up = m_tree_height == 0 ? 0.0 : (TDensityPolicy::up_0 - TDensityPolicy::up_h) / m_tree_height;
            m_delta_low = m_tree_height == 0 ? 0.0 : (TDensityPolicy::low_h - TDensityPolicy::low_0) / m_tree_height;

            const size_type new_padded = m_segment_count * m_segment_size;
            m_vertices.resize(new_padded, {.degree = 0, .start = m_edges.size()});

            recount_segment_actual();
            resize(); // Force re-spreading the edges
        }

        constexpr void build(const std::pmr::vector<edge_input> &edges, const size_type num_v, const bool reverse) {
            m_num_edges = edges.size();
            m_num_vertices = num_v;

            m_segment_size = std::max<size_type>(1, num_v <= 1 ? 0 : std::bit_width(num_v - 1));
            m_segment_count = (num_v + m_segment_size - 1) / m_segment_size;
            m_segment_count = std::max<size_type>(1, std::bit_floor(m_segment_count));
            m_segment_size = std::max<size_type>(1, (num_v + m_segment_count - 1) / m_segment_count);
            const size_type padded_vertices = m_segment_count * m_segment_size;

            m_tree_height = std::bit_width(m_segment_count) - 1;
            m_delta_up = m_tree_height == 0 ? 0.0 : (TDensityPolicy::up_0 - TDensityPolicy::up_h) / m_tree_height;
            m_delta_low = m_tree_height == 0 ? 0.0 : (TDensityPolicy::low_h - TDensityPolicy::low_0) / m_tree_height;

            const size_type elem_capacity = std::max<size_type>(padded_vertices, m_num_edges / TDensityPolicy::low_0);

            m_vertices.resize(padded_vertices, {.degree = 0, .start = 0});
            m_edges.resize(elem_capacity);
            m_tree_actual.assign(m_segment_count * 2, 0);
            m_tree_total.assign(m_segment_count * 2, 0);

            for (size_type i = 0; i < m_num_edges; ++i) {
                const auto [from, to] = edges[i];
                const auto [src, dst] = reverse ? std::pair{to, from} : std::pair{from, to};

                m_edges[i] = dst;
                if (m_vertices[src].degree == 0)
                    m_vertices[src].start = i;
                ++m_vertices[src].degree;

                const size_type leaf = src / m_segment_size + m_segment_count;
                for (size_type j = leaf; j > 0; j >>= 1)
                    ++m_tree_actual[j];
            }

            for (size_type i = 1; i < padded_vertices; ++i) {
                if (m_vertices[i].degree == 0) {
                    m_vertices[i].start = m_vertices[i - 1].start + m_vertices[i - 1].degree;
                }
            }

            const size_type gaps = elem_capacity - m_num_edges;
            const auto step = static_cast<double>(gaps) / (m_num_edges + padded_vertices);

            double total_d = 0.0;
            for (size_type i = 0; i < padded_vertices; ++i) {
                const size_type deg = m_vertices[i].degree;
                total_d += deg + step * (deg + 1);
            }

            for (size_type i = padded_vertices; i-- > 0;) {
                const size_type deg = m_vertices[i].degree;
                total_d -= deg + step * (deg + 1);
                const auto new_start = static_cast<size_type>(total_d);
                if (deg > 0 && new_start != m_vertices[i].start) {
                    std::move_backward(m_edges.data() + m_vertices[i].start, m_edges.data() + m_vertices[i].start + deg,
                                       m_edges.data() + new_start + deg);
                }
                m_vertices[i].start = new_start;
            }

            recount_segment_total();
        }
    };

    template<detail::node TNode, typename TWeight, density_policy TDensityPolicy>
    class pma_edge_list {
    public:
        using size_type = std::size_t;
        using difference_type = std::int64_t;
        using weight_type = TWeight;
        using allocator_type = std::pmr::polymorphic_allocator<TNode>;
        using density_thresholds = TDensityPolicy;

        struct edge_input {
            nodeid_t from, to;
            TWeight weight;
        };

        explicit pma_edge_list(allocator_type alloc = {}) :
            m_vertices(alloc), m_edges(alloc), m_weights(alloc), m_tree_actual(alloc), m_tree_total(alloc) {}

        std::pmr::vector<vertex_t> m_vertices;
        std::pmr::vector<nodeid_t> m_edges;
        std::pmr::vector<TWeight> m_weights;

        size_type m_segment_size = 0;
        size_type m_segment_count = 0;
        size_type m_tree_height = 0;
        size_type m_num_edges = 0;
        size_type m_num_vertices = 0;

        double m_delta_up = 0.0;
        // May be used for rebalance after deletions.
        double m_delta_low = 0.0; // NOTE: unused currently.

        std::pmr::vector<size_type> m_tree_actual;
        std::pmr::vector<size_type> m_tree_total;

        [[nodiscard]]
        constexpr size_type get_segment_id(const size_type vertex_id) const {
            return vertex_id / m_segment_size + m_segment_count;
        }

        constexpr void update_segment_edge_total(const size_type vertex_id, const difference_type count) {
            size_type j = get_segment_id(vertex_id);
            while (j > 0) {
                m_tree_total[j] += count;
                j >>= 1;
            }
        }


        constexpr void recount_segment_total(const size_type start_vertex, const size_type end_vertex) {
            const size_type start_seg = get_segment_id(start_vertex) - m_segment_count;
            const size_type end_seg = get_segment_id(end_vertex) - m_segment_count;

            for (size_type i = start_seg; i < end_seg; ++i) {
                const size_type next_start =
                        i == m_segment_count - 1 ? m_edges.size() : m_vertices[(i + 1) * m_segment_size].start;
                const size_type segment_total_p = next_start - m_vertices[i * m_segment_size].start;
                size_type j = i + m_segment_count;
                const difference_type diff = static_cast<difference_type>(segment_total_p) - static_cast<difference_type>(m_tree_total[j]);
                while (j > 0) {
                    m_tree_total[j] += diff;
                    j >>= 1;
                }
            }
        }

        constexpr void recount_segment_total() {
            m_tree_total.assign(m_segment_count * 2, 0);
            for (size_type i = 0; i < m_segment_count; ++i) {
                const size_type next_start =
                        i == m_segment_count - 1 ? m_edges.size() : m_vertices[(i + 1) * m_segment_size].start;
                const size_type segment_total_p = next_start - m_vertices[i * m_segment_size].start;
                size_type j = i + m_segment_count;
                while (j > 0) {
                    m_tree_total[j] += segment_total_p;
                    j >>= 1;
                }
            }
        }

        constexpr void recount_segment_actual() {
            m_tree_actual.assign(m_segment_count * 2, 0);
            for (size_type seg = 0; seg < m_segment_count; ++seg) {
                size_type actual = 0;
                const size_type v_start = seg * m_segment_size;
                const size_type v_end = std::min(v_start + m_segment_size, m_vertices.size());
                for (size_type v = v_start; v < v_end; ++v) {
                    actual += m_vertices[v].degree;
                }
                size_type j = seg + m_segment_count;
                while (j > 0) {
                    m_tree_actual[j] += actual;
                    j >>= 1;
                }
            }
        }

        constexpr void resize() {
            const size_type new_capacity = std::max<size_type>(m_vertices.size(), m_edges.size() * 2);
            const size_type gaps = new_capacity - m_num_edges;
            const size_type nv = m_vertices.size();
            const double step = static_cast<double>(gaps) / static_cast<double>(m_num_edges + nv);

            double total_d = 0.0;
            for (size_type i = 0; i < nv; ++i) {
                const size_type deg = m_vertices[i].degree;
                total_d += deg + step * (deg + 1);
            }

            m_edges.resize(new_capacity);
            m_weights.resize(new_capacity);

            for (size_type i = nv; i-- > 0;) {
                const size_type deg = m_vertices[i].degree;
                total_d -= deg + step * (deg + 1);
                const auto new_start = static_cast<size_type>(total_d);
                if (deg > 0 && new_start != m_vertices[i].start) {
                    std::move_backward(m_edges.data() + m_vertices[i].start, m_edges.data() + m_vertices[i].start + deg,
                                       m_edges.data() + new_start + deg);
                    std::move_backward(m_weights.data() + m_vertices[i].start,
                                       m_weights.data() + m_vertices[i].start + deg,
                                       m_weights.data() + new_start + deg);
                }
                m_vertices[i].start = new_start;
            }

            recount_segment_total();
        }

        constexpr void rebalance_weighted(const size_type start_vertex, const size_type end_vertex,
                                          const size_type pma_idx) {
            const size_type from = m_vertices[start_vertex].start;
            const size_type to = end_vertex >= m_vertices.size() ? m_edges.size() : m_vertices[end_vertex].start;
            const size_type capacity = to - from;
            const size_type gaps = capacity - m_tree_actual[pma_idx];
            const size_type total_degree = m_tree_actual[pma_idx] + (end_vertex - start_vertex);
            const double step = static_cast<double>(gaps) / gsl::narrow_cast<double>(total_degree);

            size_type compact_pos = from;
            auto index_d = static_cast<double>(from);
            for (size_type i = start_vertex; i < end_vertex; ++i) {
                const size_type deg = m_vertices[i].degree;
                if (deg > 0 && m_vertices[i].start != compact_pos) {
                    for (size_type r = 0; r < deg; ++r) {
                        m_edges[compact_pos + r] = m_edges[m_vertices[i].start + r];
                        m_weights[compact_pos + r] = m_weights[m_vertices[i].start + r];
                    }
                }
                m_vertices[i].start = static_cast<size_type>(index_d);
                compact_pos += deg;
                index_d += deg + step * (deg + 1);
            }

            for (size_type i = end_vertex; i-- > start_vertex;) {
                const size_type deg = m_vertices[i].degree;
                compact_pos -= deg;
                if (const size_type new_start = m_vertices[i].start; deg > 0 && new_start != compact_pos) {
                    for (size_type r = deg; r-- > 0;) {
                        m_edges[new_start + r] = m_edges[compact_pos + r];
                        m_weights[new_start + r] = m_weights[compact_pos + r];
                    }
                }
            }

            recount_segment_total(start_vertex, end_vertex);
        }

        constexpr void rebalance_wrapper(const size_type src) {
            int height = 0;
            size_type window = get_segment_id(src);
            double density =
                    m_tree_total[window] == 0 ? 1.0 : static_cast<double>(m_tree_actual[window]) / m_tree_total[window];
            double up_height = TDensityPolicy::up_0 - height * m_delta_up;

            while (window > 0 && density >= up_height) {
                window /= 2;
                ++height;
                up_height = TDensityPolicy::up_0 - height * m_delta_up;
                if (window > 0) {
                    density = m_tree_total[window] == 0
                                      ? 1.0
                                      : static_cast<double>(m_tree_actual[window]) / m_tree_total[window];
                }
            }

            if (height == 0)
                return;

            if (window > 0 && density < up_height) {
                const size_type window_size = m_segment_size * (1 << height);
                const size_type left_index = src / window_size * window_size;
                const size_type right_index = std::min(left_index + window_size, m_vertices.size());
                rebalance_weighted(left_index, right_index, window);
            } else {
                resize();
            }
        }

        constexpr void insert(const nodeid_t src, const nodeid_t dst, const TWeight weight) {
            const size_type current_segment = get_segment_id(src);
            const size_type loc = m_vertices[src].start + m_vertices[src].degree;

            auto right_free_slot = static_cast<size_type>(-1);
            auto left_free_slot = static_cast<size_type>(-1);
            size_type left_vertex = src, right_vertex = 0;
            size_type left_vertex_boundary, right_vertex_boundary;

            if (m_tree_total[current_segment] > m_tree_actual[current_segment]) {
                left_vertex_boundary = src / m_segment_size * m_segment_size;
                right_vertex_boundary = std::min(left_vertex_boundary + m_segment_size, m_vertices.size() - 1);
            } else {
                size_type curr_seg_size = m_segment_size;
                size_type j = current_segment;
                while (j > 0) {
                    if (m_tree_total[j] > m_tree_actual[j])
                        break;
                    j >>= 1;
                    curr_seg_size <<= 1;
                }
                left_vertex_boundary = src / curr_seg_size * curr_seg_size;
                right_vertex_boundary = std::min(left_vertex_boundary + curr_seg_size, m_vertices.size() - 1);
            }

            for (size_type i = src; i < right_vertex_boundary; ++i) {
                if (m_vertices[i + 1].start > m_vertices[i].start + m_vertices[i].degree) {
                    right_free_slot = m_vertices[i].start + m_vertices[i].degree;
                    right_vertex = i;
                    break;
                }
            }

            if (right_free_slot == static_cast<size_type>(-1) && right_vertex_boundary == m_vertices.size() - 1) {
                if (const size_type last_v = m_vertices.size() - 1;
                    m_edges.size() > m_vertices[last_v].start + m_vertices[last_v].degree) {
                    right_free_slot = m_vertices[last_v].start + m_vertices[last_v].degree;
                    right_vertex = last_v;
                }
            }

            if (right_free_slot == static_cast<size_type>(-1)) {
                for (size_type i = src; i > left_vertex_boundary; --i) {
                    if (m_vertices[i].start > m_vertices[i - 1].start + m_vertices[i - 1].degree) {
                        left_free_slot = m_vertices[i].start - 1;
                        left_vertex = i;
                        break;
                    }
                }
            }

            if (right_free_slot != static_cast<size_type>(-1)) {
                if (right_free_slot >= loc) {
                    for (size_type i = right_free_slot; i > loc; --i) {
                        m_edges[i] = m_edges[i - 1];
                        m_weights[i] = m_weights[i - 1];
                    }
                    for (size_type i = src + 1; i <= right_vertex; ++i) {
                        m_vertices[i].start += 1;
                    }
                    if (current_segment != get_segment_id(right_vertex)) {
                        update_segment_edge_total(src, 1);
                        update_segment_edge_total(right_vertex, -1);
                    }
                }
                m_edges[loc] = dst;
                m_weights[loc] = weight;
                m_vertices[src].degree += 1;
            } else if (left_free_slot != static_cast<size_type>(-1)) {
                if (left_free_slot < loc) {
                    for (size_type i = left_free_slot; i < loc - 1; ++i) {
                        m_edges[i] = m_edges[i + 1];
                        m_weights[i] = m_weights[i + 1];
                    }
                    for (size_type i = left_vertex; i <= src; ++i) {
                        m_vertices[i].start -= 1;
                    }
                    if (current_segment != get_segment_id(left_vertex)) {
                        update_segment_edge_total(src, 1);
                        update_segment_edge_total(left_vertex, -1);
                    }
                }
                m_edges[loc - 1] = dst;
                m_weights[loc - 1] = weight;
                m_vertices[src].degree += 1;
            } else {
                std::unreachable();
            }

            size_type j = current_segment;
            while (j > 0) {
                m_tree_actual[j] += 1;
                j >>= 1;
            }
            m_num_edges += 1;

            rebalance_wrapper(src);
        }

        constexpr void expand_vertices() {
            m_segment_count *= 2;
            ++m_tree_height;
            m_delta_up = (TDensityPolicy::up_0 - TDensityPolicy::up_h) / m_tree_height;
            m_delta_low = (TDensityPolicy::low_h - TDensityPolicy::low_0) / m_tree_height;

            const size_type new_padded = m_segment_count * m_segment_size;
            m_vertices.resize(new_padded, {.degree = 0, .start = m_edges.size()});

            recount_segment_actual();
            resize();
        }

        constexpr void build(const std::pmr::vector<edge_input> &edges, const size_type num_v, const bool reverse) {
            m_num_edges = edges.size();
            m_num_vertices = num_v;

            m_segment_size = std::max<size_type>(1, num_v <= 1 ? 0 : std::bit_width(num_v - 1));
            m_segment_count = (num_v + m_segment_size - 1) / m_segment_size;
            m_segment_count = std::max<size_type>(1, std::bit_floor(m_segment_count));
            m_segment_size = std::max<size_type>(1, (num_v + m_segment_count - 1) / m_segment_count);
            const size_type padded_vertices = m_segment_count * m_segment_size;

            m_tree_height = std::bit_width(m_segment_count) - 1;
            m_delta_up = m_tree_height == 0 ? 0.0 : (TDensityPolicy::up_0 - TDensityPolicy::up_h) / m_tree_height;
            m_delta_low = m_tree_height == 0 ? 0.0 : (TDensityPolicy::low_h - TDensityPolicy::low_0) / m_tree_height;

            const size_type elem_capacity = std::max<size_type>(padded_vertices, m_num_edges / TDensityPolicy::low_0);

            m_vertices.resize(padded_vertices, {.degree = 0, .start = 0});
            m_edges.resize(elem_capacity);
            m_weights.resize(elem_capacity);
            m_tree_actual.assign(m_segment_count * 2, 0);
            m_tree_total.assign(m_segment_count * 2, 0);

            for (size_type i = 0; i < m_num_edges; ++i) {
                const auto &e = edges[i];
                const nodeid_t src = reverse ? e.to : e.from;
                const nodeid_t dst = reverse ? e.from : e.to;

                m_edges[i] = dst;
                m_weights[i] = e.weight;
                if (m_vertices[src].degree == 0)
                    m_vertices[src].start = i;
                ++m_vertices[src].degree;

                const size_type leaf = src / m_segment_size + m_segment_count;
                for (size_type j = leaf; j > 0; j >>= 1)
                    ++m_tree_actual[j];
            }

            for (size_type i = 1; i < padded_vertices; ++i) {
                if (m_vertices[i].degree == 0) {
                    m_vertices[i].start = m_vertices[i - 1].start + m_vertices[i - 1].degree;
                }
            }

            const size_type gaps = elem_capacity - m_num_edges;
            const double step = static_cast<double>(gaps) / (m_num_edges + padded_vertices);

            double total_d = 0.0;
            for (size_type i = 0; i < padded_vertices; ++i) {
                const size_type deg = m_vertices[i].degree;
                total_d += deg + step * (deg + 1);
            }

            for (size_type i = padded_vertices; i-- > 0;) {
                const size_type deg = m_vertices[i].degree;
                total_d -= deg + step * (deg + 1);
                const auto new_start = static_cast<size_type>(total_d);
                if (deg > 0 && new_start != m_vertices[i].start) {
                    std::move_backward(m_edges.data() + m_vertices[i].start, m_edges.data() + m_vertices[i].start + deg,
                                       m_edges.data() + new_start + deg);
                    std::move_backward(m_weights.data() + m_vertices[i].start,
                                       m_weights.data() + m_vertices[i].start + deg,
                                       m_weights.data() + new_start + deg);
                }
                m_vertices[i].start = new_start;
            }

            recount_segment_total();
        }
    };

    template<detail::node TNode>
    class digraph_builder;
    template<detail::node TNode, density_policy TDensityPolicy = default_thresholds, typename... TExtension>
    class digraph;

    template<detail::node TNode>
    class digraph_builder {
    public:
        using degree_type = std::size_t;
        using allocator_type = std::pmr::polymorphic_allocator<TNode>;
        using edge_input = pma_edge_list<TNode>::edge_input;

        constexpr explicit digraph_builder(allocator_type alloc = {}) : m_nodes(alloc), m_edges(alloc) {}

        constexpr void add_edge(nodeid_t from, nodeid_t to) { m_edges.emplace_back(from, to); }

        constexpr nodeid_t add_node(const TNode &node) {
            m_nodes.push_back(node);
            return m_nodes.size() - 1;
        }
        constexpr nodeid_t add_node(TNode &&node) {
            m_nodes.push_back(std::forward<TNode>(node));
            return m_nodes.size() - 1;
        }
        template<typename... Args>
        constexpr nodeid_t emplace_node(Args &&...args) {
            m_nodes.emplace_back(std::forward<Args>(args)...);
            return m_nodes.size() - 1;
        }
        constexpr void reserve_nodes(const std::size_t n) { m_nodes.reserve(n); }
        constexpr void reserve_edges(const std::size_t n) { m_edges.reserve(n); }

        template<density_policy TDensityPolicy = default_thresholds, class... TExtension>
        [[nodiscard]]
        digraph<TNode, TDensityPolicy, TExtension...> compile() {
            digraph<TNode, TDensityPolicy, TExtension...> graph(m_nodes.get_allocator());
            graph.m_payloads = std::move(m_nodes);

            graph.m_forward.build(m_edges, graph.m_payloads.size(), false);
            graph.m_reverse.build(m_edges, graph.m_payloads.size(), true);

            return graph;
        }

    private:
        std::pmr::vector<TNode> m_nodes;
        std::pmr::vector<edge_input> m_edges;
    };

    template<detail::node TNode, density_policy TDensityPolicy, typename... TExtension>
    class digraph : public TExtension... {
    public:
        using node_type = TNode;
        using size_type = std::size_t;
        using allocator_type = std::pmr::polymorphic_allocator<TNode>;

        constexpr explicit digraph(allocator_type alloc = {}) : m_payloads(alloc), m_forward(alloc), m_reverse(alloc) {}

        [[nodiscard]]
        constexpr allocator_type get_allocator() const {
            return m_payloads.get_allocator();
        }

        constexpr nodeid_t add_node(const TNode &node) {
            if (m_payloads.size() >= m_forward.m_vertices.size()) [[unlikely]] {
                m_forward.expand_vertices();
                m_reverse.expand_vertices();
            }
            m_payloads.push_back(node);
            return m_payloads.size() - 1;
        }

        constexpr nodeid_t add_node(TNode &&node) {
            if (m_payloads.size() >= m_forward.m_vertices.size()) [[unlikely]] {
                m_forward.expand_vertices();
                m_reverse.expand_vertices();
            }
            m_payloads.push_back(std::move(node));
            return m_payloads.size() - 1;
        }

        template<typename... Args>
        constexpr nodeid_t emplace_node(Args &&...args) {
            if (m_payloads.size() >= m_forward.m_vertices.size()) [[unlikely]] {
                m_forward.expand_vertices();
                m_reverse.expand_vertices();
            }
            m_payloads.emplace_back(std::forward<Args>(args)...);
            return m_payloads.size() - 1;
        }

        constexpr void add_edge(nodeid_t from, nodeid_t to) {
            m_forward.insert(from, to);
            m_reverse.insert(to, from);
        }

        [[nodiscard]]
        constexpr const node_type &node(nodeid_t id) const {
            return m_payloads[id];
        }

        [[nodiscard]]
        constexpr size_type node_count() const {
            return m_payloads.size();
        }

        [[nodiscard]]
        constexpr size_type edge_count() const {
            return m_forward.m_num_edges;
        }

        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> out_edges(nodeid_t id) const {
            if (id >= m_forward.m_vertices.size()) [[unlikely]] {
                return std::unexpected("Index is out of range.");
            }
            const auto &v = m_forward.m_vertices[id];
            return std::span<const nodeid_t>(m_forward.m_edges.data() + v.start, v.degree);
        }

        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> in_edges(nodeid_t id) const {
            if (id >= m_reverse.m_vertices.size()) [[unlikely]] {
                return std::unexpected("Index is out of range.");
            }
            const auto &v = m_reverse.m_vertices[id];
            return std::span<const nodeid_t>(m_reverse.m_edges.data() + v.start, v.degree);
        }

        [[nodiscard]]
        constexpr bool is_acyclic() const {
            const auto n = node_count();
            if (n == 0)
                return true;

            auto alloc = m_payloads.get_allocator();
            std::pmr::vector<size_type> in_degree(n, 0, alloc);
            std::pmr::vector<nodeid_t> queue(alloc);
            queue.reserve(n);

            for (nodeid_t u = 0; u < n; ++u) {
                in_degree[u] = m_reverse.m_vertices[u].degree;
                if (in_degree[u] == 0) {
                    queue.emplace_back(u);
                }
            }

            size_type head = 0;
            while (head < queue.size()) {
                const auto u = queue[head++];
                if (auto edges = out_edges(u)) {
                    for (const auto v: *edges) {
                        if (--in_degree[v] == 0) {
                            queue.emplace_back(v);
                        }
                    }
                }
            }

            return queue.size() == n;
        }

    protected:
        friend class digraph_builder<node_type>;
        std::pmr::vector<node_type> m_payloads;
        pma_edge_list<node_type, void, TDensityPolicy> m_forward;
        pma_edge_list<node_type, void, TDensityPolicy> m_reverse;
    };

    template<detail::node TNode, number TWeight>
    class wdigraph_builder;
    template<detail::node TNode, number TWeight, density_policy TDensityPolicy = default_thresholds,
             typename... TExtension>
    class wdigraph;

    template<detail::node TNode, number TWeight>
    class wdigraph_builder {
    public:
        using degree_type = std::size_t;
        using weight_type = TWeight;
        using allocator_type = std::pmr::polymorphic_allocator<TNode>;
        using edge_input = pma_edge_list<TNode, TWeight>::edge_input;

        constexpr explicit wdigraph_builder(allocator_type alloc = {}) : m_nodes(alloc), m_edges(alloc) {}

        constexpr void add_edge(nodeid_t from, nodeid_t to, TWeight weight = TWeight{1}) {
            m_edges.push_back({from, to, weight});
        }

        constexpr nodeid_t add_node(const TNode &node) {
            m_nodes.push_back(node);
            return m_nodes.size() - 1;
        }
        constexpr nodeid_t add_node(TNode &&node) {
            m_nodes.push_back(std::forward<TNode>(node));
            return m_nodes.size() - 1;
        }
        template<typename... Args>
        constexpr nodeid_t emplace_node(Args &&...args) {
            m_nodes.emplace_back(std::forward<Args>(args)...);
            return m_nodes.size() - 1;
        }
        constexpr void reserve_nodes(std::size_t n) { m_nodes.reserve(n); }
        constexpr void reserve_edges(std::size_t n) { m_edges.reserve(n); }

        template<density_policy TDensityPolicy = default_thresholds, class... TExtension>
        [[nodiscard]]
        wdigraph<TNode, TWeight, TDensityPolicy, TExtension...> compile() {
            wdigraph<TNode, TWeight, TDensityPolicy, TExtension...> graph(m_nodes.get_allocator());
            graph.m_payloads = std::move(m_nodes);

            graph.m_forward.build(m_edges, graph.m_payloads.size(), false);
            graph.m_reverse.build(m_edges, graph.m_payloads.size(), true);

            return graph;
        }

    private:
        std::pmr::vector<TNode> m_nodes;
        std::pmr::vector<edge_input> m_edges;
    };

    template<detail::node TNode, number TWeight, density_policy TDensityPolicy, typename... TExtension>
    class wdigraph : public TExtension... {
    public:
        using node_type = TNode;
        using size_type = std::size_t;
        using weight_type = TWeight;
        using allocator_type = std::pmr::polymorphic_allocator<TNode>;

        constexpr explicit wdigraph(allocator_type alloc = {}) :
            m_payloads(alloc), m_forward(alloc), m_reverse(alloc) {}

        [[nodiscard]]
        constexpr allocator_type get_allocator() const {
            return m_payloads.get_allocator();
        }

        constexpr nodeid_t add_node(const TNode &node) {
            if (m_payloads.size() >= m_forward.m_vertices.size()) [[unlikely]] {
                m_forward.expand_vertices();
                m_reverse.expand_vertices();
            }
            m_payloads.push_back(node);
            return m_payloads.size() - 1;
        }
        constexpr nodeid_t add_node(TNode &&node) {
            if (m_payloads.size() >= m_forward.m_vertices.size()) [[unlikely]] {
                m_forward.expand_vertices();
                m_reverse.expand_vertices();
            }
            m_payloads.push_back(std::move(node));
            return m_payloads.size() - 1;
        }
        template<typename... Args>
        constexpr nodeid_t emplace_node(Args &&...args) {
            if (m_payloads.size() >= m_forward.m_vertices.size()) [[unlikely]] {
                m_forward.expand_vertices();
                m_reverse.expand_vertices();
            }
            m_payloads.emplace_back(std::forward<Args>(args)...);
            return m_payloads.size() - 1;
        }

        constexpr void add_edge(nodeid_t from, nodeid_t to, TWeight weight = TWeight{1}) {
            m_forward.insert(from, to, weight);
            m_reverse.insert(to, from, weight);
        }

        [[nodiscard]]
        constexpr const node_type &node(nodeid_t id) const {
            return m_payloads[id];
        }

        [[nodiscard]]
        constexpr size_type node_count() const {
            return m_payloads.size();
        }

        [[nodiscard]]
        constexpr size_type edge_count() const {
            return m_forward.m_num_edges;
        }

        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> out_edges(nodeid_t id) const {
            if (id >= m_forward.m_vertices.size()) [[unlikely]]
                return std::unexpected("Index is out of range.");
            const auto &v = m_forward.m_vertices[id];
            return std::span<const nodeid_t>(m_forward.m_edges.data() + v.start, v.degree);
        }

        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> in_edges(nodeid_t id) const {
            if (id >= m_reverse.m_vertices.size()) [[unlikely]]
                return std::unexpected("Index is out of range.");
            const auto &v = m_reverse.m_vertices[id];
            return std::span<const nodeid_t>(m_reverse.m_edges.data() + v.start, v.degree);
        }

        [[nodiscard]]
        constexpr std::expected<std::span<const weight_type>, std::string> out_weights(nodeid_t id) const {
            if (id >= m_forward.m_vertices.size()) [[unlikely]]
                return std::unexpected("Index is out of range.");
            const auto &v = m_forward.m_vertices[id];
            return std::span<const weight_type>(m_forward.m_weights.data() + v.start, v.degree);
        }

        [[nodiscard]]
        constexpr std::expected<std::span<const weight_type>, std::string> in_weights(nodeid_t id) const {
            if (id >= m_reverse.m_vertices.size()) [[unlikely]]
                return std::unexpected("Index is out of range.");
            const auto &v = m_reverse.m_vertices[id];
            return std::span<const weight_type>(m_reverse.m_weights.data() + v.start, v.degree);
        }

        [[nodiscard]]
        constexpr bool is_acyclic() const {
            const auto n = node_count();
            if (n == 0)
                return true;

            auto alloc = m_payloads.get_allocator();
            std::pmr::vector<size_type> in_degree(n, 0, alloc);
            std::pmr::vector<nodeid_t> queue(alloc);
            queue.reserve(n);

            for (nodeid_t u = 0; u < n; ++u) {
                in_degree[u] = m_reverse.m_vertices[u].degree;
                if (in_degree[u] == 0)
                    queue.emplace_back(u);
            }

            size_type head = 0;
            while (head < queue.size()) {
                const auto u = queue[head++];
                if (auto edges = out_edges(u)) {
                    for (const auto v: *edges) {
                        if (--in_degree[v] == 0)
                            queue.emplace_back(v);
                    }
                }
            }
            return queue.size() == n;
        }

    protected:
        friend class wdigraph_builder<node_type, weight_type>;
        std::pmr::vector<node_type> m_payloads;
        pma_edge_list<node_type, weight_type, TDensityPolicy> m_forward;
        pma_edge_list<node_type, weight_type, TDensityPolicy> m_reverse;
    };

    struct _static_test_node {
        int id;
    };
    static_assert(directed_graph<digraph<_static_test_node>>, "VCSR graph does not satisfy `directed_graph` concept");
    static_assert(wdirected_graph<wdigraph<_static_test_node, int>>,
                  "VCSR weighted graph does not satisfy `wdirected_graph` concept");
} // namespace dagpp::vcsr

#endif // DAGPP_VCSR_H
