#include "commit.h"
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstring>

const std::string CHANGELOG_GENERATION_ERROR = "Error generating CHANGELOG.txt file."; 

// Constructor definition
commit::commit(unsigned int id, const std::vector<std::string>& files, const std::string& message) 
    : version_control_feature("Commit " + std::to_string(id)), 
    commit_id(id), changed_files(files), commit_message(message) 
{  
    timestamp = create_timestamp(); 
}

// Copy constructor
commit::commit(const commit& other)
    : version_control_feature(other),
    commit_id(other.commit_id),
    timestamp(other.timestamp),
    changed_files(other.changed_files), 
    commit_message(other.commit_message){}

// Destructor definition
commit::~commit() {}

// Getter method definitions
unsigned int commit::get_commit_id() const { return commit_id; }
std::vector<std::string> commit::get_changed_files() const { return changed_files; }
std::string commit::get_timestamp() const { return timestamp; }
std::string commit::get_commit_message() const { return commit_message; }

// Helper function to generate a timestamp
std::string commit::create_timestamp() 
{
    std::time_t current_time = std::time(nullptr); 
    std::tm* local_time = std::localtime(&current_time);
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");  
    return oss.str(); 
}

/**
 *  Overloading operators for == and != for the commit struct to check whether two commits are equal or not using the commit ID, timestamp, and changed files.
 */
 bool operator==(const commit& commit_1, const commit& commit_2)
 {
     std::vector<std::string> c1_files = commit_1.get_changed_files();
     std::vector<std::string> c2_files = commit_2.get_changed_files();
     
     // Sort files since order does not matter (to avoid edge case where c1 == c2 but shown as false negative due to difference in file order)
     std::sort(c1_files.begin(), c1_files.end());
     std::sort(c1_files.begin(), c1_files.end());
 
     return (commit_1.get_commit_id() == commit_2.get_commit_id() &&
     commit_1.get_timestamp() == commit_2.get_timestamp() &&
     c1_files == c2_files && commit_1.get_commit_message() == commit_2.get_commit_message());
 }
 
 bool operator!=(const commit& commit_1, const commit& commit_2)
 {
  return !(commit_1 == commit_2); // Since we established the paramaters for equality with the operator above, use nagation in the inequality operator.   
 }

 void commit::generate_changelog() const
 {
    // Generate the file and write the commit information to it
    std::ofstream changelog("CHANGELOG.txt");
    if(!changelog.is_open()) {
        std::cerr << CHANGELOG_GENERATION_ERROR << std::endl; // Throw an error if the file cannot be opened
        return; 
    }

    // Allocate buffer size and temporary buffer to write to the file. 
    char buffer[1024];
    char temp[256];

    // Write commit ID
    strncpy(buffer, "Commit ID: ", sizeof(buffer));
    strncat(buffer, std::to_string(commit_id).c_str(), sizeof(buffer) - strlen(buffer) - 1);
    strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);

    // Write timestamp
    strncat(buffer, "Timestamp: ", sizeof(buffer) - strlen(buffer) - 1);
    strncat(buffer, timestamp.c_str(), sizeof(buffer) - strlen(buffer) - 1);
    strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);

    // Write changed files
    strncat(buffer, "Changed Files:\n", sizeof(buffer) - strlen(buffer) - 1);
    for (const auto& file : changed_files) {
        strncpy(temp, file.c_str(), sizeof(temp) - 1);
        strncat(temp, "\n", sizeof(temp) - strlen(temp) - 1);
        strncat(buffer, temp, sizeof(buffer) - strlen(buffer) - 1);
    }

    // Write buffer to file and close the file afterwards. 
    changelog << buffer;
    changelog.close();
 }
