#include <cassert>
#include <iostream>
#include <queue>

#include "DNSMessage.h"
#include "test_serialization.h"

extern std::queue<size_t> end_of_message_parts;

void test_header_serialization(const DNSHeader &header, DNSWriteBuffer &buffer, const std::vector<uint8_t> header_in_bytes)
{
    size_t start_of_header = buffer.get_position();
    header.serialize(buffer);
    assert((buffer.get_buffer_from(start_of_header) == header_in_bytes) == true);
}

void test_question_serialization(const DNSQuestion &question, DNSWriteBuffer &buffer, const std::vector<uint8_t> question_in_bytes)
{
    size_t start_of_question = buffer.get_position();
    question.serialize(buffer);
    assert((buffer.get_buffer_from(start_of_question) == question_in_bytes) == true);
}

void test_resource_serialization(const DNSResource &resource, DNSWriteBuffer &buffer, const std::vector<uint8_t> resource_in_bytes)
{
    size_t start_of_resource = buffer.get_position();
    resource.serialize(buffer);
    auto temp = buffer.get_buffer_from(start_of_resource);
    assert(( temp == resource_in_bytes) == true);
}

std::vector<uint8_t> get_message_part(DNSWriteBuffer &buffer, const std::vector<uint8_t> &raw_message)
{
    std::vector<uint8_t> message_part_in_bytes(raw_message.begin() + buffer.get_position(), raw_message.begin() + end_of_message_parts.front());
    end_of_message_parts.pop();
    return message_part_in_bytes;
}

void test_message_serialization(const DNSMessage &message, DNSWriteBuffer &buffer, const std::vector<uint8_t> &raw_message)
{
    std::cout << "Header serialization: ";
    test_header_serialization(message.header, buffer, get_message_part(buffer, raw_message));
    std::cout << "PASSED !" << std::endl;

    int counter = 1;
    for (const DNSQuestion &question : message.questions)
    {   
        std::cout << "Question " << counter++ << " serialization: ";
        test_question_serialization(question, buffer, get_message_part(buffer, raw_message));
        std::cout << "PASSED !" << std::endl;
    }

    counter = 1;
    for (const DNSResource &answer : message.answers)
    {
        std::cout << "Answer " << counter++ << " serialization: ";
        test_resource_serialization(answer, buffer, get_message_part(buffer, raw_message));
        std::cout << "PASSED !" << std::endl;
    }

    counter = 1;
    for (const DNSResource &authority : message.authorities)
    {
        std::cout << "Authority " << counter++ << " serialization: ";
        test_resource_serialization(authority, buffer, get_message_part(buffer, raw_message));
        std::cout << "PASSED !" << std::endl;
    }

    counter = 1;
    for (const DNSResource &additional : message.additionals)
    {
        std::cout << "Additional " << counter++ << " serialization: ";
        test_resource_serialization(additional, buffer, get_message_part(buffer, raw_message));
        std::cout << "PASSED !" << std::endl;
    }    

    std::cout << "Message serialization: PASSED !" << std::endl;
}