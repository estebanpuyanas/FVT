#include "repository.h"


// Constructor
repository::repository(const std::string &name) : repo_name(name) {}

// Destructor
repository::~repository() {}

// Create a new repository
bool repository::create_repository()
{
    try
    {
        // check if the repo exists
        if (std::filesystem::exists(repo_name))
        {
            std::cerr << REPO_EXISTS_MSG << std::endl;
            return false;
        }

        // create the repo and the commits directory
        std::filesystem::create_directory(repo_name);
        std::filesystem::create_directory(repo_name + "/commits");
        std::cout << REPO_CREATE_MSG << repo_name << std::endl;
        return true;
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        // Catch early to log, but rethrow for higher-level handling
        std::cerr << "[LOG]: Filesystem error while creating repository: " << e.what() << std::endl;
        throw; // Rethrow 
    }
}

// Add a commit to the repository
void repository::add_commit(const commit &commit) noexcept
{
    commit_history.push_back(commit);
}

void repository::show_commit_log()
{
    if (commit_history.empty())
    {
        std::cout << "No commits to show." << std::endl;
        return;
    }

    char buffer[1024];
    // char temp[256];

    for (const auto &c : commit_history)
    {
        // Clear buffer
        memset(buffer, 0, sizeof(buffer));

        // Write commit ID
        strncpy(buffer, "Commit ID: ", sizeof(buffer));
        strncat(buffer, std::to_string(c.get_commit_id()).c_str(), sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);

        // Write timestamp
        strncat(buffer, "Timestamp: ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, c.get_timestamp().c_str(), sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);

        // Write commit message
        strncat(buffer, "Message: ", sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, c.get_commit_message().c_str(), sizeof(buffer) - strlen(buffer) - 1);
        strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);

        // Print buffer
        std::cout << buffer << std::endl;
    }
}

std::string repository::get_repo_name() noexcept
{
    return repo_name;
}

void repository::create_branch(const std::string &branch_name)
{
    if (commit_history.empty())
    {
        std::cout << "No commits in the repository to create a branch." << std::endl;
        return;
    }

    commit* latest_commit = &commit_history.back();

    unsigned int branch_id = branches.size() + 1; // Generate a unique branch ID
    branches.emplace_back(branch(branch_name, latest_commit->get_commit_id(), branch_id, latest_commit));

    std::cout << "Created new branch: " << branch_name << " pointing to commit " << latest_commit->get_commit_id() << std::endl;
    unsigned int latest_commit_id = commit_history.back().get_commit_id();
}

void repository::get_branches()
{
    if (branches.empty())
    {
        std::cout << "No branches in this repository." << std::endl;
        return;
    }

    for (const auto &br : branches)
    {
        std::cout << br << std::endl;
    }
}

void repository::checkout_branch(const std::string &branch_name)
{
    for (const auto &br : branches)
    {
        if (br.get_branch_name() == branch_name)
        {
            std::cout << "Switched to branch: " << branch_name << std::endl;
            return;
        }
    }
    std::cerr << "Error Branch: " << branch_name << " not found." << std::endl;
}

void repository::checkout_commit(const std::string &commit_id)
{
    for (const auto &c : commit_history)
    {
        if (std::to_string(c.get_commit_id()) == commit_id)
        {
            std::cout << "Switched to commit: " << commit_id << std::endl;
            return;
        }
    }
    std::cerr << "Error Commit: " << commit_id << " not found." << std::endl;
}

void repository::update_branch(const std::string& branch_name, const commit& new_commit)
{
    for (auto& br : branches) {
        if (br.get_branch_name() == branch_name) {
            br.update_latest_commit(const_cast<commit*>(&new_commit)); // Update the branch's latest commit
            std::cout << "Branch '" << branch_name << "' updated to point to commit " << new_commit.get_commit_id() << std::endl;
            return;
        }
    }
    std::cerr << "Error: Branch '" << branch_name << "' not found in repository '" << repo_name << "'." << std::endl;
}
const std::vector<commit>& repository::get_commit_history() const {
    return commit_history;
}
