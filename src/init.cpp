#include "fvt/init.h"
#include <iostream>
#include <filesystem>

namespace fvt {

    /**
     * Initializes a new repository folder in the current directory or at a specified path.
     *
     * @param repo_name The name of the repository to create.
     * @param repo_path Optional parameter specifying the directory where the repository should be created.
     *                  If not provided, the repository will be created in the current directory.
     * @return True if the repository was successfully created, false otherwise.
     */
        bool init(const std::string& repo_name, const std::string& repo_path = "") {
        try {
            std::filesystem::path target_path = repo_path.empty()
            ? std::filesystem::current_path() / repo_name 
            : std::filesystem::path(repo_path) / repo_name;

            if(std::filesystem::create_directories(target_path)) {
                std::cout << "Repository initialized at: " << target_path << std::endl;
                return true; 
            } else {
                std::cerr << "Failed to create repository at: " << target_path << " Please make sure the directory is valid and you named the repository." << std::endl;
                return false;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            return false;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }
}