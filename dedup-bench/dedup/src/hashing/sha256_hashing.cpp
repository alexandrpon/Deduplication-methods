#include "sha256_hashing.hpp"
#include "hash.hpp"
#include <string>
#include <openssl/sha.h>

void SHA256_Hashing::hash_chunk(File_Chunk& file_chunk) {
    file_chunk.init_hash(HashingTech::SHA256, SHA256_DIGEST_LENGTH);

    SHA256((const unsigned char*)file_chunk.get_data(), file_chunk.get_size(), file_chunk.get_hash());
    return;
}