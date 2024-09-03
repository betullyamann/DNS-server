#include <iostream>
#include <cstdint>
#include "DNSMessage.h"

std::vector<uint8_t> hexStringToByteArray(std::string hex_dns_message){
    std::vector<uint8_t> byte_array;
    for(int i = 0; i < hex_dns_message.size(); i+=2) {
        std::string substr = hex_dns_message.substr(i,2);
        uint8_t byte = static_cast<uint8_t>(strtol(substr.c_str(), nullptr, 16));
        byte_array.push_back(byte);
    }

    return byte_array;
}

int main() {

    const std::string raw_dns_message("8c538180000100050000000006636f6e666967057465616d73096d6963726f736f667403636f6d0000010001c00c0005000100000c8e002106636f6e666967057465616d730e747261666669636d616e61676572036e657400c038000500010000003a001c0c732d303030352d7465616d7306636f6e66696705736b797065c023c0650005000100000743001f0c636f6e6669672d7465616d7306732d3030303508732d6d7365646765c054c08d00050001000000440002c09ac09a00010001000000ac00043471c284");
    std::vector<uint8_t> byte_array = hexStringToByteArray(raw_dns_message);

    DNSReadBuffer buffer(byte_array);
    DNSMessage parsed_message;
    parsed_message.deserialize(buffer);
    return 0;
}