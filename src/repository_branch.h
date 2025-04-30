#ifndef REPOSITORY_BRANCH_H
#define REPOSITORY_BRANCH_H

#include <string>
#include <iostream>
#include "version_control_feature.h"

/**
 * @brief Represents a branch in the repository.
 */
class branch : public version_control_feature 
{
private:
    std::string branch_name;
    // The commit ID this branch is pointing to
    unsigned int commit_id; 
    // Branch needs to point to the commit ID it was created from but also have own unique ID.
    unsigned int branch_id; 
    commit* latest_commit;

public:
    /**
     * @brief Constructs a branch with a name and commit ID.
     * @param name The name of the branch.
     * @param commit_id The ID of the latest commit.
     * @param branch_id The unique ID of the branch.
     * @param latest_commit The latest commit object.
     */
    branch(const std::string& name, unsigned int commit_id, unsigned int branch_id, commit* latest_commit)
        : version_control_feature(name), // Call base class constructor 
        branch_name(name), commit_id(commit_id),
        branch_id(branch_id), latest_commit(latest_commit) {}

    /**
     * @brief Copies a branch.
     * @param other The object to copy.
     */
    branch(const branch& other)
        : version_control_feature(other),  
          branch_name(other.branch_name),
          commit_id(other.commit_id),
          branch_id(other.branch_id), 
        latest_commit(other.latest_commit) {}

    /**
     * @brief Gets the branch name.
     * @return The name of the branch.
     */
    std::string get_branch_name() const {
        return branch_name;
    }

    /**
     * @brief Gets the commit ID of the latest commit on the branch.
     * @return The commit ID.
     */
    unsigned int get_commit_id() const {
        return commit_id;
    }

    /**
     * @brief Gets the branch ID.
     * @return The branch ID.
     */
    unsigned int get_branch_id() const {
        return branch_id;
    }

    /**
     * @brief Gets the latest commit object.
     * @return The latest commit object.
     */
    commit* get_latest_commit() const {
        return latest_commit;
    }

    /**
     * @brief Updates the latest commit object and commit ID for the branch.
     * @param new_commit The new commit object to update to.
     */
    void update_latest_commit(commit* new_commit) {
        if (!new_commit) {
            std::cerr << "Error: Null commit pointer provided." << std::endl;
            return;
        }
        latest_commit = new_commit;
        commit_id = new_commit->get_commit_id(); // Update the commit ID
    }

    /**
     * @brief Overloads the << operator for branch.
     * @param os The output stream.
     * @param br The branch object.
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const branch& br) {
        os << "Branch Name: " << br.get_branch_name() << ", Commit ID: " << br.get_commit_id();
        return os;
    }

    /**
     * @brief Overridden function to display branch info.
     */
    void display_info() const override
    {
        std::cout << "Branch: " << name << ", Latest Commit ID: " << commit_id << std::endl;
    }
};

#endif