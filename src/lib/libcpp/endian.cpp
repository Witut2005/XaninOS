

#include <stdint.h>
#include <lib/libcpp/endian.h>


IcmpPacket* endian_switch(IcmpPacket* IcmpData)
{
    IcmpData->checksum = endian_switch(IcmpData->checksum);
    IcmpData->echo_id = endian_switch(IcmpData->echo_id);
    IcmpData->echo_sequence = endian_switch(IcmpData->echo_sequence);
    return IcmpData;
}


AddressResolutionProtocol* endian_switch(AddressResolutionProtocol* ArpData)
{
    ArpData->hardware_type = endian_switch(ArpData->hardware_type);
    ArpData->protocol_type = endian_switch(ArpData->protocol_type);
    ArpData->opcode = endian_switch(ArpData->opcode);
    ArpData->source_protocol_address = endian_switch(ArpData->source_protocol_address);
    ArpData->destination_protocol_address = endian_switch(ArpData->destination_protocol_address);

    return ArpData;

}