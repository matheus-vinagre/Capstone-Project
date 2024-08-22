//
// Created by Vinagre on 8/18/24.
//
#include <memory.h>

void Memory::MemoryUtilization() { //Computes the system memory utilization
    _memUsed = _memTotal - _memFree;
    _memCached_T = _memCached + _sReclaimable - _sHmem;
    _memUtilization = _memUsed - (_memBuffer + _memCached_T);
    _utilization = (100.0 * _memUtilization) /_memTotal;
    _memPercentUtil = _utilization/100;
}

float & Memory::mem_percent_util() {
    return _memPercentUtil;
}

void Memory::set_mem_total(unsigned long long mem_total) {
    _memTotal = mem_total;
}

void Memory::set_mem_free(unsigned long long mem_free) {
    _memFree = mem_free;
}

void Memory::set_mem_buffer(unsigned long long mem_buffer) {
    _memBuffer = mem_buffer;
}

void Memory::set_mem_cached(unsigned long long mem_cached) {
    _memCached = mem_cached;
}

void Memory::set_s_hmem(unsigned long long s_hmem) {
    _sHmem = s_hmem;
}

void Memory::set_s_reclaimable(unsigned long long s_reclaimable) {
    _sReclaimable = s_reclaimable;
}
