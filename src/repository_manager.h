#ifndef REPOSITORY_MANAGER_H
#define REPOSITORY_MANAGER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include "repository.h"

class repository_manager {
    private:
        std::vector<repository> repositories;
        commit* latest_commit = nullptr; // Pointer to the latest commit
        std::string commit_id; // Stores the latest commit ID
    
    public:
        repository_manager() {}
        
        ~repository_manager() {
            if (latest_commit) {
                delete latest_commit; // Free the memory
                latest_commit = nullptr; // Avoid dangling pointer
            }
        }
    
        void add_repository(const repository& repo) {
            repositories.push_back(repo);
        }
    
        repository* get_repository(const std::string& repo_name) {
            for (auto& repo : repositories) {
                if (repo_name == repo.get_repo_name()) {
                    return &repo;
                }
            }
            return nullptr; // Repository not found
        }
    
        void create_branch_in_repository(const std::string& repo_name, const std::string& branch_name) {
            repository* repo = get_repository(repo_name);
            if (repo) {
                repo->create_branch(branch_name);
            } else {
                std::cout << "Repository not found!" << std::endl;
            }
        }

        void update_branch_commit(const std::string& repo_name, const std::string& branch_name, commit* new_commit); 

        void update_latest_commit(commit* new_commit)
        {
            if (!new_commit) 
            {
                std::cerr << "Error: Null commit pointer provided." << std::endl;
                return;
            }
            latest_commit = new_commit;
            commit_id = new_commit->get_commit_id(); // Update the commit ID
    }
       
    };
#endif