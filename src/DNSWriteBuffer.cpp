#include "DNSMessage.h"

void DNSWriteBuffer::write_uint8(uint8_t value)
{
    buffer.push_back(value);
    position++;
}

void DNSWriteBuffer::write_uint16(uint16_t value)
{
    write_uint8((value >> 8) & 0xFF);
    write_uint8(value & 0xFF);
}

void DNSWriteBuffer::write_uint32(uint32_t value)
{
    write_uint16((value >> 16) & 0xFFFF);
    write_uint16(value & 0xFFFF);
}

void DNSWriteBuffer::write_string(const std::string &str)
{
    buffer.insert(buffer.end(), str.begin(), str.end());
    position += str.size();
}

void DNSWriteBuffer::write_domain_name(const std::string &domain)
{
    bool is_compressed = false;
    size_t start = 0, end;
    while ((end = domain.find('.', start)) != std::string::npos)
    {
        if (uint16_t offset = get_previous_occurance(domain.substr(start)); offset != 0)
        {
            is_compressed = true;
            write_compressed_domain_name(offset);
            break;
        }
        else
        {
            domain_name_table.insert({domain.substr(start), position});

            write_uint8(end - start);
            write_string(domain.substr(start, end - start));
            start = end + 1;
        }
    }

    if (!is_compressed)
    {
        if (uint16_t offset = get_previous_occurance(domain.substr(start)); offset != 0)
        {
            is_compressed = true;
            write_compressed_domain_name(offset);
        }
        else
        {
            domain_name_table.insert({domain.substr(start), position});

            write_uint8(domain.size() - start);
            write_string(domain.substr(start));
            write_uint8(0); // end of the string
        }
    }
}

void DNSWriteBuffer::write_compressed_domain_name(std::uint16_t domain_name_start)
{
    write_uint16(domain_name_start | 0xC000);
}

uint16_t DNSWriteBuffer::get_previous_occurance(const std::string &domain)
{
    if (auto iterator = domain_name_table.find(domain); iterator != domain_name_table.end())
    {
        return iterator->second;
    }
    else
    {
        return 0; // start of header
    }
}

// void split_domain_name(std::vector<std::string>& label_list, std::string domain)
// {
//     size_t start = 0, end;   
//     while ((end = domain.find('.', start)) != std::string::npos )
//     {
//         label_list.push_back(domain.substr(start, end - start));
//         start = end + 1;
//     }

//     if(start < domain.size())
//         label_list.push_back(domain.substr(start));
// }

// void DNSWriteBuffer::create_domain_name_table(const std::string &domain)
// {
//     size_t start = 0, end;
//     size_t offset = position;
//     while ((end = domain.find('.', start)) != std::string::npos)
//     {
//         std::string sub_domain = domain.substr(start);
//         if (domain_name_table.find(sub_domain) == domain_name_table.end())
//         {
//             domain_name_table.insert({sub_domain, offset});
//         }
//         offset += end - start;
//         start = end + 1;
//     }

//     std::string sub_domain = domain.substr(start);
//     if (domain_name_table.find(sub_domain) == domain_name_table.end())
//     {
//         domain_name_table.insert({sub_domain, offset});
//     }
// }
