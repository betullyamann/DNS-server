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

void DNSReadBuffer::fill_domain_name_table(const std::string& domain_name, std::vector<size_t>& domain_names_offsets)
{
    size_t start_of_dn = 0;
    std::string dn = "." + domain_name;
    for(size_t i = 0; i  < domain_names_offsets.size(); ++i)
    {
        if((start_of_dn = dn.find(".", start_of_dn)) != std::string::npos)
        {
            rb_domain_name_table.insert({domain_names_offsets[i], dn.substr(++start_of_dn)});
        }
    }
}

std::string DNSReadBuffer::read_domain_name()
{
    std::string domain_name = "";
    std::vector<size_t> domain_names_offsets;

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
                size_t domain_name_offset = ((length & 0b00111111) << 8) | read_uint8(); // offset from the start of the message
                if(!rb_domain_name_table[domain_name_offset].empty())
                {
                    domain_name = rb_domain_name_table[domain_name_offset];
                    break;
                }
                else
                {
                    //unexpected behaviour
                }
            }
            else
            {
                if(!domain_name.empty())
                    domain_name += ".";
                std::string label(buffer.begin() + position, buffer.begin() + position + length);
                domain_name += label;
                domain_names_offsets.push_back(position - 1); // length should also be included
                position += length;
            }
        }
    }

    fill_domain_name_table(domain_name, domain_names_offsets);
    return domain_name;
}

size_t DNSReadBuffer::get_position() const
{
    return position;
}

uint8_t DNSReadBuffer::get_current_value() const
{
    return buffer[position];
}