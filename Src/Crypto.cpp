#include "../Header/Crypto.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <cstring>

const uint32_t DELTA = 0x9E3779B9 ;  // Golden number in hex
const int BLOCK_SIZE_BYTES = 8 ;


void TEA_encrypt(uint32_t* v , const uint32_t* k){
    uint32_t v0 = v[0];
    uint32_t v1 = v[1];
    uint32_t sum = 0;

    for(size_t i = 0; i < 32 ; i++){
        sum += DELTA;
        v0 += ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        v1 += ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);
    }

    v[0] =v0;
    v[1] = v1;
}

void TEA_decrypt(uint32_t* v, const uint32_t* k) {
    uint32_t v0 = v[0], v1 = v[1];

    uint32_t sum = DELTA << 5; // multiplying with 32 coz we did 32 cycles so

    for (size_t i = 0; i < 32; i++) {
        v1 -= ((v0 << 4) + k[2]) ^ (v0 + sum) ^ ((v0 >> 5) + k[3]);

        v0 -= ((v1 << 4) + k[0]) ^ (v1 + sum) ^ ((v1 >> 5) + k[1]);
        sum -= DELTA;
    }
    v[0] = v0;
    v[1] = v1;
}

// fixing key length coz user can add longer or shorter keys
void make_key128(const std::string& key_str, uint32_t k[4]) {
    std::memset(k, 0, 4 * sizeof(uint32_t));

    size_t len = key_str.size() > 16 ? 16 : key_str.size();
    std::memcpy(k, key_str.data(), len);
}

// adding padding
void add_padding(std::vector<uint8_t>& data){
    size_t length = data.size();

    uint8_t padding_needed = BLOCK_SIZE_BYTES - (length%BLOCK_SIZE_BYTES);

    for(size_t i = 0; i < padding_needed ; i++){
        data.emplace_back(padding_needed);
    }
}

// unpadding
bool un_padding(std::vector<uint8_t>& data){
    if (data.empty()) return false;
    uint8_t padding_length = data.back();

    if(padding_length == 0 || padding_length > BLOCK_SIZE_BYTES){
        return false;
    }

    if (data.size() < padding_length){
        return false;
    }


    for(size_t i = 0 ; i < padding_length ; i++){
        if(data[data.size() - 1 -i] != padding_length){
            return false;
        }
    }

    data.resize(data.size() - padding_length);

    return true;
}


// encryption
std::string encrypt_data(const std::string& plaintext, const std::string& key_str){
    std::vector<uint8_t> data (plaintext.begin() , plaintext.end());

    uint32_t key[4];
    make_key128(key_str, key);

    add_padding(data);
    uint32_t* block_ptr = reinterpret_cast<uint32_t*>(data.data());

    size_t num_blocks = data.size()/BLOCK_SIZE_BYTES;

    for (size_t i = 0; i < num_blocks; i++){
        TEA_encrypt(&block_ptr[i*2], key);
    }

    return std::string(data.begin() , data.end());
}

// decryption

std::string decrypt_data(const std::string &ciphertext, const std::string &key_str){

    std::vector<uint8_t> data (ciphertext.begin(), ciphertext.end());

    uint32_t key[4];
    make_key128(key_str, key);
    uint32_t* block_ptr = reinterpret_cast<uint32_t*>(data.data());
    size_t num_blocks = data.size() / BLOCK_SIZE_BYTES;

    for (size_t i = 0; i < num_blocks; i++) {
        TEA_decrypt(&block_ptr[i * 2], key);
    }

   // removing padding
   if (!un_padding(data)) {
       return ""; //
   }
   return std::string(data.begin(), data.end());
}
