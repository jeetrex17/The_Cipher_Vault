#pragma once

#include <vector>
#include <string>
#include <cstdint>

std::string encrypt_data(const std::string& plaintext, const std::string& key_str);
std::string decrypt_data(const std::string& ciphertext, const std::string& key_str);
void TEA_encrypt(uint32_t* v , const uint32_t* k);
void TEA_decrypt(uint32_t* v, const uint32_t* k);