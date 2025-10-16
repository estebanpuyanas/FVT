#include "fvt/command_parser.h"
#include <iostream>
#include <stdexcept>

namespace fvt {

    /**
     * Registers a command with its associated handler.
     *
     * @param command The name of the command to register.
     * @param handler The function to handle the command when invoked.
     */
    void CommandParser::register_command(const std::string& command, CommandParser::command_handler handler) {
        command_map_[command] = handler;
    }

    /**
     * Parses command-line arguments and executes the corresponding command handler.
     *
     * @param argc The number of command-line arguments.
     * @param argv The array of command-line arguments.
     */
    void CommandParser::parse_and_execute(int argc, char* argv[]) {
        if (argc < 2) {
            std::cerr << "Error: No command provided. Use 'fvt help' for usage information." << std::endl;
            return;
        }

        // Extract the command from the arguments
        std::string command = argv[1];

        // Check if the command is registered
        if (command_map_.find(command) == command_map_.end()) {
            std::cerr << "Error: Unknown command '" << command << "'. Use 'fvt help' for usage information." << std::endl;
            return;
        }

        // Collect the arguments for the command
        std::vector<std::string> args(argv + 2, argv + argc);

        // Execute the command handler
        try {
            command_map_[command](args);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

}