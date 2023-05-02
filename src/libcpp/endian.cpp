

#include <stdint.h>
#include <libcpp/endian.h>
#include <network_protocols/icmp/icmp.h>


IcmpPacket* endian_switch(IcmpPacket* IcmpData)
{
    IcmpData->checksum = endian_switch(IcmpData->checksum);
    IcmpData->echo_id = endian_switch(IcmpData->echo_id);
    IcmpData->echo_sequence = endian_switch(IcmpData->echo_sequence);
    return IcmpData;
}
