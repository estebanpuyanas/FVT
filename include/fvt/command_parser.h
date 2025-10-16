#ifndef FVT_COMMAND_PASER_H
#define FVT_COMMAND_PASER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace fvt {
    
    class CommandParser {
        public: 
        using command_handler = std::function<void (const std::vector<std::string>&)>;

        /**
         * Registers a command with its associated handler.
         * 
         * @param command_name The name of the command to register.
         * @param handler The function to handle the command when invoked.
         */
        void register_command(const std::string& command_name, command_handler handler);

        /**
         * Parses command-line arguments and executes the corresponding command handler.
         * 
         * @param argc The number of command-line arguments.
         * @param argv The array of command-line argument strings.
         */
        void parse_and_execute(int argc, char* argv[]);
    
        private:
        std::unordered_map<std::string, command_handler> command_map_;
    };
}

#endif // FVT_COMMAND_PARSER_H
