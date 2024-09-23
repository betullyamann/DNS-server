#include <algorithm>
#include <map>
#include <iomanip>
#include <sstream>
#include <string>

#include "DNSMessage.h"

void DNSResource::read_IPV4_address(DNSReadBuffer &buffer)
{
    for (size_t i = 0; i < data_length; ++i)
    {
        unsigned char byte = static_cast<unsigned char>(buffer.read_uint8());

        if (i != 0)
            resource += ".";

        resource += std::to_string(static_cast<int>(byte));
    }
}

void compress_ipv6(std::string &resource)
{
    size_t start_of_group, end_of_group = 0;
    size_t prev_index;
    char delimeter = ':';
    int address_group_length = 4;
    bool is_new_zero_group = true;
    std::map<size_t, size_t> sequence_of_zeroes;

    resource += delimeter;

    while ((end_of_group = resource.find(delimeter, end_of_group + 1)) != std::string::npos)
    {
        start_of_group = end_of_group - address_group_length;
        if (std::count(resource.begin() + start_of_group, resource.begin() + end_of_group, '0') == 4)
        {
            if (is_new_zero_group)
            {
                sequence_of_zeroes[start_of_group] = 1;
                prev_index = start_of_group;
                is_new_zero_group = false;
            }
            else
            {
                sequence_of_zeroes[prev_index] += 1;
            }
        }
        else
        {
            while (start_of_group < end_of_group && resource[start_of_group] == '0')
            {
                resource.erase(start_of_group, 1);
            }
            is_new_zero_group = true;
        }
    }

    resource.erase(resource.end() - 1); // remove the delimeter from the end

    auto longest_zeroes = std::max_element(sequence_of_zeroes.begin(), sequence_of_zeroes.end(), [](const std::pair<size_t, int> &p1, const std::pair<size_t, int> &p2)
                                           { return p1.second < p2.second; });

    resource.erase(resource.begin() + longest_zeroes->first, resource.begin() + longest_zeroes->first + (longest_zeroes->second * 5) - 1);
}

void DNSResource::read_IPV6_address(DNSReadBuffer &buffer)
{
    std::stringstream hex_str;
    for (size_t i = 0; i < data_length; ++i)
    {
        unsigned char byte = static_cast<unsigned char>(buffer.read_uint8());

        if (i != 0 && i % 2 == 0)
        {
            hex_str << ":";
        }

        hex_str << std::hex << static_cast<int>(byte);
        if (static_cast<int>(byte) == 0)
            hex_str << '0'; // for 00
    }
    resource = hex_str.str();

    compress_ipv6(resource);
}

void DNSResource::read_CNAME(DNSReadBuffer &buffer)
{
    resource = buffer.read_domain_name();
}

void DNSResource::read_unsupported_resource_types(DNSReadBuffer &buffer)
{
    if ((buffer.get_current_value() & 0b11000000) == 0b11000000)
    {
        resource = buffer.read_domain_name();
    }
    else
    {
        for (size_t i = 0; i < data_length; ++i)
        {
            resource += buffer.read_uint8();
        }
    }
}

void DNSResource::read_resource(DNSReadBuffer &buffer)
{
    switch (qtype)
    {
    case 1: // "A"
    {
        read_IPV4_address(buffer);
        break;
    }
    case 5: // "CNAME"
    {
        read_CNAME(buffer);
        break;
    }
    case 28: // "AAAA"
    {
        read_IPV6_address(buffer);
        break;
    }
    default:
    {
        read_unsupported_resource_types(buffer);
    }
    }
}

void DNSResource::write_IPV6_address(DNSWriteBuffer &buffer) const
{
    size_t nibbles_in_ipv8_address = 32;
    size_t start_of_group = 0, end_of_group;
    char delimeter = ':';
    size_t address_group_length = 4;

    int number_of_hex_chars_on_adress = resource.length() - std::count(resource.begin(), resource.end(), delimeter);
    while ((end_of_group = resource.find(delimeter, start_of_group)) != std::string::npos)
    {
        size_t current_address_group_length = end_of_group - start_of_group;
        if (current_address_group_length < address_group_length)
        {
            for (size_t i = 0; i < (address_group_length - current_address_group_length); ++i)
            {
                buffer.write_uint8(0); // add missing leading zeros
                number_of_hex_chars_on_adress++;
            }
        }

        if (end_of_group == start_of_group) // :: compressed zeros
        {
            size_t number_of_compressed_zeros = nibbles_in_ipv8_address - number_of_hex_chars_on_adress;
            for (size_t i = 0; i < number_of_compressed_zeros; ++i)
            {
                buffer.write_uint8(0);
            }
        }
        else
        {
            buffer.write_uint8(static_cast<uint8_t>(std::stoi(resource.substr(start_of_group, address_group_length), nullptr, 10)));
            start_of_group = end_of_group + 1;
        }
    }

    buffer.write_uint8(static_cast<uint8_t>(std::stoi(resource.substr(start_of_group), nullptr, 10)));
}

void DNSResource::write_IPV4_address(DNSWriteBuffer &buffer) const
{
    size_t start = 0, end;
    char delimeter = '.';
    while ((end = resource.find(delimeter, start)) != std::string::npos)
    {
        buffer.write_uint8(static_cast<uint8_t>(std::stoi(resource.substr(start, end - start), nullptr, 10)));
        start = end + 1;
    }

    buffer.write_uint8(static_cast<uint8_t>(std::stoi(resource.substr(start), nullptr, 10)));
}

void DNSResource::write_CNAME(DNSWriteBuffer &buffer) const
{
    buffer.write_domain_name(resource);
}

void DNSResource::write_unsupported_resource_types(DNSWriteBuffer &buffer) const
{
    buffer.write_string(resource);
}

void DNSResource::write_resource(DNSWriteBuffer &buffer) const
{
    switch (qtype)
    {
    case 1: // "A"
    {
        write_IPV4_address(buffer);
        break;
    }
    case 5: // "AAAA"
    {
        write_CNAME(buffer);
        break;
    }
    case 28: // "CNAME"
    {
        write_IPV6_address(buffer);
        break;
    }
    default:
    {
        write_unsupported_resource_types(buffer);
    }
    }
}
