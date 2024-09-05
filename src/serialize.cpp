#include "DNSMessage.h"

void DNSHeader::serialize(DNSWriteBuffer &buffer) const
{
    buffer.write_uint16(transaction_id);
    buffer.write_uint16(flags);
    buffer.write_uint16(question_count);
    buffer.write_uint16(answer_count);
    buffer.write_uint16(authority_count);
    buffer.write_uint16(additional_count);
}

void DNSQuestion::serialize(DNSWriteBuffer &buffer) const
{
    buffer.write_domain_name(domain_name);
//    buffer.create_domain_name_table(domain_name);

    buffer.write_uint16(qtype);
    buffer.write_uint16(qclass);
}

void DNSResource::serialize(DNSWriteBuffer &buffer) const
{
    buffer.write_domain_name(domain_name);
//    buffer.create_domain_name_table(domain_name);

    buffer.write_uint16(qtype);
    buffer.write_uint16(qclass);
    buffer.write_uint32(ttl);
    buffer.write_uint16(data_length);
    write_resource(buffer);
    //buffer.write_domain_name(resource);
}

void DNSMessage::serialize(DNSWriteBuffer &buffer) const
{
    header.serialize(buffer);

    for (const DNSQuestion question : questions)
    {
        question.serialize(buffer);
    }

    for (const DNSResource answer : answers)
    {
        answer.serialize(buffer);
    }

    for (const DNSResource authority : authorities)
    {
        authority.serialize(buffer);
    }

    for (const DNSResource additional : additionals)
    {
        additional.serialize(buffer);
    }
}
