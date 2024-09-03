#ifndef DNSHEADER_H
#define DNSHEADER_H

#include <cstdint>
#include <string>
#include <vector>

class DNSReadBuffer
{
public:
    DNSReadBuffer(const size_t size) : buffer(size), position(0){};
    DNSReadBuffer(const std::vector<uint8_t> raw_message) : buffer(raw_message.begin(), raw_message.end()), position(0){};

    uint8_t read_uint8();
    uint16_t read_uint16();
    uint32_t read_uint32();
    
    std::string read_domain_name();

private:
    std::vector<uint8_t> buffer;
    size_t position;
};

class DNSHeader
{
public:
    uint16_t transaction_id;
    uint16_t flags;  
    uint16_t question_count; 
    uint16_t answer_count;
    uint16_t authority_count;
    uint16_t additional_count;

    void deserialize(DNSReadBuffer& buffer);
};

class DNSQuestion
{
public:
    std::string domain_name;
    uint16_t qtype;
    uint16_t qclass;
    
    void deserialize(DNSReadBuffer& buffer);
};

class DNSResource
{
private:
    void read_IPV4_address(DNSReadBuffer& buffer);
    void read_IPV6_address(DNSReadBuffer& buffer);
    void read_CNAME(DNSReadBuffer& buffer);
    void read_unsupported_resource_types(DNSReadBuffer& buffer);
    
    void read_resource(DNSReadBuffer &buffer);

public:
    std::string domain_name;
    uint16_t qtype;
    uint16_t qclass;
    uint32_t ttl;
    uint16_t data_length;
    std::string resource;
    
    void deserialize(DNSReadBuffer& buffer);
};

class DNSMessage
{
public:
    DNSHeader header;
    std::vector<DNSQuestion> questions;
    std::vector<DNSResource> answers;
    std::vector<DNSResource> authorities;
    std::vector<DNSResource> additionals;

    void deserialize(DNSReadBuffer& buffer);

};

#endif // DNSHEADER_H
