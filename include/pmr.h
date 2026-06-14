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
 */

#ifndef DAGPP_PMR_H
#define DAGPP_PMR_H

#include <cstddef>
#include <memory_resource>

namespace dagpp {

    template<typename Tp>
    class arena {
    public:
        explicit arena(const std::size_t initial_size = 0,
                       std::pmr::memory_resource* upstream = std::pmr::new_delete_resource())
            : m_resource(initial_size, upstream) {}
        arena(void* buffer, const std::size_t buffer_size,
              std::pmr::memory_resource* upstream = std::pmr::new_delete_resource())
            : m_resource(buffer, buffer_size, upstream) {}

        [[nodiscard]]
        std::pmr::polymorphic_allocator<Tp> allocator() noexcept { return &m_resource; }
        [[nodiscard]]
        std::pmr::memory_resource* resource() noexcept { return &m_resource; }

        void release() noexcept { m_resource.release(); }

    private:
        std::pmr::monotonic_buffer_resource m_resource;
    };

    template<std::size_t N, typename Tp>
    class stack_arena {
    public:
        explicit stack_arena(
            std::pmr::memory_resource* upstream = std::pmr::new_delete_resource())
            : m_buffer(), m_resource(m_buffer, N, upstream) {
        }

        [[nodiscard]]
        std::pmr::polymorphic_allocator<Tp> allocator() noexcept { return &m_resource; }
        [[nodiscard]]
        std::pmr::memory_resource* resource() noexcept { return &m_resource; }

        void release() noexcept { m_resource.release(); }

    private:
        alignas(std::max_align_t) std::byte m_buffer[N];
        std::pmr::monotonic_buffer_resource m_resource;
    };

    template<typename Tp>
    class pool {
    public:
        explicit pool(std::pmr::memory_resource* upstream = std::pmr::new_delete_resource())
            : m_resource(upstream) {}

        explicit pool(const std::pmr::pool_options& opts,
                      std::pmr::memory_resource* upstream = std::pmr::new_delete_resource())
            : m_resource(opts, upstream) {}

        [[nodiscard]]
        std::pmr::polymorphic_allocator<Tp> allocator() noexcept { return &m_resource; }
        [[nodiscard]]
        std::pmr::memory_resource* resource() noexcept { return &m_resource; }

        void release() noexcept { m_resource.release(); }

    private:
        std::pmr::unsynchronized_pool_resource m_resource;
    };

    class scoped_default_resource {
    public:
        explicit scoped_default_resource(std::pmr::memory_resource* r) noexcept
            : m_previous(std::pmr::set_default_resource(r)) {}

        ~scoped_default_resource() {
            std::pmr::set_default_resource(m_previous);
        }

        scoped_default_resource(const scoped_default_resource&) = delete;
        scoped_default_resource& operator=(const scoped_default_resource&) = delete;
        scoped_default_resource(scoped_default_resource&&) = delete;
        scoped_default_resource& operator=(scoped_default_resource&&) = delete;

    private:
        std::pmr::memory_resource* m_previous;
    };

} // namespace dagpp

#endif //DAGPP_PMR_H
