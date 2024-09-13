#include "DNSMessage.h"
#include "test_deserialization.h"
#include "test_serialization.h"

std::vector<uint8_t> hexStringToByteArray(std::string hex_dns_message){
    std::vector<uint8_t> byte_array;
    for(int i = 0; i < hex_dns_message.size(); i+=2) {
        std::string substr = hex_dns_message.substr(i,2);
        uint8_t byte = static_cast<uint8_t>(strtol(substr.c_str(), nullptr, 16));
        byte_array.push_back(byte);
    }

    return byte_array;
}

int main()
{
    const std::string raw_dns_message("03308180000100010001000014617564696f2d616b2d73706f746966792d636f6d09616b616d61697a6564036e65740000410001c00c00050001000000da00130461323937046473636306616b616d6169c02bc045000600010000009b0034066e3064736363c04a0a686f73746d617374657206616b616d616903636f6d0066e47358000003e8000003e8000003e800000708");
    std::vector<uint8_t> byte_array = hexStringToByteArray(raw_dns_message);
    DNSReadBuffer read_buffer(byte_array);

    DNSMessage message;
    message.header.transaction_id = 0x0330;
    message.header.flags = 0x8180;
    message.header.question_count = 1;
    message.header.answer_count = 1;
    message.header.authority_count = 1;
    message.header.additional_count = 0;

    DNSQuestion question;
    question.domain_name = "audio-ak-spotify-com.akamaized.net";
    question.qtype = 65;
    question.qclass = 1;
    message.questions.push_back(question);

    DNSResource answer;
    answer.domain_name = "audio-ak-spotify-com.akamaized.net";
    answer.qtype = 5;
    answer.qclass = 1;
    answer.ttl = 218;
    answer.data_length = 19;
    answer.resource = "a297.dscc.akamai.net";
    message.answers.push_back(answer);

    // answer.domain_name = "mobile.events.data.trafficmanager.net";
    // answer.qtype = 5;
    // answer.qclass = 1;
    // answer.ttl = 27;
    // answer.data_length = 44;
    // answer.resource = "onedscolprdcus05.centralus.cloudapp.azure.com";
    // message.answers.push_back(answer);

    DNSResource authority;
    authority.domain_name = "dscc.akamai.net";
    authority.qtype = 6;
    authority.qclass = 1;
    authority.ttl = 155;
    authority.data_length = 52;
    authority.resource = std::string(byte_array.end()-52, byte_array.end());
    message.authorities.push_back(authority);

    test_message_deserialization(message, read_buffer);

    DNSWriteBuffer write_buffer;
    test_message_serialization(message, write_buffer, byte_array);

}