#ifndef COMMIT_H
#define COMMIT_H

#include "version_control_feature.h"
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

/**
 * @brief The commit will (should) contain essential metadata and info about the change history.
 * Could include: timestamp, list of changed files, commit ID, etc. (Keep adding/change as needed). 
 * 
 * TODO:  Might want to consider using hashing structures for commit ID? 
 * Also might want to consider mechanism to compress commits to save space. 
 */
struct commit : public version_control_feature
{

// Make these private as they are not meant to be accessible or changed by other components of the code/users.
private:
    unsigned int commit_id;
    std::string timestamp;
    std::vector<std::string> changed_files;
    std::string commit_message;

public:  
    /**
     * @brief Constructs a commit with a given ID and list of changed files.
     * @param id The unique ID of the commit.
     * @param files The list of files changed in this commit.
     */
    commit(unsigned int id, const std::vector<std::string>& files, const std::string& message); 

    /**
     * @brief Copies a commit.
     * @param other Object to copy.
     */
    commit(const commit& other);
    
    /**
     * @brief Destroys the commit object.
     */
    ~commit();

    /**
     * @brief Gets the commit ID.
     * @return The unique commit ID.
     */
    unsigned int get_commit_id() const;

    /**
     * @brief Gets the list of changed files.
     * @return A vector of strings representing the changed files.
     */
    std::vector<std::string> get_changed_files() const;

    /**
     * @brief Gets the timestamp of the commit.
     * @return A string representing the timestamp.
     */
    std::string get_timestamp() const;

    /**
     * @brief Gets the message of this commit.
     */
    std::string get_commit_message() const;

    /**
     * @brief Compares two commits for equality.
     * @param commit_1 First commit.
     * @param commit_2 Second commit.
     * @return `true` if both commits are equal, otherwise `false`.
     */
    friend bool operator==(const commit& commit_1, const commit& commit_2);

    /**
     * @brief Compares two commits for inequality.
     * @param commit_1 First commit.
     * @param commit_2 Second commit.
     * @return `true` if the commits are different, otherwise `false`.
     */
    friend bool operator!=(const commit& commit_1, const commit& commit_2);

    /**
     * @brief Overridden function to display commit info.
     */
    void display_info() const override 
    {
        std::cout << "Commit ID: " << commit_id << "\nFiles changed: ";
        for (const auto& file : changed_files) 
        {
            std::cout << file << " ";
        }
        std::cout << std::endl;
    }

    /**
     * @brief Function to generate a CHANGELOG.txt file listing all changed files in the commit.  
     */
    void generate_changelog() const; 

private:
    // Helper function to generate a timestamp
    std::string create_timestamp(); 
};

#endif