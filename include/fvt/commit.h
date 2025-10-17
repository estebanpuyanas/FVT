#ifndef FVT_COMMIT_H
#define FVT_COMMIT_H

#include <string>
#include <vector>

namespace fvt {
    /**
     * Creates a new commit in the repository.
     *
     * @param message The commit message describing the changes.
     * @param files The list of files/directories to include in the commit. If empty, commit all files.
     * @return True if the commit was successfully created, false otherwise.
     */
    bool commit(const std::string& message, const std::vector<std::string>& files = {});
}

#endif // FVT_COMMIT_H