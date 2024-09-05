#include <sstream>
#include <string>

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


void DNSResource::write_IPV4_address(DNSWriteBuffer& buffer) const
{
    parse_ip_address('.', buffer);
}

void DNSResource::write_IPV6_address(DNSWriteBuffer& buffer) const
{
    parse_ip_address(':', buffer);
}

void DNSResource::parse_ip_address(const char delimeter, DNSWriteBuffer& buffer) const
{
    size_t start = 0, end;
    while((end = resource.find(delimeter, start)) != std::string::npos) 
    {
        buffer.write_uint8(static_cast<uint8_t>(std::stoi(resource.substr(start, end - start), nullptr, 10)));
        start = end + 1;
    }

    buffer.write_uint8(static_cast<uint8_t>(std::stoi(resource.substr(start), nullptr, 10)));

}

void DNSResource::write_CNAME(DNSWriteBuffer& buffer) const
{
    buffer.write_domain_name(resource);
}

void DNSResource::write_unsupported_resource_types(DNSWriteBuffer& buffer) const
{
    buffer.write_string(resource);
}

void DNSResource::write_resource(DNSWriteBuffer &buffer) const
{
    switch (qtype)
    {
        case 1: // "A"
        {
            write_IPV4_address(buffer);
            break;
        }
        case 5: // "AAAA"
        {
            write_CNAME(buffer);
            break;
        }
        case 28: // "CNAME"
        {
            write_IPV6_address(buffer);
            break;
        }
        default:
        {
            write_unsupported_resource_types(buffer);
        }
    }
}
