#pragma once

#include <vector>
#include <tuple>

/**
 * Get CUP or GPU util.
 */

/**
 * @Brief Parse cpu infor, and gpu infor. refer code.
 */
void get_cpu_uitl(float& util, float& mem_size);
void get_nvidia_gpu_uitl(std::vector<std::tuple<int, float>>& gpuInfor);