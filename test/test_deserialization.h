#include <queue>

#include "DNSMessage.h"

extern std::queue<size_t> end_of_message_parts;

void test_header_deserialization(const DNSHeader &header, DNSReadBuffer &buffer);

void test_question_deserialization(const DNSQuestion &question, DNSReadBuffer &buffer);

void test_resource_deserialization(const DNSResource &resource, DNSReadBuffer &buffer);

void test_message_deserialization(const DNSMessage &message,  DNSReadBuffer &buffer);