#include "fvt/command_parser.h"
#include "fvt/command_registry.h"
#include <iostream>

int main(int argc, char* argv[]) {
    fvt::CommandParser parser;

    fvt::register_commands(parser);

    try {
        parser.parse_and_execute(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}