#ifndef FVT_INIT_H
#define FVT_INIT_H

#include <string>

namespace fvt {
    /**
     * Initializes a new repository folder in the current directory or at a specified path.
     * 
     * @param repo_name The name of the repository to create.
     * @param repo_path Optional parameter specifying the directory where the repository should be created.
     *                  If not provided, the repository will be created in the current directory.
     * @return True if the repository was successfully created, false otherwise.
     */
    bool init(const std::string& repo_name, const std::string& repo_path = "");
}

#endif // FVT_INIT_H