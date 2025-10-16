#include "fvt/command_registry.h"
#include "fvt/init.h"
#include <iostream>
#include <stdexcept>

namespace fvt {

    /**
     * Handles the 'init' command.
     *
     * @param args The arguments passed to the command.
     */
    void handle_init(const std::vector<std::string>& args) {
        if(args.size() < 1 || args.size() > 2) {
            throw std::invalid_argument("Usage: init [repo path] (not passing this argument will create the repo at current directory) [repo name] (must be enclosed in quotes)");
        }

        std::string repo_name = args.size() == 2 ? args[1] : args[0];
        std::string repo_path = args.size() == 2 ? args[0] : "";

        if(!fvt::init(repo_name, repo_path)) {
            throw std::runtime_error("Failed to initialize repository");
        }
    }

    void register_commands(CommandParser& parser) {
        parser.register_command("init", handle_init);
    }
}
