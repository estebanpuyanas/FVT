#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <filesystem>

#include "../include/fvt/version_control_feature.h" 
#include "../include/fvt/commit.h"
#include "../include/fvt/repository.h"
#include "../include/fvt/commands.h"

/*
 * @brief Function that accepts a base class pointer and calls the overridden function.
 */
void print_entity_info(const version_control_feature &entity)
{
    entity.display_info();
}

// Map commands to their respective functions
std::unordered_map<std::string, CommandFunction> command_map = {
    {"create", handle_create},
    {"destroy", handle_destroy},
    {"commit", handle_commit},
    {"log", handle_log},
    {"checkout", handle_checkout},
    {"quit", handle_quit}};

// Function map for create subcommands
std::unordered_map<flag_type, CreateFunction> create_map = {
    {flag_type::repo, create_repository},
    {flag_type::file, create_file},
    {flag_type::dir, create_directory}};

int main()
{
    std::string user_input;
    bool running = true;
    const std::string START_MSG = "Welcome to the version control system. Type a command to get started.";
    const std::string INVALID_CMD_MSG = "Invalid command, please try again.";

    std::cout << START_MSG << std::endl;

    while (running)
    {
        std::getline(std::cin, user_input); // Get user input

        try
        {
            command_type command = parse_command(user_input);
            flag_type flag = parse_flag(user_input);
            std::string argument = get_argument(user_input);

            // Debugging prints
            /*
            std::cout << "[DEBUG] Command: " << static_cast<int>(command) << "\n";
            std::cout << "[DEBUG] Flag: " << static_cast<int>(flag) << "\n";
            std::cout << "[DEBUG] Argument: " << argument << "\n";
            */

            // Handle quit separately
            if (command == command_type::quit)
            {
                running = false;
                break;
            }

            // If command is 'create', use flag
            if (command == command_type::create)
            {
                auto it = create_map.find(flag);
                if (it != create_map.end())
                {
                    it->second(argument); // Execute corresponding create function
                }
                else
                {
                    throw std::invalid_argument("Invalid create flag provided.");
                }
            }
            else
            {
                // Execute mapped command if it exists
                auto it = command_map.find(user_input);
                if (it != command_map.end())
                {
                    it->second(argument);
                }
                else
                {
                    throw std::invalid_argument("Invalid command entered.");
                }
            }
        }
        catch (const std::invalid_argument &e)
        {
            // Handle invalid commands
            std::cerr << "[ERROR] Invalid Input: " << e.what() << std::endl;
        }
        catch (const std::out_of_range &e)
        {
            // Handle out-of-range errors when parsing command input
            std::cerr << "[ERROR] Out of Range: " << e.what() << std::endl;
        }
        catch (const std::bad_alloc &e)
        {
            // Handle memory allocation failures
            std::cerr << "[ERROR] Memory Allocation Failed: " << e.what() << std::endl;
        }
        catch (const std::ios_base::failure &e)
        {
            // Handle input/output stream failures
            std::cerr << "[ERROR] Input/Output Error: " << e.what() << std::endl;
        }
        catch (const std::exception &e)
        {
            // Generic catch-all for other standard exceptions
            std::cerr << "[ERROR] Unexpected Exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            // Catch-all for unknown/non-standard exceptions
            std::cerr << "[ERROR] An unknown error occurred." << std::endl;
        }
    }

    return 0;
}