#include "get_cpu_gpu_util.hpp"
#include <iostream>
#include <mutex>

void get_cpu_uitl(float& util, float& mem_size) {
    static std::mutex g_mutex;
    std::lock_guard<std::mutex> lk(g_mutex);

    std::system("top -n1 > tmp_c.txt");
    float cpuidle = 0;
    std::system("cat tmp_c.txt | grep  'id,' | awk '{print $8}' > tmp.txt");
    FILE* pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[128] = {0};
        if(fgets(buf, 128, pf) > 0) {
            cpuidle = std::atof(buf);
            std::cout << "cpuidle = " << cpuidle << std::endl;
        }
        fclose(pf);
    }

    int totalMemSize = 0;
    std::system("cat tmp_c.txt | grep  'KiB Mem' | awk '{print $4}' > tmp.txt");
    pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[128] = {0};
        if(fgets(buf, 128, pf) > 0) {
            totalMemSize = std::atoi(buf);
            std::cout << "totalMemSize = " << totalMemSize << std::endl;
        }
        fclose(pf);
    }

    int freeMemSize = 0;
    std::system("cat tmp_c.txt | grep  'KiB Mem' | awk '{print $6}' > tmp.txt");
    pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[128] = {0};
        if(fgets(buf, 128, pf) > 0) {
            freeMemSize = std::atoi(buf);
            std::cout << "freeMemSize = " << freeMemSize << std::endl;
        }
        fclose(pf);
    }

    int thrdNum = 0;
    std::system("cat /proc/cpuinfo | grep 'processor' | awk '{print $3}' > tmp.txt");
    pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[128] = {0};
        for(;;) {
            if(fgets(buf, 128, pf) > 0) {
                thrdNum++;
            }
            else {
                break;
            }
        }
        std::cout << "thrdNum = " << thrdNum << std::endl;
        
        fclose(pf);
    }

    float freq = 0;
    std::system("cat /proc/cpuinfo | grep 'model name' | awk '{print $9}' | sed 's/GHz//'  > tmp.txt");
    pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[128] = {0};
        if(fgets(buf, 128, pf) > 0) {
            freq = std::atof(buf);
            std::cout << "freq = " << freq << std::endl;
        }
        fclose(pf);
    }
}

void get_nvidia_gpu_uitl(std::vector<std::tuple<int, float>>& gpuInfor) {
    static std::mutex g_mutex;
    std::lock_guard<std::mutex> lk(g_mutex);

    int usedMemSize = 0;
    int totalMemSize = 0;
    float gpu_util = 0;
    std::system("nvidia-smi > tmp_nv.txt");
    std::system("cat tmp_nv.txt | grep  'MiB /' | awk '{print $9}' | sed 's/MiB//' > tmp.txt");
    FILE* pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[128] = {0};
        if(fgets(buf, 128, pf) > 0) {
            usedMemSize = std::atoi(buf);
            std::cout << "memsize = " << usedMemSize << std::endl;
        }
        fclose(pf);
    }

    std::system("cat tmp_nv.txt | grep  'MiB /' | awk '{print $11}' | sed 's/MiB//' > tmp.txt");
    pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[128] = {0};
        if(fgets(buf, 128, pf) > 0) {
            totalMemSize = std::atoi(buf);
            std::cout << "totalMemSize = " << totalMemSize << std::endl;
        }
        fclose(pf);
    }

    std::system("cat tmp_nv.txt | grep  'MiB /' | awk '{print $13}' | sed 's/%//' > tmp.txt");
    pf = fopen("tmp.txt", "rb");
    if(pf) {
        char buf[128] = {0};
        if(fgets(buf, 128, pf) > 0) {
            gpu_util = std::atof(buf);
            std::cout << "gpu_util = " << gpu_util << std::endl;
        }
        fclose(pf);
    }
}