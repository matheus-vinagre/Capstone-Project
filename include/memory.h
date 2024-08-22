//
// Created by Vinagre on 8/18/24.
//

#ifndef MEMORY_H
#define MEMORY_H

class Memory {
    public:
        //setters
        void set_mem_total(unsigned long long mem_total);
        void set_mem_free(unsigned long long mem_free);
        void set_mem_buffer(unsigned long long mem_buffer);
        void set_mem_cached(unsigned long long mem_cached);
        void set_s_hmem(unsigned long long s_hmem);
        void set_s_reclaimable(unsigned long long s_reclaimable);

        //getter
        float & mem_percent_util();

        //Computes the system memory utilization
        void MemoryUtilization();

    private:
        unsigned long long _memTotal=0, _memFree=0, _memBuffer=0, _memCached=0, _sHmem=0, _sReclaimable=0 ;
        unsigned long long _memUsed=0, _memCached_T=0, _memUtilization=0;
        float _utilization=0, _memPercentUtil=0;


};


#endif //MEMORY_H
