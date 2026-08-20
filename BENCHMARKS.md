> NOTE: Hardware context: Ryzen 5 PRO 7540U, 12x 3.16 GHz CPUs, 16 MB L3 cache. Built with -O3, -march=native and Google Benchmark.

#### BM_ConstructChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 8.28      | 8.26     | us        | 7.75M   |
| 64    | Median                   | 8.27      | 8.25     | us        | 7.76M   |
| 64    | Standard Deviation       | 0.04      | 0.04     | us        | 39.91k  |
| 64    | Coefficient of Variation | 0.52%     | 0.52%    |           | 0.52%   |
| 256   | Mean                     | 33.12     | 33.04    | us        | 7.75M   |
| 256   | Median                   | 33.13     | 33.04    | us        | 7.75M   |
| 256   | Standard Deviation       | 0.08      | 0.08     | us        | 18.39k  |
| 256   | Coefficient of Variation | 0.24%     | 0.24%    |           | 0.24%   |
| 1024  | Mean                     | 128.34    | 128.02   | us        | 8M      |
| 1024  | Median                   | 128.35    | 128.03   | us        | 8M      |
| 1024  | Standard Deviation       | 0.47      | 0.47     | us        | 29.18k  |
| 1024  | Coefficient of Variation | 0.37%     | 0.36%    |           | 0.36%   |
| 4096  | Mean                     | 637.58    | 635.18   | us        | 6.45M   |
| 4096  | Median                   | 637.95    | 635.59   | us        | 6.44M   |
| 4096  | Standard Deviation       | 3.75      | 3.64     | us        | 37.1k   |
| 4096  | Coefficient of Variation | 0.59%     | 0.57%    |           | 0.58%   |
| 16384 | Mean                     | 2745.85   | 2734.74  | us        | 5.99M   |
| 16384 | Median                   | 2745.29   | 2734.28  | us        | 5.99M   |
| 16384 | Standard Deviation       | 2.9       | 2.84     | us        | 6.22k   |
| 16384 | Coefficient of Variation | 0.11%     | 0.1%     |           | 0.1%    |
| 65536 | Mean                     | 10871.7   | 10821.2  | us        | 6.06M   |
| 65536 | Median                   | 10863.1   | 10812.6  | us        | 6.06M   |
| 65536 | Standard Deviation       | 23.52     | 23.98    | us        | 13.4k   |
| 65536 | Coefficient of Variation | 0.22%     | 0.22%    |           | 0.22%   |
|       | Big O                    | 165.94    | 165.18   | N         |         |
|       | RMS                      | 11.84p    | 11.89p   |           |         |

#### BM_ConstructBinaryTree
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 8.7       | 8.68     | us        | 7.38M   |
| 64    | Median                   | 8.7       | 8.68     | us        | 7.38M   |
| 64    | Standard Deviation       | 0.02      | 0.02     | us        | 16.43k  |
| 64    | Coefficient of Variation | 0.25%     | 0.22%    |           | 0.22%   |
| 256   | Mean                     | 34.11     | 34.02    | us        | 7.53M   |
| 256   | Median                   | 34.13     | 34.04    | us        | 7.52M   |
| 256   | Standard Deviation       | 0.08      | 0.08     | us        | 18.17k  |
| 256   | Coefficient of Variation | 0.23%     | 0.24%    |           | 0.24%   |
| 1024  | Mean                     | 134.07    | 133.72   | us        | 7.66M   |
| 1024  | Median                   | 133.99    | 133.66   | us        | 7.66M   |
| 1024  | Standard Deviation       | 0.45      | 0.43     | us        | 24.89k  |
| 1024  | Coefficient of Variation | 0.34%     | 0.33%    |           | 0.33%   |
| 4096  | Mean                     | 515.9     | 514.49   | us        | 7.96M   |
| 4096  | Median                   | 515.49    | 514.08   | us        | 7.97M   |
| 4096  | Standard Deviation       | 1.86      | 1.83     | us        | 28.32k  |
| 4096  | Coefficient of Variation | 0.36%     | 0.36%    |           | 0.36%   |
| 16384 | Mean                     | 2055.12   | 2048.3   | us        | 8M      |
| 16384 | Median                   | 2054.9    | 2047.98  | us        | 8M      |
| 16384 | Standard Deviation       | 3.53      | 3.46     | us        | 13.53k  |
| 16384 | Coefficient of Variation | 0.17%     | 0.17%    |           | 0.17%   |
| 65536 | Mean                     | 11161.9   | 11118.9  | us        | 5.89M   |
| 65536 | Median                   | 11161.8   | 11118.4  | us        | 5.89M   |
| 65536 | Standard Deviation       | 15.69     | 15.43    | us        | 8.18k   |
| 65536 | Coefficient of Variation | 0.14%     | 0.14%    |           | 0.14%   |
|       | Big O                    | 10.57     | 10.53    | NlgN      |         |
|       | RMS                      | 66.75p    | 66.55p   |           |         |

#### BM_ConstructDenseDag
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 14.19     | 14.16    | us        | 142.39M |
| 64    | Median                   | 14.2      | 14.17    | us        | 142.26M |
| 64    | Standard Deviation       | 0.03      | 0.03     | us        | 273.36k |
| 64    | Coefficient of Variation | 0.19%     | 0.19%    |           | 0.19%   |
| 256   | Mean                     | 110.42    | 110.07   | us        | 296.54M |
| 256   | Median                   | 110.42    | 110.07   | us        | 296.53M |
| 256   | Standard Deviation       | 0.23      | 0.22     | us        | 604.76k |
| 256   | Coefficient of Variation | 0.21%     | 0.2%     |           | 0.2%    |
| 1024  | Mean                     | 4092.63   | 4062.04  | us        | 128.97M |
| 1024  | Median                   | 4091.63   | 4061.27  | us        | 128.97M |
| 1024  | Standard Deviation       | 62.38     | 64.14    | us        | 2.03M   |
| 1024  | Coefficient of Variation | 1.52%     | 1.58%    |           | 1.58%   |
| 4096  | Mean                     | 67261.5   | 66669.7  | us        | 125.79M |
| 4096  | Median                   | 67223.3   | 66636.1  | us        | 125.86M |
| 4096  | Standard Deviation       | 86.69     | 87.24    | us        | 164.42k |
| 4096  | Coefficient of Variation | 0.13%     | 0.13%    |           | 0.13%   |
|       | Big O                    | 4.01      | 3.97     | N^2       |         |
|       | RMS                      | 5.99p     | 5.93p    |           |         |

#### BM_OutEdgesChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 72.77     | 72.6     | ns        | 881.53M |
| 64    | Median                   | 72.71     | 72.55    | ns        | 882.19M |
| 64    | Standard Deviation       | 0.28      | 0.28     | ns        | 3.43M   |
| 64    | Coefficient of Variation | 0.39%     | 0.39%    |           | 0.39%   |
| 256   | Mean                     | 296.6     | 295.93   | ns        | 865.09M |
| 256   | Median                   | 296.35    | 295.67   | ns        | 865.83M |
| 256   | Standard Deviation       | 1.1       | 1.1      | ns        | 3.2M    |
| 256   | Coefficient of Variation | 0.37%     | 0.37%    |           | 0.37%   |
| 1024  | Mean                     | 1165.6    | 1162.99  | ns        | 880.49M |
| 1024  | Median                   | 1163.87   | 1161.24  | ns        | 881.82M |
| 1024  | Standard Deviation       | 3.23      | 3.29     | ns        | 2.48M   |
| 1024  | Coefficient of Variation | 0.28%     | 0.28%    |           | 0.28%   |
| 4096  | Mean                     | 4692.19   | 4681.67  | ns        | 874.91M |
| 4096  | Median                   | 4691.81   | 4681.25  | ns        | 874.98M |
| 4096  | Standard Deviation       | 17.59     | 17.44    | ns        | 3.26M   |
| 4096  | Coefficient of Variation | 0.37%     | 0.37%    |           | 0.37%   |
| 16384 | Mean                     | 18679.3   | 18637.8  | ns        | 879.08M |
| 16384 | Median                   | 18663.4   | 18623.2  | ns        | 879.76M |
| 16384 | Standard Deviation       | 46        | 45.48    | ns        | 2.14M   |
| 16384 | Coefficient of Variation | 0.25%     | 0.24%    |           | 0.24%   |
| 65536 | Mean                     | 75423.6   | 75203.2  | ns        | 871.46M |
| 65536 | Median                   | 75395.9   | 75174.2  | ns        | 871.79M |
| 65536 | Standard Deviation       | 190.63    | 188.19   | ns        | 2.18M   |
| 65536 | Coefficient of Variation | 0.25%     | 0.25%    |           | 0.25%   |
|       | Big O                    | 1.15      | 1.15     | N         |         |
|       | RMS                      | 6.21p     | 5.97p    |           |         |

#### BM_InEdgesChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 72.59     | 72.43    | ns        | 883.62M |
| 64    | Median                   | 72.44     | 72.29    | ns        | 885.37M |
| 64    | Standard Deviation       | 0.47      | 0.47     | ns        | 5.65M   |
| 64    | Coefficient of Variation | 0.64%     | 0.64%    |           | 0.64%   |
| 256   | Mean                     | 296.4     | 295.76   | ns        | 865.58M |
| 256   | Median                   | 295.96    | 295.32   | ns        | 866.85M |
| 256   | Standard Deviation       | 1.08      | 1.07     | ns        | 3.11M   |
| 256   | Coefficient of Variation | 0.36%     | 0.36%    |           | 0.36%   |
| 1024  | Mean                     | 1168.44   | 1165.89  | ns        | 878.33M |
| 1024  | Median                   | 1166.29   | 1163.8   | ns        | 879.88M |
| 1024  | Standard Deviation       | 6.5       | 6.48     | ns        | 4.84M   |
| 1024  | Coefficient of Variation | 0.56%     | 0.56%    |           | 0.55%   |
| 4096  | Mean                     | 4667.55   | 4657.32  | ns        | 879.49M |
| 4096  | Median                   | 4659.75   | 4649.6   | ns        | 880.94M |
| 4096  | Standard Deviation       | 15.91     | 15.81    | ns        | 2.97M   |
| 4096  | Coefficient of Variation | 0.34%     | 0.34%    |           | 0.34%   |
| 16384 | Mean                     | 18675.8   | 18634.2  | ns        | 879.25M |
| 16384 | Median                   | 18665.4   | 18624.8  | ns        | 879.69M |
| 16384 | Standard Deviation       | 65.61     | 64.85    | ns        | 3.05M   |
| 16384 | Coefficient of Variation | 0.35%     | 0.35%    |           | 0.35%   |
| 65536 | Mean                     | 74942.6   | 74742    | ns        | 876.83M |
| 65536 | Median                   | 74885.3   | 74686.5  | ns        | 877.48M |
| 65536 | Standard Deviation       | 147.08    | 144.75   | ns        | 1.7M    |
| 65536 | Coefficient of Variation | 0.2%      | 0.19%    |           | 0.19%   |
|       | Big O                    | 1.14      | 1.14     | N         |         |
|       | RMS                      | 4.05p     | 3.93p    |           |         |

#### BM_OutEdgesDense
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 542.44    | 541.3    | ns        | 3.72G   |
| 64    | Median                   | 542.13    | 541.01   | ns        | 3.73G   |
| 64    | Standard Deviation       | 0.79      | 0.77     | ns        | 5.29M   |
| 64    | Coefficient of Variation | 0.15%     | 0.14%    |           | 0.14%   |
| 256   | Mean                     | 9575.82   | 9555.82  | ns        | 3.42G   |
| 256   | Median                   | 9569.75   | 9549.93  | ns        | 3.42G   |
| 256   | Standard Deviation       | 25.68     | 25.14    | ns        | 8.94M   |
| 256   | Coefficient of Variation | 0.27%     | 0.26%    |           | 0.26%   |
| 1024  | Mean                     | 144103    | 143650   | ns        | 3.65G   |
| 1024  | Median                   | 144053    | 143591   | ns        | 3.65G   |
| 1024  | Standard Deviation       | 341.26    | 343.7    | ns        | 8.71M   |
| 1024  | Coefficient of Variation | 0.24%     | 0.24%    |           | 0.24%   |
| 4096  | Mean                     | 2304730   | 2271570  | ns        | 3.69G   |
| 4096  | Median                   | 2304940   | 2271360  | ns        | 3.69G   |
| 4096  | Standard Deviation       | 1204.36   | 924.88   | ns        | 1.5M    |
| 4096  | Coefficient of Variation | 0.05%     | 0.04%    |           | 0.04%   |
|       | Big O                    | 137.37m   | 135.4m   | N^2       |         |
|       | RMS                      | 1.07p     | 1.68p    |           |         |

#### BM_IsAcyclicTrue
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 208.94    | 208.48   | ns        | 306.98M |
| 64    | Median                   | 208.95    | 208.49   | ns        | 306.97M |
| 64    | Standard Deviation       | 0.25      | 0.24     | ns        | 350.5k  |
| 64    | Coefficient of Variation | 0.12%     | 0.11%    |           | 0.11%   |
| 256   | Mean                     | 788.91    | 787.18   | ns        | 325.21M |
| 256   | Median                   | 789.07    | 787.37   | ns        | 325.13M |
| 256   | Standard Deviation       | 0.55      | 0.53     | ns        | 220.64k |
| 256   | Coefficient of Variation | 0.07%     | 0.07%    |           | 0.07%   |
| 1024  | Mean                     | 3142.96   | 3136.03  | ns        | 326.53M |
| 1024  | Median                   | 3145.06   | 3137.87  | ns        | 326.34M |
| 1024  | Standard Deviation       | 11.6      | 11.63    | ns        | 1.22M   |
| 1024  | Coefficient of Variation | 0.37%     | 0.37%    |           | 0.37%   |
| 4096  | Mean                     | 12339.7   | 12312.5  | ns        | 332.7M  |
| 4096  | Median                   | 12282.3   | 12255.7  | ns        | 334.21M |
| 4096  | Standard Deviation       | 129       | 128.82   | ns        | 3.43M   |
| 4096  | Coefficient of Variation | 1.05%     | 1.05%    |           | 1.03%   |
| 16384 | Mean                     | 49027.8   | 48904.2  | ns        | 335.02M |
| 16384 | Median                   | 49054.1   | 48929.2  | ns        | 334.85M |
| 16384 | Standard Deviation       | 96.34     | 94.47    | ns        | 650.23k |
| 16384 | Coefficient of Variation | 0.2%      | 0.19%    |           | 0.19%   |
| 65536 | Mean                     | 202342    | 201588   | ns        | 325.1M  |
| 65536 | Median                   | 202291    | 201540   | ns        | 325.18M |
| 65536 | Standard Deviation       | 486.21    | 481.36   | ns        | 775.42k |
| 65536 | Coefficient of Variation | 0.24%     | 0.24%    |           | 0.24%   |
|       | Big O                    | 3.08      | 3.07     | N         |         |
|       | RMS                      | 14.75p    | 14.21p   |           |         |

#### BM_IsAcyclicFalse
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 58.65     | 58.52    | ns        | 1.09G   |
| 64    | Median                   | 58.67     | 58.53    | ns        | 1.09G   |
| 64    | Standard Deviation       | 0.09      | 0.09     | ns        | 1.66M   |
| 64    | Coefficient of Variation | 0.15%     | 0.15%    |           | 0.15%   |
| 256   | Mean                     | 150.51    | 150.18   | ns        | 1.7G    |
| 256   | Median                   | 150.42    | 150.1    | ns        | 1.71G   |
| 256   | Standard Deviation       | 0.25      | 0.24     | ns        | 2.71M   |
| 256   | Coefficient of Variation | 0.16%     | 0.16%    |           | 0.16%   |
| 1024  | Mean                     | 564.42    | 563.15   | ns        | 1.82G   |
| 1024  | Median                   | 564.79    | 563.49   | ns        | 1.82G   |
| 1024  | Standard Deviation       | 0.74      | 0.74     | ns        | 2.38M   |
| 1024  | Coefficient of Variation | 0.13%     | 0.13%    |           | 0.13%   |
| 4096  | Mean                     | 2091.84   | 2087.21  | ns        | 1.96G   |
| 4096  | Median                   | 2091.34   | 2086.68  | ns        | 1.96G   |
| 4096  | Standard Deviation       | 2.25      | 2.28     | ns        | 2.14M   |
| 4096  | Coefficient of Variation | 0.11%     | 0.11%    |           | 0.11%   |
| 16384 | Mean                     | 8128.64   | 8110.44  | ns        | 2.02G   |
| 16384 | Median                   | 8128.92   | 8110.85  | ns        | 2.02G   |
| 16384 | Standard Deviation       | 3.85      | 3.54     | ns        | 881.21k |
| 16384 | Coefficient of Variation | 0.05%     | 0.04%    |           | 0.04%   |
| 65536 | Mean                     | 33120.9   | 33032.3  | ns        | 1.98G   |
| 65536 | Median                   | 33086.8   | 32999.7  | ns        | 1.99G   |
| 65536 | Standard Deviation       | 77        | 76.39    | ns        | 4.58M   |
| 65536 | Coefficient of Variation | 0.23%     | 0.23%    |           | 0.23%   |
|       | Big O                    | 504.87m   | 503.54m  | N         |         |
|       | RMS                      | 9.77p     | 9.61p    |           |         |

#### BM_IsAcyclicDense
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 884.32    | 882.42   | ns        | 72.53M  |
| 64    | Median                   | 884.48    | 882.6    | ns        | 72.51M  |
| 64    | Standard Deviation       | 2.42      | 2.41     | ns        | 198.27k |
| 64    | Coefficient of Variation | 0.27%     | 0.27%    |           | 0.27%   |
| 256   | Mean                     | 12212     | 12185.3  | ns        | 21.01M  |
| 256   | Median                   | 12249.7   | 12221.6  | ns        | 20.95M  |
| 256   | Standard Deviation       | 127.42    | 127.19   | ns        | 220.16k |
| 256   | Coefficient of Variation | 1.04%     | 1.04%    |           | 1.05%   |
| 1024  | Mean                     | 179623    | 178922   | ns        | 5.72M   |
| 1024  | Median                   | 179687    | 178988   | ns        | 5.72M   |
| 1024  | Standard Deviation       | 982.39    | 986.61   | ns        | 31.49k  |
| 1024  | Coefficient of Variation | 0.55%     | 0.55%    |           | 0.55%   |
| 4096  | Mean                     | 3701210   | 3653010  | ns        | 1.12M   |
| 4096  | Median                   | 3700420   | 3652430  | ns        | 1.12M   |
| 4096  | Standard Deviation       | 17229.1   | 17093    | ns        | 5.23k   |
| 4096  | Coefficient of Variation | 0.47%     | 0.47%    |           | 0.47%   |
| 8192  | Mean                     | 15339500  | 15140400 | ns        | 541.07k |
| 8192  | Median                   | 15320000  | 15124500 | ns        | 541.64k |
| 8192  | Standard Deviation       | 49559     | 50812.5  | ns        | 1.81k   |
| 8192  | Coefficient of Variation | 0.32%     | 0.34%    |           | 0.33%   |
|       | Big O                    | 228.09m   | 225.13m  | N^2       |         |
|       | RMS                      | 17.57p    | 17.58p   |           |         |

#### BM_TopoSortChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 338.86    | 338.06   | ns        | 189.31M |
| 64    | Median                   | 338.56    | 337.77   | ns        | 189.48M |
| 64    | Standard Deviation       | 0.94      | 0.89     | ns        | 493.1k  |
| 64    | Coefficient of Variation | 0.28%     | 0.26%    |           | 0.26%   |
| 256   | Mean                     | 1327.1    | 1324.1   | ns        | 193.34M |
| 256   | Median                   | 1326.72   | 1323.7   | ns        | 193.4M  |
| 256   | Standard Deviation       | 1.81      | 1.82     | ns        | 266.27k |
| 256   | Coefficient of Variation | 0.14%     | 0.14%    |           | 0.14%   |
| 1024  | Mean                     | 5386.43   | 5374.39  | ns        | 190.94M |
| 1024  | Median                   | 5340.52   | 5328.47  | ns        | 192.43M |
| 1024  | Standard Deviation       | 263.25    | 262.67   | ns        | 9.31M   |
| 1024  | Coefficient of Variation | 4.89%     | 4.89%    |           | 4.87%   |
| 4096  | Mean                     | 21538.8   | 21490.4  | ns        | 192.07M |
| 4096  | Median                   | 20748     | 20701.9  | ns        | 197.86M |
| 4096  | Standard Deviation       | 2208.28   | 2203.54  | ns        | 16.05M  |
| 4096  | Coefficient of Variation | 10.25%    | 10.25%   |           | 8.36%   |
| 16384 | Mean                     | 84428.7   | 84217.4  | ns        | 194.56M |
| 16384 | Median                   | 84772.4   | 84565.9  | ns        | 193.74M |
| 16384 | Standard Deviation       | 840.26    | 840.29   | ns        | 1.97M   |
| 16384 | Coefficient of Variation | 1%        | 1%       |           | 1.01%   |
| 65536 | Mean                     | 342961    | 341858   | ns        | 191.71M |
| 65536 | Median                   | 343238    | 342152   | ns        | 191.54M |
| 65536 | Standard Deviation       | 1559.59   | 1542.31  | ns        | 865.59k |
| 65536 | Coefficient of Variation | 0.45%     | 0.45%    |           | 0.45%   |
|       | Big O                    | 5.23      | 5.21     | N         |         |
|       | RMS                      | 16.04p    | 15.89p   |           |         |

#### BM_TopoSortBinaryTree
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 269.57    | 268.95   | ns        | 237.96M |
| 64    | Median                   | 269.43    | 268.8    | ns        | 238.1M  |
| 64    | Standard Deviation       | 0.33      | 0.33     | ns        | 292.69k |
| 64    | Coefficient of Variation | 0.12%     | 0.12%    |           | 0.12%   |
| 256   | Mean                     | 1115.57   | 1113.03  | ns        | 230.25M |
| 256   | Median                   | 1096.09   | 1093.58  | ns        | 234.09M |
| 256   | Standard Deviation       | 39.33     | 39.22    | ns        | 7.9M    |
| 256   | Coefficient of Variation | 3.53%     | 3.52%    |           | 3.43%   |
| 1024  | Mean                     | 4812.58   | 4801.75  | ns        | 214.25M |
| 1024  | Median                   | 4798.08   | 4787.26  | ns        | 213.9M  |
| 1024  | Standard Deviation       | 340.47    | 339.73   | ns        | 15.74M  |
| 1024  | Coefficient of Variation | 7.07%     | 7.08%    |           | 7.34%   |
| 4096  | Mean                     | 17624.8   | 17583.7  | ns        | 232.95M |
| 4096  | Median                   | 17646.4   | 17605    | ns        | 232.66M |
| 4096  | Standard Deviation       | 95.5      | 95.01    | ns        | 1.27M   |
| 4096  | Coefficient of Variation | 0.54%     | 0.54%    |           | 0.54%   |
| 16384 | Mean                     | 73994.5   | 73801.8  | ns        | 222.04M |
| 16384 | Median                   | 74443.1   | 74245.3  | ns        | 220.67M |
| 16384 | Standard Deviation       | 1005.49   | 1006.4   | ns        | 3.07M   |
| 16384 | Coefficient of Variation | 1.36%     | 1.36%    |           | 1.38%   |
| 65536 | Mean                     | 301383    | 300415   | ns        | 218.15M |
| 65536 | Median                   | 301324    | 300362   | ns        | 218.19M |
| 65536 | Standard Deviation       | 416.44    | 403.77   | ns        | 293.15k |
| 65536 | Coefficient of Variation | 0.14%     | 0.13%    |           | 0.13%   |
|       | Big O                    | 4.59      | 4.58     | N         |         |
|       | RMS                      | 12.77p    | 12.55p   |           |         |

#### BM_TopoSortDense
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 2.1       | 2.09     | us        | 30.61M  |
| 64    | Median                   | 2.1       | 2.09     | us        | 30.61M  |
| 64    | Standard Deviation       | 0         | 0        | us        | 15.32k  |
| 64    | Coefficient of Variation | 0.05%     | 0.05%    |           | 0.05%   |
| 256   | Mean                     | 32        | 31.93    | us        | 8.02M   |
| 256   | Median                   | 32.03     | 31.96    | us        | 8.01M   |
| 256   | Standard Deviation       | 0.18      | 0.18     | us        | 46.55k  |
| 256   | Coefficient of Variation | 0.58%     | 0.58%    |           | 0.58%   |
| 1024  | Mean                     | 489.11    | 487.51   | us        | 2.1M    |
| 1024  | Median                   | 489.25    | 487.65   | us        | 2.1M    |
| 1024  | Standard Deviation       | 1.63      | 1.57     | us        | 6.78k   |
| 1024  | Coefficient of Variation | 0.33%     | 0.32%    |           | 0.32%   |
| 4096  | Mean                     | 9270.1    | 9173.44  | us        | 446.51k |
| 4096  | Median                   | 9266.68   | 9166.53  | us        | 446.84k |
| 4096  | Standard Deviation       | 15.23     | 15.51    | us        | 753.5   |
| 4096  | Coefficient of Variation | 0.16%     | 0.17%    |           | 0.17%   |
| 8192  | Mean                     | 37783.6   | 37349.5  | us        | 219.33k |
| 8192  | Median                   | 37775.8   | 37330.8  | us        | 219.44k |
| 8192  | Standard Deviation       | 80.35     | 77.96    | us        | 457.45  |
| 8192  | Coefficient of Variation | 0.21%     | 0.21%    |           | 0.21%   |
|       | Big O                    | 562.38m   | 555.95m  | N^2       |         |
|       | RMS                      | 10p       | 9.52p    |           |         |

#### BM_NodeAccessSequential
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 14.51     | 14.47    | ns        | 4.42G   |
| 64    | Median                   | 14.5      | 14.47    | ns        | 4.42G   |
| 64    | Standard Deviation       | 0.01      | 0.01     | ns        | 3.53M   |
| 64    | Coefficient of Variation | 0.06%     | 0.08%    |           | 0.08%   |
| 256   | Mean                     | 55.39     | 55.28    | ns        | 4.63G   |
| 256   | Median                   | 55.35     | 55.24    | ns        | 4.63G   |
| 256   | Standard Deviation       | 0.08      | 0.08     | ns        | 6.31M   |
| 256   | Coefficient of Variation | 0.14%     | 0.14%    |           | 0.14%   |
| 1024  | Mean                     | 442.93    | 442.02   | ns        | 2.32G   |
| 1024  | Median                   | 442.9     | 441.99   | ns        | 2.32G   |
| 1024  | Standard Deviation       | 0.15      | 0.14     | ns        | 749.19k |
| 1024  | Coefficient of Variation | 0.03%     | 0.03%    |           | 0.03%   |
| 4096  | Mean                     | 1782.84   | 1778.71  | ns        | 2.3G    |
| 4096  | Median                   | 1772.34   | 1768.53  | ns        | 2.32G   |
| 4096  | Standard Deviation       | 20.91     | 20.18    | ns        | 25.61M  |
| 4096  | Coefficient of Variation | 1.17%     | 1.13%    |           | 1.11%   |
| 16384 | Mean                     | 8569.16   | 8544.54  | ns        | 1.92G   |
| 16384 | Median                   | 8598.32   | 8573.87  | ns        | 1.91G   |
| 16384 | Standard Deviation       | 126.78    | 126.12   | ns        | 28.38M  |
| 16384 | Coefficient of Variation | 1.48%     | 1.48%    |           | 1.48%   |
| 65536 | Mean                     | 35128.3   | 35017.8  | ns        | 1.87G   |
| 65536 | Median                   | 34994.7   | 34881.9  | ns        | 1.88G   |
| 65536 | Standard Deviation       | 287.47    | 285.07   | ns        | 15.06M  |
| 65536 | Coefficient of Variation | 0.82%     | 0.81%    |           | 0.8%    |
|       | Big O                    | 534.86m   | 533.18m  | N         |         |
|       | RMS                      | 29.99p    | 29.84p   |           |         |

#### BM_NodeAccessRandom
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 20.89     | 20.84    | ns        | 3.07G   |
| 64    | Median                   | 20.88     | 20.83    | ns        | 3.07G   |
| 64    | Standard Deviation       | 0.06      | 0.06     | ns        | 8.16M   |
| 64    | Coefficient of Variation | 0.27%     | 0.27%    |           | 0.27%   |
| 256   | Mean                     | 83.83     | 83.65    | ns        | 3.06G   |
| 256   | Median                   | 83.85     | 83.67    | ns        | 3.06G   |
| 256   | Standard Deviation       | 0.14      | 0.14     | ns        | 5.06M   |
| 256   | Coefficient of Variation | 0.17%     | 0.17%    |           | 0.17%   |
| 1024  | Mean                     | 521.07    | 519.97   | ns        | 1.97G   |
| 1024  | Median                   | 520.99    | 519.9    | ns        | 1.97G   |
| 1024  | Standard Deviation       | 0.5       | 0.5      | ns        | 1.89M   |
| 1024  | Coefficient of Variation | 0.1%      | 0.1%     |           | 0.1%    |
| 4096  | Mean                     | 2074.14   | 2069.83  | ns        | 1.98G   |
| 4096  | Median                   | 2075      | 2070.68  | ns        | 1.98G   |
| 4096  | Standard Deviation       | 3.1       | 3.08     | ns        | 2.95M   |
| 4096  | Coefficient of Variation | 0.15%     | 0.15%    |           | 0.15%   |
| 16384 | Mean                     | 12988     | 12951.1  | ns        | 1.27G   |
| 16384 | Median                   | 12987.3   | 12950.7  | ns        | 1.27G   |
| 16384 | Standard Deviation       | 50.82     | 50.77    | ns        | 4.96M   |
| 16384 | Coefficient of Variation | 0.39%     | 0.39%    |           | 0.39%   |
| 65536 | Mean                     | 57296.7   | 57120.1  | ns        | 1.15G   |
| 65536 | Median                   | 57220.1   | 57044.4  | ns        | 1.15G   |
| 65536 | Standard Deviation       | 185.09    | 182.19   | ns        | 3.65M   |
| 65536 | Coefficient of Variation | 0.32%     | 0.32%    |           | 0.32%   |
|       | Big O                    | 54.71m    | 54.54m   | NlgN      |         |
|       | RMS                      | 26.24p    | 26.22p   |           |         |

#### BM_DijkstraChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 512.06    | 510.93   | ns        | 125.26M |
| 64    | Median                   | 511.89    | 510.78   | ns        | 125.3M  |
| 64    | Standard Deviation       | 0.77      | 0.73     | ns        | 179.09k |
| 64    | Coefficient of Variation | 0.15%     | 0.14%    |           | 0.14%   |
| 256   | Mean                     | 2058.58   | 2054.01  | ns        | 124.64M |
| 256   | Median                   | 2059.1    | 2054.66  | ns        | 124.59M |
| 256   | Standard Deviation       | 6.37      | 6.29     | ns        | 381.1k  |
| 256   | Coefficient of Variation | 0.31%     | 0.31%    |           | 0.31%   |
| 1024  | Mean                     | 8129.33   | 8111.64  | ns        | 126.24M |
| 1024  | Median                   | 8123.33   | 8105.69  | ns        | 126.33M |
| 1024  | Standard Deviation       | 17.62     | 17.67    | ns        | 273.54k |
| 1024  | Coefficient of Variation | 0.22%     | 0.22%    |           | 0.22%   |
| 4096  | Mean                     | 32442     | 32368.8  | ns        | 126.54M |
| 4096  | Median                   | 32437.1   | 32363.7  | ns        | 126.56M |
| 4096  | Standard Deviation       | 18.14     | 18.17    | ns        | 70.99k  |
| 4096  | Coefficient of Variation | 0.06%     | 0.06%    |           | 0.06%   |
| 16384 | Mean                     | 129726    | 129407   | ns        | 126.61M |
| 16384 | Median                   | 129630    | 129312   | ns        | 126.7M  |
| 16384 | Standard Deviation       | 327.54    | 326.65   | ns        | 318.73k |
| 16384 | Coefficient of Variation | 0.25%     | 0.25%    |           | 0.25%   |
| 65536 | Mean                     | 521487    | 519849   | ns        | 126.07M |
| 65536 | Median                   | 521746    | 520133   | ns        | 126M    |
| 65536 | Standard Deviation       | 731.95    | 733.81   | ns        | 178.12k |
| 65536 | Coefficient of Variation | 0.14%     | 0.14%    |           | 0.14%   |
|       | Big O                    | 7.95      | 7.93     | N         |         |
|       | RMS                      | 3.51p     | 3.33p    |           |         |

#### BM_DijkstraBinaryTree
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 742.24    | 740.6    | ns        | 86.42M  |
| 64    | Median                   | 741.92    | 740.28   | ns        | 86.45M  |
| 64    | Standard Deviation       | 1.29      | 1.29     | ns        | 150.16k |
| 64    | Coefficient of Variation | 0.17%     | 0.17%    |           | 0.17%   |
| 256   | Mean                     | 3652.75   | 3644.3   | ns        | 70.25M  |
| 256   | Median                   | 3662.13   | 3653.71  | ns        | 70.07M  |
| 256   | Standard Deviation       | 22.52     | 22.53    | ns        | 435.32k |
| 256   | Coefficient of Variation | 0.62%     | 0.62%    |           | 0.62%   |
| 1024  | Mean                     | 17990.9   | 17949.3  | ns        | 57.05M  |
| 1024  | Median                   | 17990     | 17949.1  | ns        | 57.05M  |
| 1024  | Standard Deviation       | 15.57     | 15.83    | ns        | 50.29k  |
| 1024  | Coefficient of Variation | 0.09%     | 0.09%    |           | 0.09%   |
| 4096  | Mean                     | 90256.1   | 90050.8  | ns        | 45.49M  |
| 4096  | Median                   | 90226.3   | 90018.4  | ns        | 45.5M   |
| 4096  | Standard Deviation       | 107.83    | 105.98   | ns        | 53.49k  |
| 4096  | Coefficient of Variation | 0.12%     | 0.12%    |           | 0.12%   |
| 16384 | Mean                     | 644946    | 642959   | ns        | 25.48M  |
| 16384 | Median                   | 645584    | 643673   | ns        | 25.45M  |
| 16384 | Standard Deviation       | 2342.39   | 2364.21  | ns        | 93.76k  |
| 16384 | Coefficient of Variation | 0.36%     | 0.37%    |           | 0.37%   |
| 65536 | Mean                     | 3384940   | 3374250  | ns        | 19.43M  |
| 65536 | Median                   | 3400980   | 3390270  | ns        | 19.33M  |
| 65536 | Standard Deviation       | 41632.2   | 41472.6  | ns        | 242.84k |
| 65536 | Coefficient of Variation | 1.23%     | 1.23%    |           | 1.25%   |
|       | Big O                    | 3.21      | 3.2      | NlgN      |         |
|       | RMS                      | 72.52p    | 72.47p   |           |         |

#### BM_DijkstraDense
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 3.48      | 3.47     | us        | 18.44M  |
| 64    | Median                   | 3.48      | 3.47     | us        | 18.44M  |
| 64    | Standard Deviation       | 0.02      | 0.02     | us        | 83.33k  |
| 64    | Coefficient of Variation | 0.45%     | 0.45%    |           | 0.45%   |
| 128   | Mean                     | 11.48     | 11.46    | us        | 11.17M  |
| 128   | Median                   | 11.49     | 11.46    | us        | 11.17M  |
| 128   | Standard Deviation       | 0.02      | 0.02     | us        | 22.69k  |
| 128   | Coefficient of Variation | 0.21%     | 0.2%     |           | 0.2%    |
| 256   | Mean                     | 36.78     | 36.67    | us        | 6.98M   |
| 256   | Median                   | 36.71     | 36.62    | us        | 6.99M   |
| 256   | Standard Deviation       | 0.16      | 0.12     | us        | 23.12k  |
| 256   | Coefficient of Variation | 0.44%     | 0.33%    |           | 0.33%   |
| 512   | Mean                     | 140.91    | 140.46   | us        | 3.65M   |
| 512   | Median                   | 140.82    | 140.37   | us        | 3.65M   |
| 512   | Standard Deviation       | 1.15      | 1.15     | us        | 29.91k  |
| 512   | Coefficient of Variation | 0.82%     | 0.82%    |           | 0.82%   |
| 1024  | Mean                     | 511.18    | 508.82   | us        | 2.01M   |
| 1024  | Median                   | 511.49    | 508.99   | us        | 2.01M   |
| 1024  | Standard Deviation       | 17.37     | 17.21    | us        | 68.19k  |
| 1024  | Coefficient of Variation | 3.4%      | 3.38%    |           | 3.38%   |
| 2048  | Mean                     | 2390.83   | 2378.3   | us        | 861.15k |
| 2048  | Median                   | 2393.09   | 2380.59  | us        | 860.29k |
| 2048  | Standard Deviation       | 14.38     | 14.22    | us        | 5.16k   |
| 2048  | Coefficient of Variation | 0.6%      | 0.6%     |           | 0.6%    |
| 4096  | Mean                     | 9693.35   | 9578.45  | us        | 427.63k |
| 4096  | Median                   | 9693.83   | 9580.5   | us        | 427.54k |
| 4096  | Standard Deviation       | 25.58     | 24.88    | us        | 1.11k   |
| 4096  | Coefficient of Variation | 0.26%     | 0.26%    |           | 0.26%   |
|       | Big O                    | 576.98m   | 570.37m  | N^2       |         |
|       | RMS                      | 21.69p    | 20.2p    |           |         |

#### BM_DijkstraSparse
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 1024  | Mean                     | 25.45     | 25.38    | us        | 40.34M  |
| 1024  | Median                   | 25.42     | 25.36    | us        | 40.38M  |
| 1024  | Standard Deviation       | 0.09      | 0.09     | us        | 138.49k |
| 1024  | Coefficient of Variation | 0.34%     | 0.34%    |           | 0.34%   |
| 4096  | Mean                     | 104.64    | 104.36   | us        | 39.25M  |
| 4096  | Median                   | 104.57    | 104.3    | us        | 39.27M  |
| 4096  | Standard Deviation       | 0.48      | 0.48     | us        | 179.56k |
| 4096  | Coefficient of Variation | 0.46%     | 0.46%    |           | 0.46%   |
| 16384 | Mean                     | 812.14    | 809.41   | us        | 20.24M  |
| 16384 | Median                   | 812.54    | 809.76   | us        | 20.23M  |
| 16384 | Standard Deviation       | 2.99      | 2.95     | us        | 73.88k  |
| 16384 | Coefficient of Variation | 0.37%     | 0.36%    |           | 0.36%   |
| 65536 | Mean                     | 3161.06   | 3150.6   | us        | 20.8M   |
| 65536 | Median                   | 3160.61   | 3150.04  | us        | 20.8M   |
| 65536 | Standard Deviation       | 6.29      | 6.29     | us        | 41.58k  |
| 65536 | Coefficient of Variation | 0.2%      | 0.2%     |           | 0.2%    |
|       | Big O                    | 48.22     | 48.06    | N         |         |
|       | RMS                      | 48.08p    | 48.04p   |           |         |

#### BM_DijkstraSparse_PointToPoint
| Param   | Metric                   | Real time | CPU time | Time unit | items/s |
|---------|--------------------------|-----------|----------|-----------|---------|
| 256     | Mean                     | 499.68    | 498.54   | ns        | 513.5M  |
| 256     | Median                   | 499.75    | 498.61   | ns        | 513.42M |
| 256     | Standard Deviation       | 0.73      | 0.7      | ns        | 725.23k |
| 256     | Coefficient of Variation | 0.15%     | 0.14%    |           | 0.14%   |
| 1024    | Mean                     | 1949.21   | 1944.7   | ns        | 526.56M |
| 1024    | Median                   | 1948.46   | 1943.81  | ns        | 526.8M  |
| 1024    | Standard Deviation       | 4.75      | 4.7      | ns        | 1.27M   |
| 1024    | Coefficient of Variation | 0.24%     | 0.24%    |           | 0.24%   |
| 4096    | Mean                     | 9221.88   | 9200.47  | ns        | 445.26M |
| 4096    | Median                   | 9157.35   | 9135.2   | ns        | 448.38M |
| 4096    | Standard Deviation       | 120       | 120      | ns        | 5.76M   |
| 4096    | Coefficient of Variation | 1.3%      | 1.3%     |           | 1.29%   |
| 16384   | Mean                     | 34851.8   | 34771.6  | ns        | 471.19M |
| 16384   | Median                   | 34806.5   | 34726    | ns        | 471.81M |
| 16384   | Standard Deviation       | 89.59     | 88.23    | ns        | 1.19M   |
| 16384   | Coefficient of Variation | 0.26%     | 0.25%    |           | 0.25%   |
| 65536   | Mean                     | 155724    | 155291   | ns        | 422.03M |
| 65536   | Median                   | 155480    | 155050   | ns        | 422.68M |
| 65536   | Standard Deviation       | 696.84    | 694.16   | ns        | 1.87M   |
| 65536   | Coefficient of Variation | 0.45%     | 0.45%    |           | 0.44%   |
| 262144  | Mean                     | 1071280   | 1067710  | ns        | 245.52M |
| 262144  | Median                   | 1070940   | 1067390  | ns        | 245.59M |
| 262144  | Standard Deviation       | 4288.33   | 4277.87  | ns        | 983.13k |
| 262144  | Coefficient of Variation | 0.4%      | 0.4%     |           | 0.4%    |
| 1048576 | Mean                     | 4353170   | 4338890  | ns        | 241.72M |
| 1048576 | Median                   | 4365540   | 4351140  | ns        | 240.99M |
| 1048576 | Standard Deviation       | 63489.7   | 63126.4  | ns        | 3.52M   |
| 1048576 | Coefficient of Variation | 1.46%     | 1.45%    |           | 1.46%   |
|         | Big O                    | 208.36m   | 207.68m  | NlgN      |         |
|         | RMS                      | 59.08p    | 59.01p   |           |         |

#### BM_VcsrConstructChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 9.33      | 9.31     | us        | 6.88M   |
| 64    | Median                   | 9.32      | 9.29     | us        | 6.89M   |
| 64    | Standard Deviation       | 0.03      | 0.03     | us        | 21.28k  |
| 64    | Coefficient of Variation | 0.31%     | 0.31%    |           | 0.31%   |
| 256   | Mean                     | 38.22     | 38.12    | us        | 6.72M   |
| 256   | Median                   | 38.29     | 38.2     | us        | 6.7M    |
| 256   | Standard Deviation       | 0.17      | 0.17     | us        | 29.63k  |
| 256   | Coefficient of Variation | 0.43%     | 0.44%    |           | 0.44%   |
| 1024  | Mean                     | 146.53    | 146.16   | us        | 7.01M   |
| 1024  | Median                   | 146.47    | 146.1    | us        | 7.01M   |
| 1024  | Standard Deviation       | 0.22      | 0.21     | us        | 9.98k   |
| 1024  | Coefficient of Variation | 0.15%     | 0.14%    |           | 0.14%   |
| 4096  | Mean                     | 559.58    | 557.88   | us        | 7.34M   |
| 4096  | Median                   | 559.38    | 557.5    | us        | 7.35M   |
| 4096  | Standard Deviation       | 1.12      | 1.13     | us        | 14.88k  |
| 4096  | Coefficient of Variation | 0.2%      | 0.2%     |           | 0.2%    |
| 16384 | Mean                     | 2258.61   | 2251.57  | us        | 7.28M   |
| 16384 | Median                   | 2258.59   | 2251.44  | us        | 7.28M   |
| 16384 | Standard Deviation       | 1.27      | 1.26     | us        | 4.09k   |
| 16384 | Coefficient of Variation | 0.06%     | 0.06%    |           | 0.06%   |
| 65536 | Mean                     | 9096.65   | 9065.44  | us        | 7.23M   |
| 65536 | Median                   | 9100.03   | 9069.51  | us        | 7.23M   |
| 65536 | Standard Deviation       | 17.95     | 18.24    | us        | 14.58k  |
| 65536 | Coefficient of Variation | 0.2%      | 0.2%     |           | 0.2%    |
|       | Big O                    | 138.74    | 138.27   | N         |         |
|       | RMS                      | 5.05p     | 5.01p    |           |         |

#### BM_VcsrConstructBinaryTree
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 9.51      | 9.49     | us        | 6.75M   |
| 64    | Median                   | 9.51      | 9.49     | us        | 6.75M   |
| 64    | Standard Deviation       | 0.03      | 0.03     | us        | 18.86k  |
| 64    | Coefficient of Variation | 0.28%     | 0.28%    |           | 0.28%   |
| 256   | Mean                     | 38.49     | 38.38    | us        | 6.67M   |
| 256   | Median                   | 38.49     | 38.36    | us        | 6.67M   |
| 256   | Standard Deviation       | 0.09      | 0.09     | us        | 16.32k  |
| 256   | Coefficient of Variation | 0.25%     | 0.25%    |           | 0.24%   |
| 1024  | Mean                     | 147.21    | 146.84   | us        | 6.97M   |
| 1024  | Median                   | 147.22    | 146.85   | us        | 6.97M   |
| 1024  | Standard Deviation       | 0.12      | 0.11     | us        | 5.45k   |
| 1024  | Coefficient of Variation | 0.08%     | 0.08%    |           | 0.08%   |
| 4096  | Mean                     | 562.03    | 560.31   | us        | 7.31M   |
| 4096  | Median                   | 562       | 560.27   | us        | 7.31M   |
| 4096  | Standard Deviation       | 0.38      | 0.36     | us        | 4.74k   |
| 4096  | Coefficient of Variation | 0.07%     | 0.06%    |           | 0.06%   |
| 16384 | Mean                     | 2262.89   | 2255.36  | us        | 7.26M   |
| 16384 | Median                   | 2262.8    | 2255.19  | us        | 7.27M   |
| 16384 | Standard Deviation       | 1.98      | 1.91     | us        | 6.14k   |
| 16384 | Coefficient of Variation | 0.09%     | 0.08%    |           | 0.08%   |
| 65536 | Mean                     | 9110.59   | 9078.91  | us        | 7.22M   |
| 65536 | Median                   | 9099.48   | 9067.51  | us        | 7.23M   |
| 65536 | Standard Deviation       | 27.06     | 26.66    | us        | 21.17k  |
| 65536 | Coefficient of Variation | 0.3%      | 0.29%    |           | 0.29%   |
|       | Big O                    | 138.96    | 138.48   | N         |         |
|       | RMS                      | 6.23p     | 6.14p    |           |         |

#### BM_VcsrConstructDense
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 31.19     | 31.11    | us        | 64.8M   |
| 64    | Median                   | 31.19     | 31.12    | us        | 64.78M  |
| 64    | Standard Deviation       | 0.03      | 0.03     | us        | 66.64k  |
| 64    | Coefficient of Variation | 0.11%     | 0.1%     |           | 0.1%    |
| 256   | Mean                     | 422.02    | 420.65   | us        | 77.59M  |
| 256   | Median                   | 421.76    | 420.41   | us        | 77.64M  |
| 256   | Standard Deviation       | 0.75      | 0.75     | us        | 138.52k |
| 256   | Coefficient of Variation | 0.18%     | 0.18%    |           | 0.18%   |
| 1024  | Mean                     | 8062.18   | 8031.23  | us        | 65.22M  |
| 1024  | Median                   | 8059.75   | 8029.28  | us        | 65.23M  |
| 1024  | Standard Deviation       | 20.48     | 20.02    | us        | 162.64k |
| 1024  | Coefficient of Variation | 0.25%     | 0.25%    |           | 0.25%   |
| 4096  | Mean                     | 153640    | 152645   | us        | 54.94M  |
| 4096  | Median                   | 153618    | 152629   | us        | 54.95M  |
| 4096  | Standard Deviation       | 78.76     | 80.95    | us        | 29.1k   |
| 4096  | Coefficient of Variation | 0.05%     | 0.05%    |           | 0.05%   |
|       | Big O                    | 9.15      | 9.09     | N^2       |         |
|       | RMS                      | 19.11p    | 18.85p   |           |         |

#### BM_VcsrOutEdgesChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 73.06     | 72.89    | ns        | 878.03M |
| 64    | Median                   | 72.95     | 72.79    | ns        | 879.28M |
| 64    | Standard Deviation       | 0.27      | 0.26     | ns        | 3.12M   |
| 64    | Coefficient of Variation | 0.37%     | 0.36%    |           | 0.36%   |
| 256   | Mean                     | 306.06    | 305.4    | ns        | 838.27M |
| 256   | Median                   | 305.63    | 304.97   | ns        | 839.43M |
| 256   | Standard Deviation       | 1.12      | 1.12     | ns        | 3.05M   |
| 256   | Coefficient of Variation | 0.37%     | 0.37%    |           | 0.36%   |
| 1024  | Mean                     | 1352.59   | 1349.58  | ns        | 758.76M |
| 1024  | Median                   | 1352.57   | 1349.6   | ns        | 758.75M |
| 1024  | Standard Deviation       | 2.8       | 2.81     | ns        | 1.58M   |
| 1024  | Coefficient of Variation | 0.21%     | 0.21%    |           | 0.21%   |
| 4096  | Mean                     | 5366.26   | 5354.43  | ns        | 764.97M |
| 4096  | Median                   | 5366.42   | 5354.24  | ns        | 765M    |
| 4096  | Standard Deviation       | 3.01      | 3.09     | ns        | 440.91k |
| 4096  | Coefficient of Variation | 0.06%     | 0.06%    |           | 0.06%   |
| 16384 | Mean                     | 21367     | 21319.2  | ns        | 768.51M |
| 16384 | Median                   | 21358.3   | 21310.7  | ns        | 768.81M |
| 16384 | Standard Deviation       | 53.24     | 52.77    | ns        | 1.9M    |
| 16384 | Coefficient of Variation | 0.25%     | 0.25%    |           | 0.25%   |
| 65536 | Mean                     | 80245.6   | 79986.3  | ns        | 819.34M |
| 65536 | Median                   | 80187.5   | 79922.1  | ns        | 820M    |
| 65536 | Standard Deviation       | 132.75    | 131.3    | ns        | 1.34M   |
| 65536 | Coefficient of Variation | 0.17%     | 0.16%    |           | 0.16%   |
|       | Big O                    | 1.23      | 1.23     | N         |         |
|       | RMS                      | 29.73p    | 30.2p    |           |         |

#### BM_VcsrInEdgesChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 72.87     | 72.71    | ns        | 880.25M |
| 64    | Median                   | 72.86     | 72.7     | ns        | 880.28M |
| 64    | Standard Deviation       | 0.09      | 0.09     | ns        | 1.05M   |
| 64    | Coefficient of Variation | 0.12%     | 0.12%    |           | 0.12%   |
| 256   | Mean                     | 295.35    | 294.7    | ns        | 868.67M |
| 256   | Median                   | 295.34    | 294.69   | ns        | 868.7M  |
| 256   | Standard Deviation       | 0.29      | 0.29     | ns        | 849.19k |
| 256   | Coefficient of Variation | 0.1%      | 0.1%     |           | 0.1%    |
| 1024  | Mean                     | 1219.31   | 1216.13  | ns        | 842.02M |
| 1024  | Median                   | 1219.21   | 1215.59  | ns        | 842.39M |
| 1024  | Standard Deviation       | 2.69      | 2.3      | ns        | 1.59M   |
| 1024  | Coefficient of Variation | 0.22%     | 0.19%    |           | 0.19%   |
| 4096  | Mean                     | 4806.33   | 4795.55  | ns        | 854.14M |
| 4096  | Median                   | 4799.26   | 4788.27  | ns        | 855.42M |
| 4096  | Standard Deviation       | 18.66     | 18.71    | ns        | 3.32M   |
| 4096  | Coefficient of Variation | 0.39%     | 0.39%    |           | 0.39%   |
| 16384 | Mean                     | 19167.5   | 19121.9  | ns        | 856.82M |
| 16384 | Median                   | 19149.2   | 19105    | ns        | 857.58M |
| 16384 | Standard Deviation       | 41.4      | 40.63    | ns        | 1.82M   |
| 16384 | Coefficient of Variation | 0.22%     | 0.21%    |           | 0.21%   |
| 65536 | Mean                     | 80153.9   | 79883.1  | ns        | 820.41M |
| 65536 | Median                   | 80218.7   | 79948    | ns        | 819.73M |
| 65536 | Standard Deviation       | 273.9     | 274.42   | ns        | 2.83M   |
| 65536 | Coefficient of Variation | 0.34%     | 0.34%    |           | 0.34%   |
|       | Big O                    | 1.22      | 1.22     | N         |         |
|       | RMS                      | 20.99p    | 20.56p   |           |         |

#### BM_VcsrOutEdgesDense
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 573.75    | 572.47   | ns        | 3.52G   |
| 64    | Median                   | 573.38    | 572.08   | ns        | 3.52G   |
| 64    | Standard Deviation       | 3.09      | 3.07     | ns        | 18.9M   |
| 64    | Coefficient of Variation | 0.54%     | 0.54%    |           | 0.54%   |
| 256   | Mean                     | 10038.7   | 10016.7  | ns        | 3.26G   |
| 256   | Median                   | 10018.8   | 9997.21  | ns        | 3.26G   |
| 256   | Standard Deviation       | 56.24     | 55.58    | ns        | 17.85M  |
| 256   | Coefficient of Variation | 0.56%     | 0.55%    |           | 0.55%   |
| 1024  | Mean                     | 156023    | 155521   | ns        | 3.37G   |
| 1024  | Median                   | 156404    | 155903   | ns        | 3.36G   |
| 1024  | Standard Deviation       | 1064.55   | 1061.6   | ns        | 23.2M   |
| 1024  | Coefficient of Variation | 0.68%     | 0.68%    |           | 0.69%   |
| 4096  | Mean                     | 4037820   | 3988120  | ns        | 2.1G    |
| 4096  | Median                   | 4033870   | 3984150  | ns        | 2.1G    |
| 4096  | Standard Deviation       | 8885.43   | 9046.27  | ns        | 4.76M   |
| 4096  | Coefficient of Variation | 0.22%     | 0.23%    |           | 0.23%   |
|       | Big O                    | 58.78µ    | 58.06µ   | N^3       |         |
|       | RMS                      | 44.59p    | 45.27p   |           |         |

#### BM_VcsrIsAcyclicTrue
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 225.6     | 225.1    | ns        | 284.32M |
| 64    | Median                   | 225.56    | 225.07   | ns        | 284.35M |
| 64    | Standard Deviation       | 0.15      | 0.14     | ns        | 174.5k  |
| 64    | Coefficient of Variation | 0.07%     | 0.06%    |           | 0.06%   |
| 256   | Mean                     | 831.95    | 830.18   | ns        | 308.37M |
| 256   | Median                   | 832.51    | 830.74   | ns        | 308.16M |
| 256   | Standard Deviation       | 0.95      | 0.95     | ns        | 351.94k |
| 256   | Coefficient of Variation | 0.11%     | 0.11%    |           | 0.11%   |
| 1024  | Mean                     | 3288.74   | 3281.34  | ns        | 312.07M |
| 1024  | Median                   | 3293.55   | 3286.28  | ns        | 311.6M  |
| 1024  | Standard Deviation       | 13.35     | 13.42    | ns        | 1.28M   |
| 1024  | Coefficient of Variation | 0.41%     | 0.41%    |           | 0.41%   |
| 4096  | Mean                     | 13059.7   | 13030.2  | ns        | 314.37M |
| 4096  | Median                   | 12985     | 12955.2  | ns        | 316.17M |
| 4096  | Standard Deviation       | 120.43    | 120.37   | ns        | 2.89M   |
| 4096  | Coefficient of Variation | 0.92%     | 0.92%    |           | 0.92%   |
| 16384 | Mean                     | 53321.2   | 53149.5  | ns        | 308.26M |
| 16384 | Median                   | 53326.6   | 53160.1  | ns        | 308.2M  |
| 16384 | Standard Deviation       | 147.93    | 150.44   | ns        | 872.29k |
| 16384 | Coefficient of Variation | 0.28%     | 0.28%    |           | 0.28%   |
| 65536 | Mean                     | 212266    | 211562   | ns        | 309.77M |
| 65536 | Median                   | 212198    | 211520   | ns        | 309.83M |
| 65536 | Standard Deviation       | 121.38    | 85.99    | ns        | 125.86k |
| 65536 | Coefficient of Variation | 0.06%     | 0.04%    |           | 0.04%   |
|       | Big O                    | 3.24      | 3.23     | N         |         |
|       | RMS                      | 3.38p     | 3.28p    |           |         |

#### BM_VcsrIsAcyclicFalse
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 57.52     | 57.39    | ns        | 1.12G   |
| 64    | Median                   | 57.49     | 57.37    | ns        | 1.12G   |
| 64    | Standard Deviation       | 0.15      | 0.14     | ns        | 2.8M    |
| 64    | Coefficient of Variation | 0.25%     | 0.25%    |           | 0.25%   |
| 256   | Mean                     | 138.81    | 138.5    | ns        | 1.85G   |
| 256   | Median                   | 138.78    | 138.47   | ns        | 1.85G   |
| 256   | Standard Deviation       | 0.21      | 0.2      | ns        | 2.68M   |
| 256   | Coefficient of Variation | 0.15%     | 0.15%    |           | 0.15%   |
| 1024  | Mean                     | 522.56    | 521.35   | ns        | 1.97G   |
| 1024  | Median                   | 531.05    | 529.85   | ns        | 1.93G   |
| 1024  | Standard Deviation       | 18.53     | 18.48    | ns        | 73.4M   |
| 1024  | Coefficient of Variation | 3.55%     | 3.54%    |           | 3.73%   |
| 4096  | Mean                     | 1843.24   | 1838.99  | ns        | 2.23G   |
| 4096  | Median                   | 1838.5    | 1834.36  | ns        | 2.23G   |
| 4096  | Standard Deviation       | 19.33     | 19.28    | ns        | 23.28M  |
| 4096  | Coefficient of Variation | 1.05%     | 1.05%    |           | 1.04%   |
| 16384 | Mean                     | 7402.31   | 7385.57  | ns        | 2.22G   |
| 16384 | Median                   | 7402.88   | 7386.04  | ns        | 2.22G   |
| 16384 | Standard Deviation       | 16.56     | 16.28    | ns        | 4.88M   |
| 16384 | Coefficient of Variation | 0.22%     | 0.22%    |           | 0.22%   |
| 65536 | Mean                     | 30264.6   | 30167.7  | ns        | 2.17G   |
| 65536 | Median                   | 30268.1   | 30169.5  | ns        | 2.17G   |
| 65536 | Standard Deviation       | 36.34     | 35.2     | ns        | 2.54M   |
| 65536 | Coefficient of Variation | 0.12%     | 0.12%    |           | 0.12%   |
|       | Big O                    | 461.19m   | 459.73m  | N         |         |
|       | RMS                      | 11.09p    | 10.7p    |           |         |

#### BM_VcsrIsAcyclicDense
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 997.45    | 994.76   | ns        | 64.34M  |
| 64    | Median                   | 995.47    | 993.16   | ns        | 64.44M  |
| 64    | Standard Deviation       | 8.03      | 7.47     | ns        | 478.16k |
| 64    | Coefficient of Variation | 0.8%      | 0.75%    |           | 0.74%   |
| 256   | Mean                     | 13094     | 13064.8  | ns        | 19.6M   |
| 256   | Median                   | 13067     | 13038.5  | ns        | 19.63M  |
| 256   | Standard Deviation       | 81.1      | 80.44    | ns        | 119.81k |
| 256   | Coefficient of Variation | 0.62%     | 0.62%    |           | 0.61%   |
| 1024  | Mean                     | 209691    | 209009   | ns        | 4.9M    |
| 1024  | Median                   | 210639    | 209952   | ns        | 4.88M   |
| 1024  | Standard Deviation       | 3801.41   | 3789.39  | ns        | 90.51k  |
| 1024  | Coefficient of Variation | 1.81%     | 1.81%    |           | 1.85%   |
| 4096  | Mean                     | 5483800   | 5420610  | ns        | 755.64k |
| 4096  | Median                   | 5483810   | 5421520  | ns        | 755.51k |
| 4096  | Standard Deviation       | 14536.7   | 14635.5  | ns        | 2.04k   |
| 4096  | Coefficient of Variation | 0.27%     | 0.27%    |           | 0.27%   |
| 8192  | Mean                     | 19042800  | 18825800 | ns        | 435.15k |
| 8192  | Median                   | 19040900  | 18823400 | ns        | 435.2k  |
| 8192  | Standard Deviation       | 38943.8   | 39177    | ns        | 904.83  |
| 8192  | Coefficient of Variation | 0.2%      | 0.21%    |           | 0.21%   |
|       | Big O                    | 286.27m   | 283.01m  | N^2       |         |
|       | RMS                      | 64.01p    | 63.92p   |           |         |

#### BM_VcsrTopoSortChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 329.24    | 328.43   | ns        | 194.87M |
| 64    | Median                   | 329.18    | 328.39   | ns        | 194.89M |
| 64    | Standard Deviation       | 0.18      | 0.12     | ns        | 73.11k  |
| 64    | Coefficient of Variation | 0.05%     | 0.04%    |           | 0.04%   |
| 256   | Mean                     | 1307.53   | 1304.54  | ns        | 196.24M |
| 256   | Median                   | 1307.75   | 1304.78  | ns        | 196.2M  |
| 256   | Standard Deviation       | 1.82      | 1.81     | ns        | 272.37k |
| 256   | Coefficient of Variation | 0.14%     | 0.14%    |           | 0.14%   |
| 1024  | Mean                     | 5157.34   | 5145.26  | ns        | 199.03M |
| 1024  | Median                   | 5130.62   | 5117.85  | ns        | 200.08M |
| 1024  | Standard Deviation       | 46.66     | 46.51    | ns        | 1.79M   |
| 1024  | Coefficient of Variation | 0.9%      | 0.9%     |           | 0.9%    |
| 4096  | Mean                     | 20576.8   | 20530.4  | ns        | 199.51M |
| 4096  | Median                   | 20577.8   | 20531.4  | ns        | 199.5M  |
| 4096  | Standard Deviation       | 97.61     | 97.64    | ns        | 947.77k |
| 4096  | Coefficient of Variation | 0.47%     | 0.48%    |           | 0.48%   |
| 16384 | Mean                     | 84063.8   | 83761.1  | ns        | 195.61M |
| 16384 | Median                   | 84077     | 83745.7  | ns        | 195.64M |
| 16384 | Standard Deviation       | 259.61    | 260.28   | ns        | 607.15k |
| 16384 | Coefficient of Variation | 0.31%     | 0.31%    |           | 0.31%   |
| 65536 | Mean                     | 334027    | 332946   | ns        | 196.84M |
| 65536 | Median                   | 333971    | 332884   | ns        | 196.87M |
| 65536 | Standard Deviation       | 234.42    | 223.97   | ns        | 132.37k |
| 65536 | Coefficient of Variation | 0.07%     | 0.07%    |           | 0.07%   |
|       | Big O                    | 5.1       | 5.08     | N         |         |
|       | RMS                      | 3.93p     | 3.75p    |           |         |

#### BM_VcsrTopoSortBinaryTree
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 273.95    | 273.33   | ns        | 234.15M |
| 64    | Median                   | 273.96    | 273.33   | ns        | 234.15M |
| 64    | Standard Deviation       | 0.2       | 0.19     | ns        | 164.67k |
| 64    | Coefficient of Variation | 0.07%     | 0.07%    |           | 0.07%   |
| 256   | Mean                     | 1118.33   | 1115.84  | ns        | 229.52M |
| 256   | Median                   | 1102.13   | 1099.66  | ns        | 232.8M  |
| 256   | Standard Deviation       | 24.42     | 24.36    | ns        | 4.97M   |
| 256   | Coefficient of Variation | 2.18%     | 2.18%    |           | 2.17%   |
| 1024  | Mean                     | 4383.83   | 4373.35  | ns        | 234.15M |
| 1024  | Median                   | 4385.48   | 4375.28  | ns        | 234.04M |
| 1024  | Standard Deviation       | 19.76     | 18.67    | ns        | 994.33k |
| 1024  | Coefficient of Variation | 0.45%     | 0.43%    |           | 0.42%   |
| 4096  | Mean                     | 18231.7   | 18189.1  | ns        | 225.3M  |
| 4096  | Median                   | 18175     | 18132.2  | ns        | 225.92M |
| 4096  | Standard Deviation       | 419.77    | 419.25   | ns        | 5.17M   |
| 4096  | Coefficient of Variation | 2.3%      | 2.3%     |           | 2.29%   |
| 16384 | Mean                     | 71849     | 71624.2  | ns        | 228.75M |
| 16384 | Median                   | 71794.6   | 71569.3  | ns        | 228.93M |
| 16384 | Standard Deviation       | 304.62    | 300.94   | ns        | 959.98k |
| 16384 | Coefficient of Variation | 0.42%     | 0.42%    |           | 0.42%   |
| 65536 | Mean                     | 292841    | 291888   | ns        | 224.53M |
| 65536 | Median                   | 292789    | 291834   | ns        | 224.57M |
| 65536 | Standard Deviation       | 635.37    | 630.89   | ns        | 484.69k |
| 65536 | Coefficient of Variation | 0.22%     | 0.22%    |           | 0.22%   |
|       | Big O                    | 4.46      | 4.45     | N         |         |
|       | RMS                      | 9.74p     | 9.68p    |           |         |

#### BM_VcsrTopoSortDense
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 2.14      | 2.13     | us        | 30.03M  |
| 64    | Median                   | 2.13      | 2.13     | us        | 30.08M  |
| 64    | Standard Deviation       | 0.01      | 0.01     | us        | 79.81k  |
| 64    | Coefficient of Variation | 0.27%     | 0.27%    |           | 0.27%   |
| 256   | Mean                     | 32.44     | 32.36    | us        | 7.91M   |
| 256   | Median                   | 32.54     | 32.47    | us        | 7.88M   |
| 256   | Standard Deviation       | 0.21      | 0.21     | us        | 51.5k   |
| 256   | Coefficient of Variation | 0.65%     | 0.65%    |           | 0.65%   |
| 1024  | Mean                     | 508.33    | 506.66   | us        | 2.02M   |
| 1024  | Median                   | 507.04    | 505.32   | us        | 2.03M   |
| 1024  | Standard Deviation       | 4.52      | 4.51     | us        | 17.94k  |
| 1024  | Coefficient of Variation | 0.89%     | 0.89%    |           | 0.89%   |
| 4096  | Mean                     | 11972.9   | 11843.3  | us        | 345.86k |
| 4096  | Median                   | 11979.2   | 11849.1  | us        | 345.68k |
| 4096  | Standard Deviation       | 51.2      | 47.55    | us        | 1.39k   |
| 4096  | Coefficient of Variation | 0.43%     | 0.4%     |           | 0.4%    |
| 8192  | Mean                     | 43289.6   | 42819.7  | us        | 191.31k |
| 8192  | Median                   | 43265.6   | 42790    | us        | 191.45k |
| 8192  | Standard Deviation       | 57.5      | 59.92    | us        | 267.09  |
| 8192  | Coefficient of Variation | 0.13%     | 0.14%    |           | 0.14%   |
|       | Big O                    | 649.06m   | 642.02m  | N^2       |         |
|       | RMS                      | 45.35p    | 45.34p   |           |         |

#### BM_VcsrNodeAccessSequential
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 14.3      | 14.27    | ns        | 4.49G   |
| 64    | Median                   | 14.29     | 14.26    | ns        | 4.49G   |
| 64    | Standard Deviation       | 0.03      | 0.03     | ns        | 9.02M   |
| 64    | Coefficient of Variation | 0.23%     | 0.2%     |           | 0.2%    |
| 256   | Mean                     | 55.18     | 55.05    | ns        | 4.65G   |
| 256   | Median                   | 55.16     | 55.04    | ns        | 4.65G   |
| 256   | Standard Deviation       | 0.06      | 0.03     | ns        | 2.63M   |
| 256   | Coefficient of Variation | 0.12%     | 0.06%    |           | 0.06%   |
| 1024  | Mean                     | 440.72    | 439.77   | ns        | 2.33G   |
| 1024  | Median                   | 440.6     | 439.64   | ns        | 2.33G   |
| 1024  | Standard Deviation       | 0.36      | 0.37     | ns        | 1.95M   |
| 1024  | Coefficient of Variation | 0.08%     | 0.08%    |           | 0.08%   |
| 4096  | Mean                     | 1765.7    | 1761.89  | ns        | 2.32G   |
| 4096  | Median                   | 1766.37   | 1762.58  | ns        | 2.32G   |
| 4096  | Standard Deviation       | 1.7       | 1.63     | ns        | 2.16M   |
| 4096  | Coefficient of Variation | 0.1%      | 0.09%    |           | 0.09%   |
| 16384 | Mean                     | 7180.4    | 7158.33  | ns        | 2.29G   |
| 16384 | Median                   | 7176.44   | 7154.49  | ns        | 2.29G   |
| 16384 | Standard Deviation       | 10.52     | 10.31    | ns        | 3.29M   |
| 16384 | Coefficient of Variation | 0.15%     | 0.14%    |           | 0.14%   |
| 65536 | Mean                     | 37327.5   | 37207.9  | ns        | 1.76G   |
| 65536 | Median                   | 37316.3   | 37197.5  | ns        | 1.76G   |
| 65536 | Standard Deviation       | 147.79    | 145.88   | ns        | 6.86M   |
| 65536 | Coefficient of Variation | 0.4%      | 0.39%    |           | 0.39%   |
|       | Big O                    | 35.4m     | 35.29m   | NlgN      |         |
|       | RMS                      | 51.11p    | 51.05p   |           |         |

#### BM_VcsrNodeAccessRandom
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 20.83     | 20.79    | ns        | 3.08G   |
| 64    | Median                   | 20.86     | 20.81    | ns        | 3.08G   |
| 64    | Standard Deviation       | 0.12      | 0.12     | ns        | 17.66M  |
| 64    | Coefficient of Variation | 0.57%     | 0.57%    |           | 0.57%   |
| 256   | Mean                     | 83.65     | 83.47    | ns        | 3.07G   |
| 256   | Median                   | 83.64     | 83.47    | ns        | 3.07G   |
| 256   | Standard Deviation       | 0.08      | 0.08     | ns        | 2.96M   |
| 256   | Coefficient of Variation | 0.1%      | 0.1%     |           | 0.1%    |
| 1024  | Mean                     | 519.98    | 518.87   | ns        | 1.97G   |
| 1024  | Median                   | 519.78    | 518.68   | ns        | 1.97G   |
| 1024  | Standard Deviation       | 0.98      | 0.97     | ns        | 3.69M   |
| 1024  | Coefficient of Variation | 0.19%     | 0.19%    |           | 0.19%   |
| 4096  | Mean                     | 2081.29   | 2076.01  | ns        | 1.97G   |
| 4096  | Median                   | 2081.92   | 2077.35  | ns        | 1.97G   |
| 4096  | Standard Deviation       | 7.73      | 8.16     | ns        | 7.75M   |
| 4096  | Coefficient of Variation | 0.37%     | 0.39%    |           | 0.39%   |
| 16384 | Mean                     | 8997.63   | 8969.75  | ns        | 1.83G   |
| 16384 | Median                   | 8996.7    | 8969.01  | ns        | 1.83G   |
| 16384 | Standard Deviation       | 55.53     | 55.18    | ns        | 11.18M  |
| 16384 | Coefficient of Variation | 0.62%     | 0.62%    |           | 0.61%   |
| 65536 | Mean                     | 52197.8   | 52029.8  | ns        | 1.26G   |
| 65536 | Median                   | 52144.2   | 51982.1  | ns        | 1.26G   |
| 65536 | Standard Deviation       | 142.69    | 142.96   | ns        | 3.45M   |
| 65536 | Coefficient of Variation | 0.27%     | 0.27%    |           | 0.27%   |
|       | Big O                    | 49.28m    | 49.12m   | NlgN      |         |
|       | RMS                      | 91.76p    | 91.71p   |           |         |

#### BM_VcsrDynamicAddNode
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 11.68     | 11.67    | us        | 5.48M   |
| 64    | Median                   | 11.69     | 11.67    | us        | 5.48M   |
| 64    | Standard Deviation       | 0.02      | 0.02     | us        | 8.38k   |
| 64    | Coefficient of Variation | 0.15%     | 0.15%    |           | 0.15%   |
| 256   | Mean                     | 44.75     | 44.64    | us        | 5.73M   |
| 256   | Median                   | 44.71     | 44.61    | us        | 5.74M   |
| 256   | Standard Deviation       | 0.12      | 0.08     | us        | 9.99k   |
| 256   | Coefficient of Variation | 0.26%     | 0.17%    |           | 0.17%   |
| 1024  | Mean                     | 175.47    | 175.03   | us        | 5.85M   |
| 1024  | Median                   | 175.67    | 175.25   | us        | 5.84M   |
| 1024  | Standard Deviation       | 0.54      | 0.53     | us        | 17.89k  |
| 1024  | Coefficient of Variation | 0.31%     | 0.31%    |           | 0.31%   |
| 4096  | Mean                     | 683.94    | 681.77   | us        | 6.01M   |
| 4096  | Median                   | 684.48    | 682.35   | us        | 6M      |
| 4096  | Standard Deviation       | 1.57      | 1.59     | us        | 14.03k  |
| 4096  | Coefficient of Variation | 0.23%     | 0.23%    |           | 0.23%   |
| 16384 | Mean                     | 2741.6    | 2732.7   | us        | 6M      |
| 16384 | Median                   | 2741.58   | 2732.69  | us        | 6M      |
| 16384 | Standard Deviation       | 4.28      | 4.18     | us        | 9.17k   |
| 16384 | Coefficient of Variation | 0.16%     | 0.15%    |           | 0.15%   |
| 65536 | Mean                     | 11989.1   | 11942.3  | us        | 5.49M   |
| 65536 | Median                   | 11983.8   | 11937.2  | us        | 5.49M   |
| 65536 | Standard Deviation       | 11.61     | 10.8     | us        | 4.96k   |
| 65536 | Coefficient of Variation | 0.1%      | 0.09%    |           | 0.09%   |
|       | Big O                    | 11.46     | 11.42    | NlgN      |         |
|       | RMS                      | 28.04p    | 28.28p   |           |         |

#### BM_VcsrDijkstraChain
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 487.64    | 486.56   | ns        | 131.54M |
| 64    | Median                   | 487.36    | 486.28   | ns        | 131.61M |
| 64    | Standard Deviation       | 0.64      | 0.64     | ns        | 172.75k |
| 64    | Coefficient of Variation | 0.13%     | 0.13%    |           | 0.13%   |
| 256   | Mean                     | 1968.91   | 1964.44  | ns        | 130.32M |
| 256   | Median                   | 1969.1    | 1964.67  | ns        | 130.3M  |
| 256   | Standard Deviation       | 9.76      | 9.74     | ns        | 646.18k |
| 256   | Coefficient of Variation | 0.5%      | 0.5%     |           | 0.5%    |
| 1024  | Mean                     | 7848.29   | 7830.45  | ns        | 130.77M |
| 1024  | Median                   | 7839.05   | 7821.62  | ns        | 130.92M |
| 1024  | Standard Deviation       | 37.41     | 37.38    | ns        | 621.41k |
| 1024  | Coefficient of Variation | 0.48%     | 0.48%    |           | 0.48%   |
| 4096  | Mean                     | 32104.6   | 32021.1  | ns        | 127.93M |
| 4096  | Median                   | 32339     | 32268    | ns        | 126.94M |
| 4096  | Standard Deviation       | 411.24    | 399.71   | ns        | 1.61M   |
| 4096  | Coefficient of Variation | 1.28%     | 1.25%    |           | 1.26%   |
| 16384 | Mean                     | 127424    | 127023   | ns        | 128.98M |
| 16384 | Median                   | 127411    | 127009   | ns        | 129M    |
| 16384 | Standard Deviation       | 67.56     | 62.19    | ns        | 63.14k  |
| 16384 | Coefficient of Variation | 0.05%     | 0.05%    |           | 0.05%   |
| 65536 | Mean                     | 508433    | 506813   | ns        | 129.31M |
| 65536 | Median                   | 508133    | 506526   | ns        | 129.38M |
| 65536 | Standard Deviation       | 1554.37   | 1533.36  | ns        | 389.53k |
| 65536 | Coefficient of Variation | 0.31%     | 0.3%     |           | 0.3%    |
|       | Big O                    | 7.76      | 7.73     | N         |         |
|       | RMS                      | 5.76p     | 5.71p    |           |         |

#### BM_VcsrDijkstraBinaryTree
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 740.14    | 738.39   | ns        | 86.68M  |
| 64    | Median                   | 738.31    | 736.3    | ns        | 86.92M  |
| 64    | Standard Deviation       | 3.95      | 3.94     | ns        | 459.78k |
| 64    | Coefficient of Variation | 0.53%     | 0.53%    |           | 0.53%   |
| 256   | Mean                     | 3726.29   | 3715.44  | ns        | 69.08M  |
| 256   | Median                   | 3661.22   | 3648.37  | ns        | 70.17M  |
| 256   | Standard Deviation       | 209.76    | 210.07   | ns        | 3.42M   |
| 256   | Coefficient of Variation | 5.63%     | 5.65%    |           | 4.95%   |
| 1024  | Mean                     | 18004.5   | 17959.9  | ns        | 57.02M  |
| 1024  | Median                   | 18012.2   | 17969.6  | ns        | 56.99M  |
| 1024  | Standard Deviation       | 31.42     | 33.69    | ns        | 106.97k |
| 1024  | Coefficient of Variation | 0.17%     | 0.19%    |           | 0.19%   |
| 4096  | Mean                     | 90340.6   | 90132.6  | ns        | 45.44M  |
| 4096  | Median                   | 90368.1   | 90153.4  | ns        | 45.43M  |
| 4096  | Standard Deviation       | 238.99    | 243.02   | ns        | 122.56k |
| 4096  | Coefficient of Variation | 0.26%     | 0.27%    |           | 0.27%   |
| 16384 | Mean                     | 629934    | 628285   | ns        | 26.08M  |
| 16384 | Median                   | 630236    | 628540   | ns        | 26.07M  |
| 16384 | Standard Deviation       | 1122.23   | 1100.12  | ns        | 45.71k  |
| 16384 | Coefficient of Variation | 0.18%     | 0.18%    |           | 0.18%   |
| 65536 | Mean                     | 3269100   | 3258210  | ns        | 20.11M  |
| 65536 | Median                   | 3270230   | 3258300  | ns        | 20.11M  |
| 65536 | Standard Deviation       | 6621.5    | 6397.41  | ns        | 39.51k  |
| 65536 | Coefficient of Variation | 0.2%      | 0.2%     |           | 0.2%    |
|       | Big O                    | 3.1       | 3.09     | NlgN      |         |
|       | RMS                      | 64.05p    | 63.79p   |           |         |

#### BM_VcsrDijkstraDense
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 64    | Mean                     | 3.27      | 3.26     | us        | 19.63M  |
| 64    | Median                   | 3.26      | 3.25     | us        | 19.68M  |
| 64    | Standard Deviation       | 0.05      | 0.05     | us        | 267.37k |
| 64    | Coefficient of Variation | 1.4%      | 1.4%     |           | 1.36%   |
| 128   | Mean                     | 10.66     | 10.63    | us        | 12.04M  |
| 128   | Median                   | 10.64     | 10.61    | us        | 12.06M  |
| 128   | Standard Deviation       | 0.04      | 0.04     | us        | 48.98k  |
| 128   | Coefficient of Variation | 0.42%     | 0.41%    |           | 0.41%   |
| 256   | Mean                     | 33.63     | 33.53    | us        | 7.64M   |
| 256   | Median                   | 33.28     | 33.2     | us        | 7.71M   |
| 256   | Standard Deviation       | 0.6       | 0.59     | us        | 131.8k  |
| 256   | Coefficient of Variation | 1.78%     | 1.76%    |           | 1.73%   |
| 512   | Mean                     | 130.21    | 129.73   | us        | 3.95M   |
| 512   | Median                   | 131.85    | 131.42   | us        | 3.9M    |
| 512   | Standard Deviation       | 3.05      | 2.99     | us        | 92.61k  |
| 512   | Coefficient of Variation | 2.34%     | 2.31%    |           | 2.35%   |
| 1024  | Mean                     | 438.28    | 435.85   | us        | 2.35M   |
| 1024  | Median                   | 438.83    | 436.27   | us        | 2.35M   |
| 1024  | Standard Deviation       | 5.25      | 5.1      | us        | 27.51k  |
| 1024  | Coefficient of Variation | 1.2%      | 1.17%    |           | 1.17%   |
| 2048  | Mean                     | 2493.21   | 2474.32  | us        | 827.72k |
| 2048  | Median                   | 2493.28   | 2473.98  | us        | 827.82k |
| 2048  | Standard Deviation       | 11.44     | 11.55    | us        | 3.87k   |
| 2048  | Coefficient of Variation | 0.46%     | 0.47%    |           | 0.47%   |
| 4096  | Mean                     | 9146.67   | 9035.93  | us        | 453.3k  |
| 4096  | Median                   | 9150.74   | 9038.04  | us        | 453.19k |
| 4096  | Standard Deviation       | 19.91     | 20.77    | us        | 1.04k   |
| 4096  | Coefficient of Variation | 0.22%     | 0.23%    |           | 0.23%   |
|       | Big O                    | 547.59m   | 541.13m  | N^2       |         |
|       | RMS                      | 52.61p    | 54.21p   |           |         |

#### BM_VcsrDijkstraSparse
| Param | Metric                   | Real time | CPU time | Time unit | items/s |
|-------|--------------------------|-----------|----------|-----------|---------|
| 1024  | Mean                     | 24.1      | 24.04    | us        | 42.6M   |
| 1024  | Median                   | 24.01     | 23.94    | us        | 42.77M  |
| 1024  | Standard Deviation       | 0.32      | 0.32     | us        | 547.44k |
| 1024  | Coefficient of Variation | 1.32%     | 1.33%    |           | 1.29%   |
| 4096  | Mean                     | 100.35    | 100.04   | us        | 40.96M  |
| 4096  | Median                   | 99.18     | 98.88    | us        | 41.43M  |
| 4096  | Standard Deviation       | 2.17      | 2.16     | us        | 867.47k |
| 4096  | Coefficient of Variation | 2.16%     | 2.16%    |           | 2.12%   |
| 16384 | Mean                     | 801.15    | 798.49   | us        | 20.52M  |
| 16384 | Median                   | 801.68    | 798.96   | us        | 20.51M  |
| 16384 | Standard Deviation       | 2.36      | 2.34     | us        | 60.23k  |
| 16384 | Coefficient of Variation | 0.29%     | 0.29%    |           | 0.29%   |
| 65536 | Mean                     | 4546.15   | 4528.89  | us        | 14.48M  |
| 65536 | Median                   | 4517.09   | 4500.21  | us        | 14.56M  |
| 65536 | Standard Deviation       | 108.77    | 108.41   | us        | 342.75k |
| 65536 | Coefficient of Variation | 2.39%     | 2.39%    |           | 2.37%   |
|       | Big O                    | 4.29      | 4.28     | NlgN      |         |
|       | RMS                      | 88.77p    | 88.65p   |           |         |

#### BM_VcsrDijkstraSparse_PointToPoint
| Param   | Metric                   | Real time | CPU time | Time unit | items/s |
|---------|--------------------------|-----------|----------|-----------|---------|
| 256     | Mean                     | 492.27    | 490.91   | ns        | 521.5M  |
| 256     | Median                   | 490.72    | 489.58   | ns        | 522.89M |
| 256     | Standard Deviation       | 3.26      | 3.25     | ns        | 3.42M   |
| 256     | Coefficient of Variation | 0.66%     | 0.66%    |           | 0.66%   |
| 1024    | Mean                     | 1887.31   | 1883.02  | ns        | 543.81M |
| 1024    | Median                   | 1887.4    | 1882.99  | ns        | 543.82M |
| 1024    | Standard Deviation       | 1.48      | 1.45     | ns        | 419.79k |
| 1024    | Coefficient of Variation | 0.08%     | 0.08%    |           | 0.08%   |
| 4096    | Mean                     | 8731.88   | 8711.58  | ns        | 470.2M  |
| 4096    | Median                   | 8725.34   | 8705.08  | ns        | 470.53M |
| 4096    | Standard Deviation       | 55.96     | 55.72    | ns        | 3.01M   |
| 4096    | Coefficient of Variation | 0.64%     | 0.64%    |           | 0.64%   |
| 16384   | Mean                     | 32997.5   | 32919.3  | ns        | 497.71M |
| 16384   | Median                   | 33058.1   | 32981    | ns        | 496.77M |
| 16384   | Standard Deviation       | 161.85    | 160.66   | ns        | 2.43M   |
| 16384   | Coefficient of Variation | 0.49%     | 0.49%    |           | 0.49%   |
| 65536   | Mean                     | 149569    | 149111   | ns        | 439.53M |
| 65536   | Median                   | 149254    | 148808   | ns        | 440.41M |
| 65536   | Standard Deviation       | 935.03    | 925.4    | ns        | 2.69M   |
| 65536   | Coefficient of Variation | 0.63%     | 0.62%    |           | 0.61%   |
| 262144  | Mean                     | 1040460   | 1036980  | ns        | 252.8M  |
| 262144  | Median                   | 1040350   | 1036880  | ns        | 252.82M |
| 262144  | Standard Deviation       | 1483.74   | 1477.93  | ns        | 360.31k |
| 262144  | Coefficient of Variation | 0.14%     | 0.14%    |           | 0.14%   |
| 1048576 | Mean                     | 8529350   | 8497740  | ns        | 123.4M  |
| 1048576 | Median                   | 8505050   | 8473730  | ns        | 123.74M |
| 1048576 | Standard Deviation       | 60874.4   | 60488    | ns        | 874.54k |
| 1048576 | Coefficient of Variation | 0.71%     | 0.71%    |           | 0.71%   |
|         | Big O                    | 7.79µ     | 7.76µ    | N^2       |         |
|         | RMS                      | 141.92p   | 142.01p  |           |         |
