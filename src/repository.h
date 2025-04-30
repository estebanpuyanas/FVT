#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <iostream>
#include <filesystem>
#include <vector>
#include <cstring>
#include "repository_branch.h" // To be able to create branches.
#include "commit.h" // To be able to create data structures with commit objects. 


/**
 *  Class for handling repository operations: Creating repos, tracking commits, storing commits, etc. (update/modifiy as needed).
 * TODO: Maybe add function to destroy repository or go to a specific commit.
 *  
 */

 /**
 * @brief Represents a repository in the version control system.
 */
class repository {
private:
    std::string repo_name;
    std::vector<commit> commit_history;  
    std::vector<branch> branches;

    // Messages:
    const std::string REPO_EXISTS_MSG = "This repository already exists. Choose a different name or delete the existing repository.";
    const std::string REPO_CREATE_MSG = "Successfully created repository: ";

public:
    /**
     * @brief Constructs a repository with a given name.
     * @param name The name of the repository.
     */    
    repository(const std::string& name);

    /**
     * @brief Destructor.
     */  
    ~repository();

    /**
     * @brief Creates the repository.
     */    
    bool create_repository();

    /**
     * @brief Adds a commit to the repository.
     * @param commit The commit to be added.
     */
    void add_commit(const commit& commit) noexcept;

    /**
     * @brief Displays the commit log.
     */
    void show_commit_log();

    /**
     * @brief Gets the name of the repository.
     * @return The name of the repository.
     */
    std::string get_repo_name() noexcept;

    /**
     * @brief Creates a branch.
     * @param commit The branch name.
     */    
    void create_branch(const std::string& branch_name);

    /**
     * @brief Updates a branch with a new commit.
     * @param branch_name The branch name.
     * @param commit The commit to update the branch with.
     */
    void update_branch(const std::string& branch_name, const commit& commit);

    /**
     * @brief Gets the branches of the repository.
     */
    void get_branches(); 

    /**
     * @brief Checks out a branch.
     * @param branch_name The branch name.
     */
    void checkout_branch(const std::string& branch_name);

    /**
     * @brief Checks out a commit.
     * @param commit_id The commit id.
     */
    void checkout_commit(const std::string& commit_id);

    /**
     * @brief Gets the commit history of the repository.
     * @return A const reference to the vector of commits.
     */
    const std::vector<commit>& get_commit_history() const;

};

#endif 
