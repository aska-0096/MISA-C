/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2021 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/
#ifndef _INPUTFLAG_PARSER_HPP_
#define _INPUTFLAG_PARSER_HPP_

#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>

struct input
{
    std::string long_name;
    char short_name;
    std::string value;
    std::string help_text;
    std::string type;
    bool convert2uppercase;
};

class inputflags
{
    std::map<char, input> map_inputs;

public:
    inputflags()
    {
        // AddInputFlag("help", 'h', "", "print Help Message", "string");
    }

    int add_inputflag(const std::string& _long_name,
                      char _short_name,
                      const std::string& _value,
                      const std::string& _help_text,
                      const std::string& _type,
                      const bool _convert2uppercase = false)
    {

        input in;
        in.long_name         = _long_name;
        in.short_name        = _short_name;
        in.value             = _value;
        in.help_text         = _help_text;
        in.type              = _type;
        in.convert2uppercase = _convert2uppercase;

        if(map_inputs.count(_short_name) > 0)
            printf("Input flag: %s (%c) already exists !", _long_name.c_str(), _short_name);
        else
            map_inputs[_short_name] = in;

        return 0;
    }

    [[gnu::noreturn]] void print() const
    {
        printf("GXCO Driver Input Flags: \n\n");

        for(auto& content : map_inputs)
        {
            std::vector<std::string> help_text_lines;
            size_t pos = 0;
            for(size_t next_pos = content.second.help_text.find('\n', pos);
                next_pos != std::string::npos;)
            {
                help_text_lines.push_back(
                    std::string(content.second.help_text.begin() + pos,
                                content.second.help_text.begin() + next_pos++));
                pos      = next_pos;
                next_pos = content.second.help_text.find('\n', pos);
            }
            help_text_lines.push_back(std::string(content.second.help_text.begin() + pos,
                                                  content.second.help_text.end()));

            std::cout << std::setw(8) << "--" << content.second.long_name
                      << std::setw(20 - content.second.long_name.length()) << "-" << content.first
                      << std::setw(8) << " " << help_text_lines[0] << std::endl;

            for(auto help_next_line = std::next(help_text_lines.begin());
                help_next_line != help_text_lines.end();
                ++help_next_line)
            {
                std::cout << std::setw(37) << " " << *help_next_line << std::endl;
            }
        }
        exit(0); // NOLINT (concurrency-mt-unsafe)
    }

    char find_shortname(const std::string& long_name) const
    {
        char short_name = '\0';
        for(auto& content : map_inputs)
        {
            if(content.second.long_name == long_name)
                short_name = content.first;
        }
        if(short_name == '\0')
        {
            std::cout << "Long Name: " << long_name << " Not Found !";
            exit(0); // NOLINT (concurrency-mt-unsafe)
        }
        return short_name;
    }

    void parse(int argc, char* argv[])
    {
        std::vector<std::string> args;
        for(int i = 1; i < argc; i++)
            args.push_back(argv[i]);

        for(int i = 0; i < argc - 1; i++)
        {
            std::string temp = args[i];
            if(temp[0] != '-')
            {
                printf("Illegal input flag\n");
                print();
            }
            else if(temp[0] == '-' && temp[1] == '-') // Long Name Input
            {
                std::string long_name = temp.substr(2);
                if(long_name == "help")
                    print();
                char short_name = find_shortname(long_name);
                store_optional_flagvalue(short_name, args[i + 1]);
                i++;
            }
            else if(temp[0] == '-' && temp[1] == '?') // Help Input
                print();
            else // Short Name Input
            {
                char short_name = temp[1];
                if(map_inputs.find(short_name) == map_inputs.end())
                {
                    std::cout << "Input Flag: " << short_name << " Not Found !";
                    exit(0); // NOLINT (concurrency-mt-unsafe)
                }
                if(short_name == 'h')
                    print();

                if(i + 1 >= argc - 1) // Check whether last arg has a value
                    print();
                else
                {
                    map_inputs[short_name].value = args[i + 1];
                    i++;
                }
            }
        }
    }

    // This function updates the input flag parameters values.Depending on the flag setting,
    // input values are converted to uppercase & stored into map.This is used while
    // parsing the driver arguments.
    void store_optional_flagvalue(char short_name, const std::string input_value)
    {
        if(map_inputs[short_name].convert2uppercase == true)
        {
            std::string tvalue = input_value;
            std::transform(tvalue.begin(), tvalue.end(), tvalue.begin(), ::toupper);
            map_inputs[short_name].value = tvalue;
        }
        else
        {
            map_inputs[short_name].value = input_value;
        }
    }

    std::map<char, input> get_inputmap() const { return map_inputs; }
    std::string get_valuestr(const std::string& long_name) const
    {
        char short_name   = find_shortname(long_name);
        std::string value = map_inputs.at(short_name).value;

        return value;
    }

    int get_valueint(const std::string& long_name) const
    {
        char short_name = find_shortname(long_name);
        int value       = atoi(map_inputs.at(short_name).value.c_str());

        return value;
    }

    uint64_t get_valueuint64(const std::string& long_name) const
    {
        char short_name = find_shortname(long_name);
        uint64_t value  = strtoull(map_inputs.at(short_name).value.c_str(), nullptr, 10);

        return value;
    }

    double get_valuedouble(const std::string& long_name) const
    {
        char short_name = find_shortname(long_name);
        double value    = atof(map_inputs.at(short_name).value.c_str());

        return value;
    }

    void set_value(const std::string& long_name, const std::string& new_value)
    {
        char short_name                 = find_shortname(long_name);
        map_inputs.at(short_name).value = new_value;
    }

    virtual ~inputflags() {}
};

#endif //_MISA_C_INPUTFLAG_PARSER_HPP_
