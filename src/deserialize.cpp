#include "DNSMessage.h"

void DNSHeader::deserialize(DNSReadBuffer &buffer)
{
    transaction_id = buffer.read_uint16();
    flags = buffer.read_uint16();
    question_count = buffer.read_uint16();
    answer_count = buffer.read_uint16();
    authority_count = buffer.read_uint16();
    additional_count = buffer.read_uint16();
}

void DNSQuestion::deserialize(DNSReadBuffer &buffer)
{
    domain_name = buffer.read_domain_name();
    qtype = buffer.read_uint16();
    qclass = buffer.read_uint16();
}

void DNSResource::deserialize(DNSReadBuffer &buffer)
{
    domain_name = buffer.read_domain_name();
    qtype = buffer.read_uint16();
    qclass = buffer.read_uint16();
    ttl = buffer.read_uint32();
    data_length = buffer.read_uint16();
    read_resource(buffer);
}

void DNSMessage::deserialize(DNSReadBuffer &buffer)
{
    header.deserialize(buffer);

    questions.resize(header.question_count);
    for (auto &question : questions)
    {
        question.deserialize(buffer);
    }

    answers.resize(header.answer_count);
    for (auto &answer : answers)
    {
        answer.deserialize(buffer);
    }

    authorities.resize(header.authority_count);
    for (auto &authority : authorities)
    {
        authority.deserialize(buffer);
    }

    additionals.resize(header.additional_count);
    for (auto &addition : additionals)
    {
        addition.deserialize(buffer);
    }
}