//
// Created by Vinagre on 8/18/24.
//

#ifndef MEMORY_H
#define MEMORY_H

class Memory {
    public:
        void set_mem_total(unsigned long long mem_total);
        void set_mem_free(unsigned long long mem_free);
        void set_mem_buffer(unsigned long long mem_buffer);
        void set_mem_cached(unsigned long long mem_cached);
        void set_s_hmem(unsigned long long s_hmem);
        void set_s_reclaimable(unsigned long long s_reclaimable);
        float & mem_percent_util();
        void MemoryUtilization();
    private:
        unsigned long long _memTotal, _memFree, _memBuffer, _memCached, _sHmem, _sReclaimable ;
        unsigned long long _memUsed, _memCached_T, _memUtilization;
        float _utilization, _memPercentUtil;


};


#endif //MEMORY_H
