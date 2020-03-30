#include "get_hw_infor.hpp"
#include <stdio.h>
#include <mutex>

static std::mutex g_get_hw_mutex;

inline int get_int_val() {
    int val = -1;
    FILE* pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[64] = {0};
        if(fgets(buf, 64, pf) > 0) {
            val = std::atoi(buf);
        }
        fclose(pf);
    }
    return val;
}

inline float get_float_val() {
    float val = -1;
    FILE* pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[64] = {0};
        if(fgets(buf, 64, pf) > 0) {
            val = std::atof(buf);
        }
        fclose(pf);
    }
    return val;
}

inline int get_num() {
    int num = 0;
    FILE* pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[128] = {0};
        for(;;) {
            if(fgets(buf, 128, pf) > 0) {
                num++;
            }
            else {
                break;
            }
        }        
        fclose(pf);
    }
    return num;
}

bool get_cpu_infor(TCPUInfo& cpuInfor) {
    std::lock_guard<std::mutex> lk(g_get_hw_mutex);

    std::system("top -n1 > tmp_c.txt");

    std::system("cat tmp_c.txt | grep  'id,' | awk '{print $8}' > tmp.txt");
    float cpuUtil = get_float_val();
    if(cpuUtil >= 0) {
        cpuInfor.util = 100 - cpuUtil;
    }

    std::system("cat tmp_c.txt | grep  'KiB Mem' | awk '{print $4}' > tmp.txt");
    cpuInfor.memTotal = get_int_val();
    
    std::system("cat tmp_c.txt | grep  'KiB Mem' | awk '{print $8}' > tmp.txt");
    cpuInfor.memUsed = get_int_val();

    std::system("cat /proc/cpuinfo | grep 'processor' | awk '{print $3}' > tmp.txt");
    cpuInfor.thrdNum = get_num();

    float freq = 0;
    std::system("cat /proc/cpuinfo | grep 'model name' | awk '{print $9}' | sed 's/GHz//'  > tmp.txt");
    cpuInfor.freq = get_float_val();
    return true;
}

bool get_nv_gpu_infor(std::vector<TNVGPUInfo>& gpuInfor) {
    std::lock_guard<std::mutex> lk(g_get_hw_mutex);

    std::system("nvidia-smi > tmp_nv.txt");

    std::system("cat tmp_nv.txt | grep  'MiB /' | awk '{print $11}' | sed 's/MiB//' > tmp.txt");
    int total = get_num();

    std::system("cat tmp_nv.txt | grep  'MiB /' | awk '{print $9}' | sed 's/MiB//' > tmp.txt");
    FILE* pf = fopen("tmp.txt", "rb");
    if(pf) {
        for(int i = 0; i < total; i++) {
            TNVGPUInfo ginfo;
            char buf[64] = {0};
            if(fgets(buf, 64, pf) > 0) {
                ginfo.memUsed = std::atoi(buf);
            }
            gpuInfor.push_back(ginfo);
        }
        fclose(pf);
    }

    std::system("cat tmp_nv.txt | grep  'MiB /' | awk '{print $11}' | sed 's/MiB//' > tmp.txt");
    pf = fopen("tmp.txt", "rb");
    if(pf) {
        for(int i = 0; i < total; i++) {
            char buf[64] = {0};
            if(fgets(buf, 64, pf) > 0) {
                gpuInfor[i].memTotal = std::atoi(buf);
            }
        }
        fclose(pf);
    }

    std::system("cat tmp_nv.txt | grep  'MiB /' | awk '{print $13}' | sed 's/%//' > tmp.txt");
    pf = fopen("tmp.txt", "rb");
    if(pf) {
        for(int i = 0; i < total; i++) {
            char buf[64] = {0};
            if(fgets(buf, 64, pf) > 0) {
                gpuInfor[i].util = std::atoi(buf);
            }
        }
        fclose(pf);
    }
    return true;
}