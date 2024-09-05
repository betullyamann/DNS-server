#ifndef DNSHEADER_H
#define DNSHEADER_H

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

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

class DNSWriteBuffer
{
public:
    DNSWriteBuffer() : buffer(0), position(0){};

    void write_uint8(uint8_t value);
    void write_uint16(uint16_t value);
    void write_uint32(uint32_t value);
    void write_string(const std::string& str);
    void write_domain_name(const std::string& domain);
    void write_compressed_domain_name(std::uint16_t domain_name_start);

    void create_domain_name_table(const std::string &domain);
    uint16_t get_previous_occurance(const std::string &domain);

private:
    std::vector<uint8_t> buffer;
    std::unordered_map<std::string, std::uint16_t> domain_name_table;
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

    void serialize(DNSWriteBuffer& buffer) const;
    void deserialize(DNSReadBuffer& buffer);
};

class DNSQuestion
{
public:
    std::string domain_name;
    uint16_t qtype;
    uint16_t qclass;
    
    void serialize(DNSWriteBuffer& buffer) const;
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

    void parse_ip_address(const char delimeter, DNSWriteBuffer& buffer) const;
    void write_IPV4_address(DNSWriteBuffer& buffer) const;
    void write_IPV6_address(DNSWriteBuffer& buffer) const;
    void write_CNAME(DNSWriteBuffer& buffer) const;
    void write_unsupported_resource_types(DNSWriteBuffer& buffer) const;
    void write_resource(DNSWriteBuffer &buffer) const;

public:
    std::string domain_name;
    uint16_t qtype;
    uint16_t qclass;
    uint32_t ttl;
    uint16_t data_length;
    std::string resource;
    
    void serialize(DNSWriteBuffer& buffer) const;
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

    void serialize(DNSWriteBuffer& buffer) const;
    void deserialize(DNSReadBuffer& buffer);

};

#endif // DNSHEADER_H
