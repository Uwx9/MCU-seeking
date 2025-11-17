#ifndef __W25Q64_H__
#define __W25Q64_H__

void W25Q64_init();
void W25Q64_read_id(uint8_t* MID, uint16_t* DID);
void W25Q64_page_program(uint32_t addr, uint8_t* data_arry, uint16_t count);
void W25Q64_sector_erase(uint32_t addr);
void W25Q64_read_data(uint32_t addr, uint8_t* data_arry, uint32_t count);
#endif