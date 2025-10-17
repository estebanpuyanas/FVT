#include "fvt/commit.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h> // For generating commit hashes

namespace fvt {

    // Helper function to generate a SHA-1 hash for the commit
    std::string generate_commit_hash(const std::string& data) {
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

        std::ostringstream hash_stream;
        for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
            hash_stream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        return hash_stream.str();
    }

    bool commit(const std::string& message, const std::vector<std::string>& files) {
        // Check if the repository is valid
        if (!std::filesystem::exists(".fvt")) {
            std::cerr << "Error: No repository found. Please run 'fvt init' first." << std::endl;
            return false;
        }

        // Read the current HEAD to get the parent commit hash
        std::string parent_commit = "null";
        std::ifstream head_file(".fvt/HEAD");
        if (head_file) {
            std::getline(head_file, parent_commit);
            head_file.close();
        }

        // Generate a timestamp for the commit
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::string timestamp = std::ctime(&now_time_t);
        timestamp.pop_back(); // Remove the trailing newline

        // Generate a unique commit hash
        std::string commit_data = message + parent_commit + timestamp;
        std::string commit_hash = generate_commit_hash(commit_data);

        // Create the commit metadata file
        std::filesystem::path commit_file = ".fvt/commits/" + commit_hash;
        std::ofstream commit_out(commit_file);
        if (!commit_out) {
            std::cerr << "Error: Failed to create commit file." << std::endl;
            return false;
        }

        commit_out << "Commit Hash: " << commit_hash << "\n";
        commit_out << "Parent: " << parent_commit << "\n";
        commit_out << "Timestamp: " << timestamp << "\n";
        commit_out << "Message: " << message << "\n";

        // Save the list of files/directories in the commit metadata
        commit_out << "Files:\n";
        for (const auto& file : files) {
            commit_out << "  " << file << "\n";
        }
        commit_out.close();

        // Update the HEAD file to point to the new commit
        std::ofstream head_out(".fvt/HEAD");
        if (!head_out) {
            std::cerr << "Error: Failed to update HEAD file." << std::endl;
            return false;
        }
        head_out << commit_hash;
        head_out.close();

        std::cout << "Commit created successfully with hash: " << commit_hash << std::endl;
        return true;
    }

}