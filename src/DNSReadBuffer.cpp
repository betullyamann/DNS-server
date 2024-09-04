#include <DNSMessage.h>
#include <stdexcept>

uint8_t DNSReadBuffer::read_uint8()
{
    if (position < buffer.size())
    {
        return buffer[position++];
    }
    else
    {
        throw std::out_of_range("Buffer overflow");
    }
}

uint16_t DNSReadBuffer::read_uint16()
{
    return ((read_uint8() << 8) | read_uint8());
}

uint32_t DNSReadBuffer::read_uint32()
{
    return ((read_uint16() << 16) | read_uint16());
}

std::string DNSReadBuffer::read_domain_name()
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
            if ((length & 0b11000000) == 0b11000000)
            {
                current_position = position;
                position = ((length & 0b00111111) << 8) | read_uint8(); // offset from the start of the message
                domain_name += read_domain_name();
                position = current_position + 1;
                break;
    
            }
            else
            {
                if(!domain_name.empty())
                    domain_name += ".";
                std::string label(buffer.begin() + position, buffer.begin() + position + length);
                domain_name += label;
                position += length;
            }
        }
    }

    return domain_name;
}

