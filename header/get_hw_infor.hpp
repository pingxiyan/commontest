#pragma once
#include <vector>

typedef struct tagCPUInfo
{
    float freq = -1;     // GHz
    int thrdNum = -1;
    int memTotal = -1;   // kByte
    int memUsed = -1;    // kByte
    float util = -1;     // 100%
}TCPUInfo;

typedef struct tagNVGPUInfo
{
    int memTotal = -1;   // MB
    int memUsed = -1;    // MB
    float util = -1;     // 100%
}TNVGPUInfo;

bool get_cpu_infor(TCPUInfo& cpuInfor);
bool get_nv_gpu_infor(std::vector<TNVGPUInfo>& gpuInfor);