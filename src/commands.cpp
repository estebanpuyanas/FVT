#include <iostream>
#include <fstream>
#include <filesystem>

#include "../include/fvt/commands.h"
#include "../include/fvt/repository.h"
#include "../include/fvt/repository_manager.h"
#include "../include/fvt/invalid_command_exception.h"

int command_usage_stats[static_cast<int>(command_stats_type::count)] = {0};

const char *valid_commands[] = {"create", "destroy", "commit", "log", "checkout", "quit"};
const int num_valid_commands = sizeof(valid_commands) / sizeof(valid_commands[0]);

// Global repository manager instance
repository_manager repo_manager;

void update_command_stats(command_stats_type cmd)
{
    command_usage_stats[static_cast<int>(cmd)]++;
}

void display_command_statistics()
{
    std::cout << "Command Usage Statistics:\n";
    std::cout << "Create:    " << command_usage_stats[static_cast<int>(command_stats_type::create)] << "\n";
    std::cout << "Destroy:   " << command_usage_stats[static_cast<int>(command_stats_type::destroy)] << "\n";
    std::cout << "Commit:    " << command_usage_stats[static_cast<int>(command_stats_type::commit)] << "\n";
    std::cout << "Log:       " << command_usage_stats[static_cast<int>(command_stats_type::log)] << "\n";
    std::cout << "Checkout:  " << command_usage_stats[static_cast<int>(command_stats_type::checkout)] << "\n";
    std::cout << "Quit:      " << command_usage_stats[static_cast<int>(command_stats_type::quit)] << "\n";
    std::cout << "Invalid:   " << command_usage_stats[static_cast<int>(command_stats_type::invalid)] << "\n";
}

// Function to search for a command in history
void search_command_in_history(const char history[][256], int count, const std::string &query) {
    std::cout << "Searching for: " << query << " in command history...\n";
    bool found = false;

    for (int i = 0; i < count; ++i) {
        if (strstr(history[i], query.c_str())) {  // checks if query is a substring
            std::cout << "Found: " << history[i] << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "No matching command found in history.\n";
    }
}

// Function to display the last N commands
void display_last_n_commands(const char history[][256], int count, int n) {
    if (n > count) {
        n = count; // Prevent accessing out-of-bounds
    }

    std::cout << "Last " << n << " commands:\n";
    for (int i = count - n; i < count; ++i) {
        std::cout << history[i] << std::endl;
    }
}

const char* find_closest_command(const std::string &input)
{
    for (int i = 0; i < num_valid_commands; ++i)
    {
        if (strncmp(input.c_str(), valid_commands[i], input.length()) == 0)
        {
            return valid_commands[i]; // Return the closest matching command
        }
    }
    return nullptr; // No close match found
}

void initialize_command_history(int initial_capacity)
{
    command_history_capacity = initial_capacity;
    command_history_count = 0;
    command_history = new char*[command_history_capacity];
    for(int i = 0; i < command_history_capacity; i++)
    {
        command_history[i] = nullptr;
    }
}

void free_command_history()
{
    for(int i = 0; i < command_history_count; i++)
    {
        delete[] command_history[i];
    }
    delete[] command_history;
    command_history = nullptr;
    command_history_capacity = 0;
    command_history_count = 0;
}

void resize_command_history(int new_capacity)
{
    char** new_history = new char*[new_capacity];

    for(int i = 0; i < command_history_count; i++)
    {
        new_history[i] = new char[std::strlen(command_history[i]) + 1];
        std::strcpy(new_history[i], command_history[i]);
    }

    // Free the old array
    for(int i = 0; i < command_history_count; i++)
    {
        delete[] command_history[i];
    }
    delete[] command_history;

    command_history = new_history;
    command_history_capacity = new_capacity;
}



void add_command_to_history(const std::string &command) 
{
    if (command_history_count >= command_history_capacity)
    {
        resize_command_history(command_history_capacity * 2);
    }
    command_history[command_history_count] = new char[command.size() + 1];
    std::strcpy(command_history[command_history_count], command.c_str());
    command_history_count++;
}

void display_command_history() 
{
    std::cout << "Command History:\n";
    for(int i = 0; i < command_history_count; ++i)
    {
        if (command_history[i] && std::strlen(command_history[i]) > 0) 
        {
            std::cout << command_history[i] << std::endl;
        }
    }
}

/**
 * @brief Handles the creation command.
 */
void handle_create(const std::string &argument)
{
    add_command_to_history("create " + argument);
    update_command_stats(command_stats_type::create);

    if (argument.empty())
    {
        std::cerr << "Error: Repository name required. Usage: create <name>\n";
        return;
    }
    repository new_repo(argument);
    new_repo.create_repository();
    repo_manager.add_repository(new_repo); // Add repository to manager
}

/**
 * @brief Handles the destroy command.
 */
void handle_destroy(const std::string &argument)
{
    add_command_to_history("destroy " + argument);
    update_command_stats(command_stats_type::destroy);

    if (argument.empty())
    {
        std::cerr << "Error: Repository name required. Usage: destroy <name>\n";
        return;
    }

    if (!std::filesystem::exists(argument))
    {
        std::cerr << "Error: Repository '" << argument << "' does not exist.\n";
        return;
    }

    try
    {
        std::filesystem::remove_all(argument);
        std::cout << "Repository '" << argument << "' deleted successfully.\n";
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Error deleting repository: " << e.what() << std::endl;
    }
}

/**
 * @brief Handles the commit command.
 */
void handle_commit(const std::string &argument)
{
    add_command_to_history("commit " + argument);
    update_command_stats(command_stats_type::commit);

    std::cout << "Committing changes: " << argument << std::endl;
    // TODO: Implement commit logic
}

/**
 * @brief Handles the log command.
 */
void handle_log(const std::string &argument)
{
    add_command_to_history("log " + argument);
    update_command_stats(command_stats_type::log);

    repository* repo = repo_manager.get_repository(argument); // Use repo_manager instance
    if (repo)
    {
        repo->show_commit_log();
    }
    else
    {
        std::cerr << "Error: Repository '" << argument << "' not found.\n";
    }
}

void handle_checkout(const std::string &argument) {
    add_command_to_history("checkout " + argument);
    update_command_stats(command_stats_type::checkout);

    repository* repo = repo_manager.get_repository(argument); // Use pointer to repository
    if (!repo) {
        std::cerr << "Error: Repository '" << argument << "' not found.\n";
        return;
    }

    // Determine if the argument is a branch name or a commit ID
    if (std::isdigit(argument[0])) {
        // Argument is a commit ID
        unsigned int commit_id = std::stoi(argument);
        for (const auto& c : repo->get_commit_history()) {
            if (c.get_commit_id() == commit_id) {
                repo->checkout_commit(std::to_string(commit_id));
                return;
            }
        }
        std::cerr << "Error: Commit ID '" << commit_id << "' not found.\n";
    } else {
        // Argument is a branch name
        repo->checkout_branch(argument);
    }
}

/**
 * @brief Handles the quit command.
 */
void handle_quit(const std::string &)
{
    add_command_to_history("quit");
    update_command_stats(command_stats_type::quit);

    std::cout << "Exiting version control system.\n";
    exit(0);
}

/**
 * @brief Handles invalid commands.
 */
void handle_invalid(const std::string &command)
{
    add_command_to_history("invalid " + command);
    update_command_stats(command_stats_type::invalid);

    const char* suggestion = find_closest_command(command);
    if (suggestion)
    {
        std::cerr << "Command '" << command << "' is not valid. Did you mean '" << suggestion << "'?\n";
    }
    else
    {
        std::cerr << "Command '" << command << "' is not valid. Please try again.\n";
    }
}

/**
 * @brief Handles creating a repository.
 */
void create_repository(const std::string &name)
{
    if (name.empty())
    {
        std::cerr << "Error: Repository name required. Usage: create -repo <name>\n";
        return;
    }
    repository new_repo(name);
    new_repo.create_repository();
    repo_manager.add_repository(new_repo); // Add repository to manager
}

/**
 * @brief Handles creating a file.
 */
void create_file(const std::string &filename)
{
    if (filename.empty())
    {
        std::cerr << "Error: File name required. Usage: create -file <filename>\n";
        return;
    }
    std::ofstream file(filename);
    if (file)
    {
        std::cout << "Created file: " << filename << std::endl;
    }
    else
    {
        std::cerr << "Error creating file.\n";
    }
    file.close();
}

/**
 * @brief Handles creating a directory.
 */
void create_directory(const std::string &dirname)
{
    if (dirname.empty())
    {
        std::cerr << "Error: Directory name required. Usage: create -dir <dirname>\n";
        return;
    }
    if (std::filesystem::create_directory(dirname))
    {
        std::cout << "Created directory: " << dirname << std::endl;
    }
    else
    {
        std::cerr << "Error creating directory.\n";
    }
}

/**
 * @brief Handles invalid create flags.
 */
void handle_invalid_create(const std::string &)
{
    std::cerr << "Invalid flag for create command. Use:\n"
              << "  create -repo <name>\n"
              << "  create -file <filename>\n"
              << "  create -dir <dirname>\n";
}