#include "Site.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring> // For size_t


// private methods
std::string Site::encrypt(std::string pass, const std::string& k) {
    if (k.empty()) return pass;
    for (size_t i = 0; i < pass.length(); i++) {
        char keychar = k[i % k.length()];
        pass[i] = keychar ^ pass[i];
    }
    return pass;
}


// public methods
Site::Site() {}

Site::Site(std::string raw_p, std::string u, std::string sn, std::string key)
    : username(u), sitename(sn) {
    password = encrypt(raw_p, key);
    check_phrase = encrypt("valid", key);
}

Site Site::createFromFile(std::string p, std::string u, std::string sn, std::string c) {
    Site s;
    s.password = p;
    s.username = u;
    s.sitename = sn;
    s.check_phrase = c;
    return s;
}

bool Site::isValidKey(std::string k) {
    std::string decrypted_canary = encrypt(check_phrase, k);
    if (decrypted_canary == "valid") {
        return true;
    } else {
        std::cout << "Wrong key buddy" << std::endl;
        return false;
    }
}

void Site::setPass(std::string raw_p, std::string k) {
    password = encrypt(raw_p, k);
}

std::string Site::getPass(std::string key) {
    return encrypt(password, key);
}

std::string Site::getRawPass() {
    return password;
}

void Site::setUser(std::string usr) { username = usr; }
void Site::setSite(std::string sn) { sitename = sn; }
std::string Site::getUser() { return username; }
std::string Site::getSitename() { return sitename; }
std::string Site::getRawCanary() { return check_phrase; }


// Binary file handling
void load_file(std::vector<Site>& v) {
    std::fstream fcout;
    fcout.open("vault.bin", std::ios::binary | std::ios::in);

    if (!fcout) {
        std::cout << "No existing vault found. Starting fresh.\n";
        return;
    }

    size_t count;
    fcout.read((char*)&count, sizeof(count));

    if (fcout) {
        std::cout << "Loading " << count << " sites...\n";
        for (int i = 0; i < count; i++) {
            std::string t_site, t_user, t_pass, t_canary;
            size_t len;

            fcout.read((char*)&len, sizeof(len));
            t_site.resize(len);
            fcout.read(&t_site[0], len);

            fcout.read((char*)&len, sizeof(len));
            t_user.resize(len);
            fcout.read(&t_user[0], len);

            fcout.read((char*)&len, sizeof(len));
            t_pass.resize(len);
            fcout.read(&t_pass[0], len);

            fcout.read((char*)&len, sizeof(len));
            t_canary.resize(len);
            fcout.read(&t_canary[0], len);

            Site s = Site::createFromFile(t_pass, t_user, t_site, t_canary);
            v.push_back(s);
        }
    }
    fcout.close();
}

void save_file(std::vector<Site>& v) {
    std::fstream fcout;
    fcout.open("vault.bin", std::ios::binary | std::ios::out);

    size_t s_vector = v.size();
    fcout.write(reinterpret_cast<char*>(&s_vector), sizeof(s_vector));

    if (fcout) {
        std::cout << "File opened successfully \n";
        for (int i = 0; i < s_vector; i++) {
            std::string f_sitename = v[i].getSitename();
            size_t len = f_sitename.size();
            fcout.write((const char*)&len, sizeof(len));
            fcout.write(f_sitename.data(), len);

            std::string f_username = v[i].getUser();
            size_t lenu = f_username.size();
            fcout.write((const char*)&lenu, sizeof(lenu));
            fcout.write(f_username.data(), lenu);

            std::string f_rawpass = v[i].getRawPass();
            size_t lenr = f_rawpass.size();
            fcout.write((const char*)&lenr, sizeof(lenr));
            fcout.write(f_rawpass.data(), lenr);

            std::string f_carnary = v[i].getRawCanary();
            size_t lenc = f_carnary.size();
            fcout.write((const char*)&lenc, sizeof(lenc));
            fcout.write(f_carnary.data(), lenc);
        }
    } else {
        std::cout << " Error opening the file \n";
    }
    fcout.close();
}
