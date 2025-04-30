#ifndef FILE_UTILS_H
#define FILE_UTILS_H

/**
 * @brief Returns a pointer to the file extension within a given file path.
 * @param file_path A C-string representing the file path.
 * @return A pointer to the file extension (including the dot), or nullptr if no extension is found.
 */
const char* get_file_extension(const char* file_path);

#endif 