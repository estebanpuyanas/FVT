#include "fvt/command_registry.h"
#include "fvt/init.h"
#include "fvt/commit.h"
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

    void handle_commit(const std::vector<std::string>& args) {

        if (args.size() < 2) {
            throw std::invalid_argument("Usage: fvt commit ['.' for all files or <file1 dir2/ file3 dir4>] \"commit message\" ");
        }

        std::string commit_message = args.back();

        // Sanitize commit message:
        if (commit_message.front() != '"' || commit_message.back() != '"') {
            throw std::invalid_argument("A commit message must be enclosed in double quotes.");
        }
        commit_message = commit_message.substr(1, commit_message.size() - 2); 

        // Extract files/directories to commit:
        std::vector<std::string> files_to_commit(args.begin(), args.end() - 1);

        if (files_to_commit.size() == 1 && files_to_commit[0] == ".") {
            std::cout << "Committing all tracked files..." << std::endl;
            files_to_commit.clear(); 
        } 

        if (!fvt::commit(commit_message, files_to_commit)) {
            throw std::runtime_error("Commit failed.");
        }
    }

    void register_commands(CommandParser& parser) {
        parser.register_command("init", handle_init);
        parser.register_command("commit", handle_commit);
    }
}
