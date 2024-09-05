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

    const std::string raw_dns_message("b51881800001001000000000146f7074696d697a6174696f6e67756964652d70610a676f6f676c656170697303636f6d0000010001c00c000100010000006c00048efb8c2ac00c000100010000006c00048efb8c4ac00c000100010000006c0004acd9a98ac00c000100010000006c00048efb8d2ac00c000100010000006c00048efb8c0ac00c000100010000006c0004d83ad42ac00c000100010000006c0004d83ad40ac00c000100010000006c00048efabb8ac00c000100010000006c0004acd9a9aac00c000100010000006c00048efabb6ac00c000100010000006c00048efab88ac00c000100010000006c0004d83ad68ac00c000100010000006c0004d83ad56ac00c000100010000006c0004acd9a9cac00c000100010000006c0004acd9144ac00c000100010000006c0004acd9a96a");
    std::vector<uint8_t> byte_array = hexStringToByteArray(raw_dns_message);

    DNSReadBuffer read_buffer(byte_array);
    DNSMessage parsed_message;
    parsed_message.deserialize(read_buffer);

    DNSWriteBuffer write_buffer;
    parsed_message.serialize(write_buffer);
    
    return 0;
}