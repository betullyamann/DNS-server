#include <cassert>
#include <cstring>
#include <iostream>
#include <queue>

#include "DNSMessage.h"
#include "test_deserialization.h"

std::queue<size_t> end_of_message_parts;

void test_header_deserialization(const DNSHeader &header, DNSReadBuffer &buffer)
{
    DNSHeader header_byte_array;
    header_byte_array.deserialize(buffer);

    assert(header.transaction_id == header_byte_array.transaction_id);
    assert(header.flags == header_byte_array.flags);
    assert(header.question_count == header_byte_array.question_count);
    assert(header.answer_count == header_byte_array.answer_count);
    assert(header.authority_count == header_byte_array.authority_count);
    assert(header.additional_count == header_byte_array.additional_count);

    end_of_message_parts.push(buffer.get_position());
}

void test_question_deserialization(const DNSQuestion &question, DNSReadBuffer &buffer)
{
    DNSQuestion question_byte_array;
    question_byte_array.deserialize(buffer);

    assert(!strcmp(question.domain_name.c_str(), question_byte_array.domain_name.c_str()));
    assert(question.qtype == question_byte_array.qtype);
    assert(question.qclass == question_byte_array.qclass);

    end_of_message_parts.push(buffer.get_position());
}

void test_resource_deserialization(const DNSResource &resource, DNSReadBuffer &buffer)
{
    DNSResource resource_byte_array;
    resource_byte_array.deserialize(buffer);

    assert(!strcmp(resource.domain_name.c_str(), resource_byte_array.domain_name.c_str()));
    assert(resource.qtype == resource_byte_array.qtype);
    assert(resource.qclass == resource_byte_array.qclass);
    assert(resource.ttl == resource_byte_array.ttl);
    assert(resource.data_length == resource_byte_array.data_length);
    assert(!strcmp(resource.resource.c_str(), resource_byte_array.resource.c_str()));

    end_of_message_parts.push(buffer.get_position());
}

void test_message_deserialization(const DNSMessage &message, DNSReadBuffer &buffer)
{
    std::cout << "Header deserialization: ";
    test_header_deserialization(message.header, buffer);
    std::cout << "PASSED !" << std::endl;

    int counter = 1;
    for (const auto &question : message.questions)
    {
        std::cout << "Question " << counter++ << " deserialization: ";
        test_question_deserialization(question, buffer);
        std::cout << "PASSED !" << std::endl;
    }

    counter = 1;
    for (const auto &answer : message.answers)
    {
        std::cout << "Answer " << counter++ << " deserialization: ";
        test_resource_deserialization(answer, buffer);
        std::cout << "PASSED !" << std::endl;
    }

    counter = 1;
    for (const auto &authority : message.authorities)
    {
        std::cout << "Authority " << counter++ << " deserialization: ";
        test_resource_deserialization(authority, buffer);
        std::cout << "PASSED !" << std::endl;
    }

    counter = 1;
    for (const auto &additional : message.additionals)
    {
        std::cout << "Additional " << counter++ << " deserialization: ";
        test_resource_deserialization(additional, buffer);
        std::cout << "PASSED !" << std::endl;
    }

    std::cout << "Message deserialization: PASSED !" << std::endl;

}