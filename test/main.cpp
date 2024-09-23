#include "DNSMessage.h"
#include "test_deserialization.h"
#include "test_serialization.h"

std::vector<uint8_t> hexStringToByteArray(std::string hex_dns_message)
{
    std::vector<uint8_t> byte_array;
    for (size_t i = 0; i < hex_dns_message.size(); i += 2)
    {
        std::string substr = hex_dns_message.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(strtol(substr.c_str(), nullptr, 16));
        byte_array.push_back(byte);
    }

    return byte_array;
}

int main()
{
    const std::string raw_dns_message("3c4d81800002000200020002076578616D706C6503636F6D000001000103777777c00c004100010c6D61696C2D6578616D706C65c0140001000100000E100004c0000201036E7331c00c001c000100000e100010fe800000000000001e61b4fffeb5593cc01d0005000100000e10001005616C696173c00cc00c0006000100000e10002e036e7331c02c0a686f73746d617374657206666173746c79c03800000001000070800000038400093a800000012cc00c0002000100000e10000ec042c01d0045000100000e10000b036e7331c02c0a686f7374");
    std::vector<uint8_t> byte_array = hexStringToByteArray(raw_dns_message);
    DNSReadBuffer read_buffer(byte_array);

    DNSMessage message;
    message.header.transaction_id = 0x3c4d;
    message.header.flags = 0x8180; // (Recursion Desired and Recursion Available)
    message.header.question_count = 2;
    message.header.answer_count = 2;
    message.header.authority_count = 2;
    message.header.additional_count = 2;

    DNSQuestion question;
    question.domain_name = "example.com";
    question.qtype = 1;  // A - IPv4 Address
    question.qclass = 1; // INternet
    message.questions.push_back(question);


    question.domain_name = "www.example.com";
    question.qtype = 65; // HTTPS Specific Service Endpoints
    question.qclass = 1; // INternet
    message.questions.push_back(question);

    DNSResource answer;
    answer.domain_name = "mail-example.com";
    answer.qtype = 1;  // A - IPv4 Address
    answer.qclass = 1; // INternet
    answer.ttl = 3600;
    answer.data_length = 4;
    answer.resource = "192.0.2.1";
    message.answers.push_back(answer);

    answer.domain_name = "ns1.example.com";
    answer.qtype = 28; // AAAA  - IPv6 Address
    answer.qclass = 1; // INternet
    answer.ttl = 3600;
    answer.data_length = 16;
    answer.resource = "fe80::1e61:b4ff:feb5:593c";
    message.answers.push_back(answer);

    DNSResource authority;
    authority.domain_name = "www.example.com";
    authority.qtype = 5;  // CNAME
    authority.qclass = 1; // INternet
    authority.ttl = 3600;
    authority.data_length = 16;
    authority.resource = "alias.example.com";
    message.authorities.push_back(authority);

    authority.domain_name = "example.com";
    authority.qtype = 6;  // SOA - Start of Authority
    authority.qclass = 1; // INternet
    authority.ttl = 3600;
    authority.data_length = 46;
    std::vector<uint8_t> auth = hexStringToByteArray("036e7331c02c0a686f73746d617374657206666173746c79c03800000001000070800000038400093a800000012c");
    authority.resource = std::string(auth.begin(), auth.end());
    message.authorities.push_back(authority);

    DNSResource additional;
    additional.domain_name = "example.com";
    additional.qtype = 2;  // NS - Name Server
    additional.qclass = 1; // INternet
    additional.ttl = 3600;
    additional.data_length = 14;
    additional.resource = "ns1.example.com";
    message.additionals.push_back(additional);

    additional.domain_name = "www.example.com";
    additional.qtype = 65; // HTTPS Specific Service Endpoints
    additional.qclass = 1; // INternet
    additional.ttl = 3600;
    additional.data_length = 11;
    std::vector<uint8_t> add = hexStringToByteArray("036e7331c02c0a686f7374");
    additional.resource = std::string(add.begin(), add.end());
    message.additionals.push_back(additional);

    test_message_deserialization(message, read_buffer);

    DNSWriteBuffer write_buffer;
    test_message_serialization(message, write_buffer, byte_array);
}