#include "DNSMessage.h"

void DNSHeader::deserialize(DNSBuffer &buffer)
{
    transaction_id = buffer.read_uint16();
    flags = buffer.read_uint16();
    question_count = buffer.read_uint16();
    answer_count = buffer.read_uint16();
    authority_count = buffer.read_uint16();
    additional_count = buffer.read_uint16();
}

void DNSQuestion::deserialize(DNSBuffer &buffer)
{
    domain_name = buffer.read_domain_name();
    qtype = buffer.read_uint16();
    qclass = buffer.read_uint16();
}

void DNSRecord::deserialize(DNSBuffer &buffer)
{
    domain_name = buffer.read_domain_name();
    qtype = buffer.read_uint16();
    qclass = buffer.read_uint16();
    ttl = buffer.read_uint8();
    data_length = buffer.read_uint16();
}

void DNSMessage::deserialize(DNSBuffer &buffer)
{
    header.deserialize(buffer);

    questions.resize(header.get_question_count());
    for(auto &question : questions){
        question.deserialize(buffer);
    }

    answers.resize(header.get_answer_count());
    for(auto &answer : answers){
        answer.deserialize(buffer);
    }

    authorities.resize(header.get_authority_count());
    for(auto &authority : authorities){
        authority.deserialize(buffer);
    }

    additionals.resize(header.get_additional_count());
    for(auto &addition : additionals){
        addition.deserialize(buffer);
    }
}