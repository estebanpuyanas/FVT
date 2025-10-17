#ifndef FVT_COMMIT_H
#define FVT_COMMIT_H

#include <string>

namespace fvt {

    /**
     * Creates a new commit in the repository with the given commit message.
     *
     * @param message The commit message.
     * @return True if the commit was successful, false otherwise.
     */
    bool commit(const std::string& message);
}


#endif // FVT_COMMIT_H