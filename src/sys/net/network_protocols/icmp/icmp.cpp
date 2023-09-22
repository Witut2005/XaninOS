
#include <sys/net/network_protocols/icmp/icmp.h>
#include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/endian.h>
#include <lib/libc/memory.h>


NetworkResponse* IcmpResponse;
std::UnorderedMap<std::pair<uint16_t, uint16_t>, NetworkResponse*> IcmpModule::PacketsInfo;
uint32_t IcmpModule::echo_id_global;
uint32_t IcmpModule::echo_seq_global;

void IcmpModule::ping(uint32_t ip_dest, NetworkResponse* Response)
{

    IcmpResponse = Response;

    IcmpPacket* RequestPacket = (IcmpPacket*)malloc(SIZE_OF(IcmpPacket));
    RequestPacket->type = ICMP_ECHO_REQUEST;
    RequestPacket->code = 0x0;
    
    RequestPacket->echo_id = BIG_ENDIAN(IcmpModule::echo_id_global);
    RequestPacket->echo_sequence = BIG_ENDIAN(IcmpModule::echo_seq_global);
    
    RequestPacket->checksum = 0x0;
    RequestPacket->checksum = ipv4_checksum_get((address_t)RequestPacket, SIZE_OF(IcmpPacket));

    IcmpModule::echo_seq_global++;
    IcmpModule::echo_id_global++;

    // xprintf("%d %d\n", IcmpModule::echo_id_global, IcmpModule::echo_seq_global);

    ipv4_packet_send(ip_dest, xanin_ip_get(), INTERNET_CONTROL_MESSAGE_PROTOCOL, 64, (uint8_t*)RequestPacket, SIZE_OF(IcmpPacket), Response);
    free(RequestPacket);
}



void IcmpModule::ping_reply(IcmpPacket* PacketReceived, uint32_t ip_dest)
{
    IcmpPacket* ReplyPacket = (IcmpPacket*)malloc(SIZE_OF(IcmpPacket));
    ReplyPacket->type = ICMP_ECHO_REPLY;
    ReplyPacket->code = 0x0;
    
    ReplyPacket->echo_id = PacketReceived->echo_id;
    ReplyPacket->echo_sequence = PacketReceived->echo_sequence;
    
    ReplyPacket->checksum = 0x0;
    ReplyPacket->checksum = ipv4_checksum_get((address_t)ReplyPacket, SIZE_OF(IcmpPacket));

    if(is_loopback_packet())
        ipv4_packet_send(LOOPBACK_IP_ADDRESS, LOOPBACK_IP_ADDRESS, INTERNET_CONTROL_MESSAGE_PROTOCOL, 64, (uint8_t*)ReplyPacket, SIZE_OF(IcmpPacket), NULL);

    else
        ipv4_packet_send(ip_dest, xanin_ip_get(), INTERNET_CONTROL_MESSAGE_PROTOCOL, 64, (uint8_t*)ReplyPacket, SIZE_OF(IcmpPacket), NULL);
    

    free(ReplyPacket);
}


void IcmpModule::receive(IcmpPacket* IcmpPacketReceived, uint32_t ip_src)
{
    uint16_t echo_id = LITTLE_ENDIAN(IcmpPacketReceived->echo_id);
    uint16_t echo_sequence = LITTLE_ENDIAN(IcmpPacketReceived->echo_sequence);

    if(IcmpPacketReceived->type == ICMP_ECHO_REPLY)
    {
        if(IcmpModule::PacketsInfo.exists(std::make_pair(echo_id, echo_sequence)))
        {
            (*IcmpModule::PacketsInfo.find(std::make_pair(echo_id, echo_sequence)))->success = true;
            IcmpPacketReceived = LITTLE_ENDIAN(IcmpPacketReceived); 
            memcpy((uint8_t*)(*IcmpModule::PacketsInfo.find(std::make_pair(echo_id, echo_sequence)))->data, (uint8_t*)IcmpPacketReceived, SIZE_OF(IcmpPacket));
        }
    }

    else if(IcmpPacketReceived->type == ICMP_ECHO_REQUEST)
        IcmpModule::ping_reply(IcmpPacketReceived, ip_src);
}

extern "C"
{

void icmp_ping(uint32_t ip_dest, NetworkResponse* Response)
{
    return IcmpModule::ping(ip_dest, Response);
}

void icmp_ping_reply(IcmpPacket* PacketReceived, uint32_t ip_dest)
{
    return IcmpModule::ping_reply(PacketReceived, ip_dest);
}

void icmp_packet_receive(IcmpPacket* IcmpPacketReceived, uint32_t ip_src)
{
    return IcmpModule::receive(IcmpPacketReceived, ip_src);    
}


}