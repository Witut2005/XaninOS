

#include <fat/fat_driver.h>

void fat_create_file(uint16_t cluster_number)
{
    file_allocation_table[cluster_number] = 0x01;
    file_allocation_table[cluster_number+1] = 0xF8;
}

void fat_read_cluster(uint16_t cluster_number)
{    
    while(file_allocation_table[cluster_number] != 0xF8)
    {
        if(file_allocation_table[cluster_number] == 0x01)
        {
            char* data_pointer = (char*)(cluster_number * CLUSTER_SIZE); 
            
            xprintf("%c", *data_pointer);
            data_pointer++;
            
            cluster_number++;
        }
        xprintf("%d\n", cluster_number);
    }
}


uint8_t fat_find_unallocated_cluster(void)
{
    
    uint16_t fat_counter = 0x0;

    while(file_allocation_table[fat_counter] != 0)
        fat_counter++;

	return fat_counter;
    //return &file_allocation_table[fat_counter];

}


root_directory_entry* fat_find_free_root_entry(void)
{

    char* fat_counter;

    for(fat_counter = (char*)0x1800; (uint32_t)fat_counter < 0x1800 + (SECTOR_SIZE * 2); fat_counter++)
    { 
    
        if((uint32_t)fat_counter % 32 == 0 && *fat_counter == 0)
            return (root_directory_entry*)fat_counter;
    
    }

    return nullptr;

}




