// Define headers.
#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include "invalid_command_exception.h"
#include <cstring>

// Function type for command handlers
using CommandFunction = std::function<void(const std::string&)>;

using CreateFunction = std::function<void(const std::string&)>;

// Command handler functions
void handle_create(const std::string& argument);
void handle_destroy(const std::string& argument);
void handle_commit(const std::string& argument);
void handle_log(const std::string& argument);
void handle_checkout(const std::string& argument);
void handle_quit(const std::string& argument);
void handle_invalid(const std::string& argument);

// Create subcommand handlers
void create_repository(const std::string& name);
void create_file(const std::string& filename);
void create_directory(const std::string& dirname);
void handle_invalid_create(const std::string& argument);

/**
 * Enum class containing all the valid commands the user can input. All commands mirror their corresponding git commands.
 * Defined as header file so it can be reused by any file that needs command parsing functionality. Also improves compilation performance.
 * TODO: Add more flags.
 * Also, might want to implement mechanism to disregard case sensitivity.
 */

enum class command_type
{
   create,
   destroy, // cannot use delete as is reserved keyword for C++
   commit,
   log,
   checkout,
   quit,
   clear,
   invalid
};

enum class flag_type
{
   repo,
   file,
   dir,
   none,
   invalid
};

enum class command_stats_type
{
   create,
   destroy,
   commit,
   log,
   checkout,
   quit,
   clear,
   invalid,
   count
};

// Array to store the count of each command type
extern int command_usage_stats[static_cast<int>(command_stats_type::count)];

// Function to update command statistics
void update_command_stats(command_stats_type cmd);

// Function to display command statistics
void display_command_statistics();

void search_command_in_history(const char history[][256], int count, const std::string &query);
void display_last_n_commands(const char history[][256], int count, int n);

const char* find_closest_command(const std::string &input);

// Command storage feature using 1D array. 
extern char** command_history;
extern int command_history_capacity;
extern int command_history_count;

// Initializes the command history with an initial capacity
void initialize_command_history(int initial_capacity = 10);

// Frees the dynamic array
void free_command_history();

// Dynamically resizes the command history array
void resize_command_history(int new_capacity);

/**
 * @brief adds this command to the command history array. 
 */
void add_command_to_history(const std::string &command);

/**
 * @brief displays all the commands in the command history array. 
 */
void display_command_history();

// Helper function to parse command from user input. Use inline to avoid multiple definition error.
inline command_type parse_command(const std::string &input)
{
   std::istringstream iss(input);
   std::string command;
   iss >> command; // Extract first word

   if (command == "create") 
      return command_type::create;
   if (command == "delete") 
      return command_type::destroy;
   if (command == "commit") 
      return command_type::commit;
   if (command == "log") 
      return command_type::log;
   if (command == "checkout") 
      return command_type::checkout;
   if (command == "quit") 
      return command_type::quit;
   if (command == "clear") 
      return command_type::clear;

   throw invalid_command_exception("Invalid command: " + command);
}

// Function to parse the flag separately from user input.
inline flag_type parse_flag(const std::string &input) 
{
   char temp[256];
   strncpy(temp, input.c_str(), sizeof(temp));
   temp[sizeof(temp) - 1] = '\0';

   char *token = strtok(temp, " "); // Command
   if (!token) return flag_type::none;

   token = strtok(NULL, " "); // Flag (second token)
   if (!token) return flag_type::none;

   if (strcmp(token, "-repo") == 0) 
       return flag_type::repo;
   if (strcmp(token, "-file") == 0) 
       return flag_type::file;
   if (strcmp(token, "-dir") == 0) 
       return flag_type::dir;

   return flag_type::invalid;
}

// Function to extract the argument (e.g., file or repo name)
inline std::string get_argument(const std::string &input)
{
   std::istringstream iss(input);
   std::vector<std::string> tokens;
   std::string token;

   while (iss >> token)
   {
      tokens.push_back(token);
   }

   // The argument should be the third word
   if (tokens.size() < 3)
      return "";

   return tokens[2];
}

#endif