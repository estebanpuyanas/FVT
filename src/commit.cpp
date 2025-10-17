#include "fvt/commit.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <openssl/evp.h> 
#include <zlib.h>
#include <set>

namespace fvt {

    /**
     * Generates a SHA-256 hash for a file.
     * 
     * @param path The path to the file to hash.
     * 
     */
    std::string hash_file(const std::filesystem::path& path) {
        std::ifstream file(path, std::ios::binary); 

        if (!file) {
            return "";
        }

        EVP_MD_CTX* ctx = EVP_MD_CTX_new(); 
        EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);

        char buffer[4096];

        while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
            EVP_DigestUpdate(ctx, buffer, file.gcount());
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hash_length = 0;
        EVP_DigestFinal_ex(ctx, hash, &hash_length);
        EVP_MD_CTX_free(ctx);

        std::ostringstream hash_stream;
        for (unsigned int i = 0; i < hash_length; ++i) {
            hash_stream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        return hash_stream.str();
    }

    /**
     * Generates a unique commit hash based on commit data.
     * 
     * @param data The data to hash for the commit.
     */
    std::string generate_commit_hash(const std::string& data) {
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
        EVP_DigestUpdate(ctx, data.c_str(), data.size());

        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hash_length;
        EVP_DigestFinal_ex(ctx, hash, &hash_length);
        EVP_MD_CTX_free(ctx);

        std::ostringstream hash_stream;
        for (unsigned int i = 0; i < hash_length; ++i) {
            hash_stream << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        return hash_stream.str();
    }

    /**
     * Compresses a file using zlib and writes to destination.
     * 
     * @param source The source file path.
     * @param destination The destination file path.
     * @return True if compression was successful, false otherwise.
     */
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

    std::set<std::string> get_repo_files() {
        std::set<std::string> files;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(".")) {
            if (entry.is_regular_file()) {
                std::string path = entry.path().string();

                if (path.find(".fvt") == std::string::npos) {
                    files.insert(path.substr(2));
                }
            }
        }

        return files;
    }

    /**
     * Creates a new commit in the repository with the specified message and files.
     * 
     * @param message The commit message.
     * @param files The list of files to include in the commit.
     * @return True if the commit was successful, false otherwise.
     */
    bool commit(const std::string& message, const std::vector<std::string>& files) {
        // Check if the repository is valid
        if (!std::filesystem::exists(".fvt")) {
            std::cerr << "Error: No repository found. Please run 'fvt init' first." << std::endl;
            return false;
        }

        std::set<std::string> files_to_commit;
        if (files.empty()) {
            files_to_commit = get_repo_files();
        } else {
            for (const auto& file : files) {
                files_to_commit.insert(file);
            }
        }

        // Read previous index if it exists:
        std::set<std::string> prev_index;
        std::ifstream index_in(".fvt/index");
        std::string line;
        while (std::getline(index_in, line)) {
            prev_index.insert(line);
        }
        index_in.close();

        // Detect deleted files
        std::set<std::string> deleted_files;
        for (const auto& f : prev_index) {
            if (!std::filesystem::exists(f)) deleted_files.insert(f);
        }

        // Write new index
        std::ofstream idx_out(".fvt/index");
        for (const auto& f : files_to_commit) idx_out << f << "\n";
        idx_out.close();

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
        
        // Store compressed files in objects and in commit folder
        for (const auto& file : files_to_commit) {
            std::filesystem::path source(file);
            if (!std::filesystem::exists(source)) continue;
            std::string file_hash = hash_file(source);
            std::filesystem::path obj_path = ".fvt/objects/" + file_hash + ".gz";
            std::filesystem::create_directories(".fvt/objects");
            
            if (!std::filesystem::exists(obj_path)) {
                compress_files(source, obj_path);
            }
            // Copy compressed file to commit folder
            std::filesystem::copy_file(obj_path, commit_folder / (source.filename().string() + ".gz"),
                                      std::filesystem::copy_options::overwrite_existing);
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