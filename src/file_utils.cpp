#include <cstring>

#include "../include/fvt/file_utils.h"

/**
 * @brief Returns a pointer to the file extension within a given file path.
 */
const char* get_file_extension(const char* file_path)
{
    if (!file_path)
    {
        return nullptr;
    }

    const char* dot = strrchr(file_path, '.');

    if (!dot || dot == file_path)
    {
        return nullptr;
    }

    return dot;
}