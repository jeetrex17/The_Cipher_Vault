#ifndef SITE_H
#define SITE_H

#include <string>
#include <vector>

class Site {
private:
    std::string password;
    std::string check_phrase;
    std::string encrypt(std::string pass, const std::string& k);

public:
    std::string username;
    std::string sitename;

    // Constructors
    Site();
    Site(std::string raw_p, std::string u, std::string sn, std::string key);

    // Static Helper
    static Site createFromFile(std::string p, std::string u, std::string sn, std::string c);

    // Methods
    bool isValidKey(std::string k);
    void setPass(std::string raw_p, std::string k);
    std::string getPass(std::string key);
    std::string getRawPass();
    void setUser(std::string usr);
    void setSite(std::string sn);
    std::string getUser();
    std::string getSitename();
    std::string getRawCanary();
};

// functions for handling binary
void load_file(std::vector<Site>& v);
void save_file(std::vector<Site>& v);

#endif