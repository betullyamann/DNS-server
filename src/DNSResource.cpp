#include <sstream>

#include "DNSMessage.h"

void DNSResource::read_IPV4_address(DNSReadBuffer& buffer)
{
    for(size_t i=0; i < data_length; ++i){
        unsigned char byte = static_cast<unsigned char>(buffer.read_uint8());

        if(i != 0)
            resource += ".";
        
        resource += std::to_string(static_cast<int>(byte));
    }
}

void DNSResource::read_IPV6_address(DNSReadBuffer& buffer)
{
    std::ostringstream hex_string;
    for(size_t i=0; i < data_length; ++i){
        hex_string << std::hex << buffer.read_uint8() << ":";
    }
    resource = hex_string.str();
}

void DNSResource::read_CNAME(DNSReadBuffer& buffer)
{
    resource = buffer.read_domain_name();
}

void DNSResource::read_unsupported_resource_types(DNSReadBuffer& buffer)
{
    std::ostringstream hex_string;
    for(size_t i=0; i < data_length; ++i){
        hex_string << std::hex << buffer.read_uint8();
    }
    resource = hex_string.str();
}

void DNSResource::read_resource(DNSReadBuffer &buffer)
{
    switch (qtype)
    {
        case 1: // "A"
        {
            read_IPV4_address(buffer);
            break;
        }
        case 5: // "AAAA"
        {
            read_CNAME(buffer);
            break;
        }
        case 28: // "CNAME"
        {
            read_IPV6_address(buffer);
            break;
        }
        default:
        {
            read_unsupported_resource_types(buffer);
        }
    }
}
