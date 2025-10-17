#include "fvt/commit.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h> 
#include <zlib.h>

namespace fvt {

    // Helper function to generate a SHA-256 hash for the commit
    std::string generate_commit_hash(const std::string& data) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

        std::ostringstream hash_stream;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            hash_stream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        return hash_stream.str();
    }

    bool compress_files(const std::filesystem::path& source, const std::filesystem::path& destination) {

        std::ifstream input(source, std::ios::binary);
        if (!input) {
            std::cerr << "Error: Unable to open source file for compression." << std::endl;
            return false;
        }

        std::ofstream output(destination, std::ios::binary);
        if (!output) {
            std::cerr << "Error: Unable to open destination file for compression." << std::endl;
            return false;
        }

        // Declare and initialize zlib stream
        z_stream zs{};
        
        /**
         * zalloc = Allocate memory.
         * zfree = Free memory.
         * opaque = Application-defined pointer.
         */
        zs.zalloc = Z_NULL;
        zs.zfree = Z_NULL;
        zs.opaque = Z_NULL;

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            std::cerr << "Error: deflateInit failed." << std::endl;
            return false;
        }

        char in_buffer[4096];
        char out_buffer[4096];

        int flush;

        do {
            // Read data from input file
            input.read(in_buffer, sizeof(in_buffer));

            // Set the input data for zlib
            zs.avail_in = input.gcount();
            zs.next_in = reinterpret_cast<unsigned char*>(in_buffer);

            // Compress the data
            flush = input.eof() ? Z_FINISH : Z_NO_FLUSH;

            do {

                // Set the output buffer for zlib
                zs.avail_out = sizeof(out_buffer);
                zs.next_out = reinterpret_cast<unsigned char*>(out_buffer);

                // Perform compression
                deflate(&zs, flush);
                output.write(out_buffer, sizeof(out_buffer) - zs.avail_out);
            } while (zs.avail_out == 0); // While output buffer is full
        } while (flush != Z_FINISH); // While not finished

        deflateEnd(&zs);
        return true;

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
        std::filesystem::path commit_folder = ".fvt/commits/" + commit_hash;
        std::filesystem::create_directories(commit_folder);
        std::ofstream commit_metadata(commit_folder / "metadata.txt");
        
        if (!commit_metadata) {
            std::cerr << "Error: Failed to create commit metadatafile." << std::endl;
            return false;
        }

        commit_metadata << "Commit Hash: " << commit_hash << "\n";
        commit_metadata << "Parent: " << parent_commit << "\n";
        commit_metadata << "Timestamp: " << timestamp << "\n";
        commit_metadata << "Message: " << message << "\n";
        commit_metadata.close();

        for (const auto& file : files) {
            std::filesystem::path source(file);

            if (!std::filesystem::exists(source)) {
                std::cerr << "Error: File " << file << " not found." << std::endl;
                continue;
            }



            std::filesystem::path dest_path = commit_folder / (source.filename().string() + ".gz");

            if (!compress_files(source, dest_path)) {
                std::cerr << "Error: Failed to compress file " << file << std::endl;
                return false;
            }
        }

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