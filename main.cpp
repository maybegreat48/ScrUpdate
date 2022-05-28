// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "common.hpp"
#include <iostream>
#include "src/program.hpp"
#include "src/generator.hpp"
#include "src/parser.hpp"

void help(const char* program_name)
{
    std::cout << "Usage: " << program_name << " -r <scriptfile> -d <scriptdir>" << std::endl
              << "       " << program_name << " -g <scriptfile> -i <index> [-o <offset>] [-l|--local]" << std::endl;
}

int main(int argc, char** argv)
{
    upd::input_parser parser(argc, argv);

    if (parser.cmd_option_exists("-h") || parser.cmd_option_exists("--help"))
    {
        help(argv[0]);
        return 1;
    }

    if (parser.cmd_option_exists("-g"))
    {
        uint32_t index;
        uint32_t offset;
        bool local;
        if (!parser.cmd_option_exists("-i"))
        {
            help(argv[0]);
            return 1;
        }
        index = std::atoi(parser.get_cmd_option("-i").c_str());
        offset = parser.cmd_option_exists("-o") ? std::atoi(parser.get_cmd_option("-o").c_str()) : 0;
        local = parser.cmd_option_exists("-l") || parser.cmd_option_exists("--local");

        upd::generator generator(parser.get_cmd_option("-g"), index, offset, local);
        generator.run();
    }
    else if (parser.cmd_option_exists("-r"))
    {
        if (!parser.cmd_option_exists("-d"))
        {
            help(argv[0]);
            return 1;
        }

        upd::program program(parser.get_cmd_option("-r"), parser.get_cmd_option("-d"));
        program.run();
    }
    else
    {
        help(argv[0]);
    }
}