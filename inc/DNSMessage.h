#ifndef DNSHEADER_H
#define DNSHEADER_H

#include <cstdint>
#include <string>
#include <vector>

class DNSHeader
{
private:
    uint16_t transaction_id;
    uint16_t flags;  
    uint16_t question_count; 
    uint16_t answer_count;
    uint16_t authority_count;
    uint16_t additional_count;
public:
    uint16_t get_question_count();
    uint16_t get_answer_count();
    uint16_t get_authority_count();
    uint16_t get_additional_count();

    void serialize(DNSBuffer& buffer) const;
    void deserialize(DNSBuffer& buffer);
};

class DNSQuestion
{
private:
    std::string domain_name;
    uint16_t qtype;
    uint16_t qclass;
public:
    std::string get_domain_name();
    
    void serialize(DNSBuffer& buffer) const;
    void deserialize(DNSBuffer& buffer);
};

class DNSRecord
{
private:
    std::string domain_name;
    uint16_t qtype;
    uint16_t qclass;
    uint32_t ttl;
    uint16_t data_length;
    //std::vector<unsigned char> rdata;
public:
    void serialize(DNSBuffer& buffer) const;
    void deserialize(DNSBuffer& buffer);
};

class DNSBuffer
{
public:
    DNSBuffer(size_t size) : buffer(size), position(0){};

    uint8_t read_uint8();
    uint16_t read_uint16();
    uint32_t read_uint32();
    std::string read_domain_name();

    void write_uint8(uint8_t value);
    void write_uint16(uint16_t value);
    void write_uint32(uint32_t value);
    void write_domain_name(const std::string& domain);
private:
    std::vector<uint8_t> buffer;
    size_t position;
};

class DNSMessage
{
private:
    DNSHeader header;
    std::vector<DNSQuestion> questions;
    std::vector<DNSRecord> answers;
    std::vector<DNSRecord> authorities;
    std::vector<DNSRecord> additionals;
public:    

    void serialize(DNSBuffer& buffer) const;
    void deserialize(DNSBuffer& buffer);

};

#endif // DNSHEADER_H
