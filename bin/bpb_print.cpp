

#include <fstream>
#include <iostream>
#include <inttypes.h>

using namespace std;


struct bios_parameter_block
{

    uint64_t oem_id;
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t number_of_fats;
    uint16_t number_os_rde; /* root directory entry */
    uint16_t total_sectors;
    uint8_t media_descriptor_type;
    uint16_t number_of_sectors_per_fat;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t large_sector_counter;

}__attribute__((packed));

struct bios_parameter_block bpb;

int main(int argc, char* argv[])
{

    if(argc != 2)
    {
        cout << "usage: " << argv[0] << " file" << endl;
        exit(1);
    }

    fstream file;
    file.open(argv[1], ios::in | ios::out | ios::binary);

    file.seekg(ios::beg + 0x3);

    file.read((char*)&bpb, sizeof(bios_parameter_block));

    printf("YOUR FAT INFO:\n");
    printf("oem_id: %lx\n", bpb.oem_id);
    printf("bytes_per_sector: %x\n", bpb.bytes_per_sector);
    printf("sectors_per_cluster: %x\n", bpb.sectors_per_cluster);
    printf("reserved_sectors: %x\n", bpb.reserved_sectors);
    printf("number of fats: %x\n", bpb.number_of_fats);
    printf("number_of_root_directory_tables: %x\n", bpb.number_os_rde);
    printf("total_sectors: %x\n", bpb.total_sectors);
    printf("media_descriptor_type: %x\n", bpb.media_descriptor_type);
    printf("number_of_sectors_per_fat: %x\n", bpb.number_of_sectors_per_fat);
    printf("heads: %x\n", bpb.heads);
    printf("hidden_sectors: %x\n", bpb.hidden_sectors);
    printf("large_sector_counter: %x\n", bpb.large_sector_counter);



    

}
