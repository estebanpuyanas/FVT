#include "fvt/commit.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

namespace fvt {

    /**
     * Helper function to generate a SHA-256 hash for the commit.
     * 
     */
    std::string generate_commit_hash(const std::string& data) {

        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

        std::ostringstream hash_stream;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            hash_stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }
        return hash_stream.str();
    }

    bool commit(std::string& message) {

        if (!std::filesystem::exists(".fvt")) {
            std::cerr << "Error: No repository found in the current directory." << std::endl;
            return false;
        }

        // Read current HEAD and get parent commit hash:
        std::string parent_commit = "null";
        std::ifstream head_file(".fvt/HEAD");
        if (head_file) {
            std::getline(head_file, parent_commit);
            head_file.close();
        }

        // Commit timestamp:
        auto now = std::chrono::system_clock::now();
        std::time_t commit_time = std::chrono::system_clock::to_time_t(now);
        std::string string_timestamp = std::ctime(&commit_time);
        string_timestamp.pop_back(); // Remove trailing newline

        std::string commit_data = message + parent_commit + string_timestamp;
        std::string commit_hash = generate_commit_hash(commit_data);

        // Create commit meatadata file:
        std::filesystem::path commit_file_path = ".fvt/commits/" + commit_hash;
        std::ofstream commit_file(commit_file_path);

        if (!commit_file) {
            std::cerr << "Error: Failed to create commit file." << std::endl;
            return false;
        }

        commit_file << "commit_hash=" << commit_hash << "\n";
        commit_file << "parent_commit=" << parent_commit << "\n";
        commit_file << "timestamp=" << string_timestamp << "\n";
        commit_file << "message=" << message << "\n";
        commit_file.close();
    }

    std::ofstream head_file(".fvt/HEAD");
    if (!head_file) {
        std::cerr << "Error: Failed to update HEAD file." << std::endl;
        return false;
    }

    head_file << commit_hash << "\n";
    head_file.close();

    return true;
}