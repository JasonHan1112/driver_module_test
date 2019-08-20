#ifndef __LIBMEM_H__
#define __LIBMEM_H__


#define DEV_MEM "/dev/mem_map"

#define nd_print printf
#define dg_print printf

uint16_t read_dram_raw(uint64_t dram_addr, uint32_t *value, uint32_t size);
uint16_t read_dram_map_raw(uint64_t dram_addr, uint32_t *value, uint32_t size);
uint16_t write_dram_raw(uint64_t dram_addr, uint32_t value, uint32_t size);
uint16_t write_dram_map_raw(uint64_t dram_addr, uint32_t value, uint32_t size);
#endif
