#include <queue>

#include "DNSMessage.h"

extern std::queue<size_t> end_of_message_parts;

void test_message_serialization(const DNSMessage &message, DNSWriteBuffer& buffer, const std::vector<uint8_t> &raw_message);

void test_header_serialization(const DNSHeader &header, DNSWriteBuffer &buffer, const std::vector<uint8_t> header_in_bytes);

void test_question_serialization(const DNSQuestion &question, DNSWriteBuffer &buffer, const std::vector<uint8_t> question_in_bytes);

void test_resource_serialization(const DNSResource &resource, DNSWriteBuffer &buffer, const std::vector<uint8_t> resource_in_bytes);
