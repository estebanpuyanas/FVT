#include "fvt/init.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fvt {

    /**
     * Initializes a new repository folder in the current directory or at a specified path.
     *
     * @param repo_name The name of the repository to create.
     * @param repo_path Optional parameter specifying the directory where the repository should be created.
     *                  If not provided, the repository will be created in the current directory.
     * @return True if the repository was successfully created, false otherwise.
     */
        bool init(const std::string& repo_name, const std::string& repo_path) {
        try {
            std::filesystem::path target_path = repo_path.empty()
            ? std::filesystem::current_path() / repo_name 
            : std::filesystem::path(repo_path) / repo_name;

            if (!std::filesystem::create_directories(target_path)){
                std::cerr << "Error: failed to create repository at: " << target_path << std::endl;
                return false;
            }

            // Create a .fvt directory inside of the repository to keep track of metadata:
            std::filesystem::path dot_fvt_dir = target_path / ".fvt";
            std::filesystem::create_directories(dot_fvt_dir / "commits");
            std::filesystem::create_directories(dot_fvt_dir / "objects");

            std::ofstream index_file(dot_fvt_dir / "index");

            if (!index_file) {
                std::cerr << "Error: Failed to create index file in repository." << std::endl;
                return false;
            }

            std::ofstream head_file(dot_fvt_dir / "HEAD");

            if (!head_file) {
                std::cerr << "Error: Failed to create HEAD file in repository." << std::endl;
                return false;
            }

            head_file << "null. There are no commits to this repository."; 
            head_file.close();


            std::ofstream config_file(dot_fvt_dir / "config");

            if (!config_file) {
                std::cerr << "Error: Failed to create config file in repository." << std::endl;
                return false;
            }

            config_file << "repo_name=" << repo_name << "\n";
            config_file << "created_at=" << std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) << "\n";
            config_file << "directory=" << target_path.string() << "\n";
           config_file.close();

           std::cout << "Repository initialized at: " << target_path << std::endl;
           return true;
           
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            return false;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }
}