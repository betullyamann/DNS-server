#include <DNSMessage.h>
#include <stdexcept>

// GETTER functions
uint16_t DNSHeader::get_question_count()
{
    return question_count;
}

uint16_t DNSHeader::get_answer_count()
{
    return answer_count;
}

uint16_t DNSHeader::get_authority_count()
{
    return authority_count;
}

uint16_t DNSHeader::get_additional_count()
{
    return additional_count;
}

// READ functions
uint8_t DNSBuffer::read_uint8()
{
    if (position < buffer.size())
    {
        return buffer[position++];
    }
    else
    {
        throw std::out_of_range("BUffer overflow");
    }
}

uint16_t DNSBuffer::read_uint16()
{
    return ((read_uint8() << 8) | read_uint8());
}

uint32_t DNSBuffer::read_uint32()
{
    return ((read_uint16() << 16) | read_uint16());
}

std::string DNSBuffer::read_domain_name()
{
    std::string domain_name = "";
    size_t current_position;

    while (true)
    {
        size_t length = read_uint8();
        if (length == 0)
        {
            break;
        }
        else
        {
            /*
            In message compression the first two bits are ones.
            This allows a pointer to be distinguished from a label,
            since the label must begin with two zero bits because
            labels are restricted to 63 octets or less.
            */
            if (length & 0b11000000 != 0)
            {
                current_position = position;
                position =  ((length << 8) | read_uint8()) & 0b00111111; // offset from the start of the message
                domain_name += read_domain_name();
                position = current_position;
            }
            else
            {
                std::string label(buffer.begin() + position, buffer.begin() + position + length);
                domain_name += label + ".";
                position += length;
            }
        }
    }

    return domain_name;
}
