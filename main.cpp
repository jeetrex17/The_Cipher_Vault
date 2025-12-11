#include <cctype>
#include <cstring>
#include <fstream>
#include <ios>
#include <string>
#include <iostream>
#include <vector>


class Site{
    private:
        std::string password;
        std::string check_phrase;
        std::string encrypt(std::string pass , const std::string& k){
            if (k.empty()) return pass;
            for(size_t i = 0; i < pass.length() ; i++)
            {
                char keychar = k[i % k.length()];
                pass[i] = keychar ^ pass[i];
            }
            return pass;
        }
    public:
        std::string username;
        std::string sitename;

        Site(){};
        Site(std::string raw_p , std::string u , std::string sn , std::string key): username(u) , sitename(sn){
            password = encrypt(raw_p, key);
            check_phrase = encrypt("valid", key);
        }
        static Site createFromFile(std::string p, std::string u, std::string sn, std::string c) {
            Site s;

            s.password = p;
            s.username = u;
            s.sitename = sn;
            s.check_phrase = c;

            return s;
        }
        bool isValidKey(std::string k) {
            std::string decrypted_canary = encrypt(check_phrase, k);
            if (decrypted_canary == "valid") {
                return true;
            } else {
                std::cout << "Wrong key buddy" << std::endl;
                return false;
            }
        }
        // Site(std::string p , std::string u , std::string sn): password(p) , username(u) , sitename(sn){
        // }
        void setPass(std::string raw_p , std::string k) {
            password = encrypt(raw_p,k);
        }
        std::string getPass(std::string key){
            return encrypt(password , key);
        }

        std::string getRawPass() {
            return password;
        }

        void setUser(std::string usr) { username = usr; }
        void setSite(std::string sn) { sitename = sn; }

        std::string getUser(){return username;}
        std::string getSitename(){return sitename;}
        std::string getRawCanary() {return check_phrase;}
};


// void load_file(std::vector<Site>& v){ // read
//     std::ifstream file("vault.txt");
//     std::string t_site, t_user, t_pass, t_canary;
//     while(file >> t_site >> t_user >> t_pass >> t_canary){
//         Site s =  Site::createFromFile(t_pass, t_user, t_site, t_canary);
//         v.push_back(s);
//     }
//     file.close();
// }


//making load_file for binary
void load_file(std::vector<Site>& v) {
    std::fstream fcout;
    fcout.open("vault.bin", std::ios::binary | std::ios::in);

    if (!fcout) {
        std::cout << "No existing vault found. Starting fresh.\n";
        return;
    }

    // 1. Read the Master Header (How many sites?)
    size_t count;
    fcout.read((char*)&count, sizeof(count));

    if (fcout) {
        std::cout << "Loading " << count << " sites...\n";

        for (int i = 0; i < count; i++) {
            // Temporary variables to hold the data for ONE site
            std::string t_site, t_user, t_pass, t_canary;
            size_t len;

            // --- READ SITE NAME ---
            fcout.read((char*)&len, sizeof(len));    // Read Length
            t_site.resize(len);                      // Resize Buffer
            fcout.read(&t_site[0], len);             // Read Data

            // --- READ USERNAME ---
            fcout.read((char*)&len, sizeof(len));
            t_user.resize(len);
            fcout.read(&t_user[0], len);

            // --- READ PASSWORD ---
            fcout.read((char*)&len, sizeof(len));
            t_pass.resize(len);
            fcout.read(&t_pass[0], len);

            // --- READ CANARY ---
            fcout.read((char*)&len, sizeof(len));
            t_canary.resize(len);
            fcout.read(&t_canary[0], len);

            // 2. Rebuild the Site Object (using your static tool)
            Site s = Site::createFromFile(t_pass, t_user, t_site, t_canary);

            // 3. Add it to the list
            v.push_back(s);
        }
    }
    fcout.close();
}

// void save_file(std::vector<Site>& v){
//     std::ofstream file("vault.txt");  // open / create
//     if(file.is_open()){
//         std::cout << "DEBUG: File opened successfully!\n";
//         for(int i = 0; i < v.size() ; i++)
//         {
//             file<< v[i].getSitename() << " "
//                 << v[i].getUser() << " "
//                 << v[i].getRawPass() << " "
//                 << v[i].getRawCanary() << "\n"  ;
//         }

//         file.close();
//     }
//     else {
//         std::cout << "DEBUG: Could not open file!\n";
//     }
// }

// making it save to binary
void save_file(std::vector<Site>& v){
    std::fstream fcout;
    // fist opening the file in write mode "out"
    fcout.open("vault.bin" , std::ios::binary | std::ios::out);
    // first we will store the size of the vextor at very top so we know how many tomes to loop it throught
    size_t s_vector = v.size();
    fcout.write(reinterpret_cast<char*>(&s_vector),sizeof(s_vector));

    if(fcout){
        std::cout<<"File opend succesfully \n";
        for(int i = 0 ; i < s_vector ; i++){
            // writting sitename
            std::string f_sitename = v[i].getSitename();
            size_t len = f_sitename.size();
            fcout.write((const char*)&len, sizeof(len));
            fcout.write(f_sitename.data(), len);

            // writting username
            std::string f_username = v[i].getUser();
            size_t lenu = f_username.size();
            fcout.write((const char*)&lenu, sizeof(lenu));
            fcout.write(f_username.data(), lenu);

            // writting raw password
            std::string f_rawpass = v[i].getRawPass();
            size_t lenr = f_rawpass.size();
            fcout.write((const char*)&lenr, sizeof(lenr));
            fcout.write(f_rawpass.data(), lenr);

            // writting raw carnay
            std::string f_carnary = v[i].getRawCanary();
            size_t lenc = f_carnary.size();
            fcout.write((const char*)&lenc, sizeof(lenc));
            fcout.write(f_carnary.data(), lenc);
        }
    }
    else {
        std::cout<< " Error opeing the file \n" ;
    }

    fcout.close();
}

int main() {
    std::vector<Site> vault;
    load_file(vault);
    int option;

    std::string t_site, t_user, t_pass, t_key;
    bool exit = false;

    while (!exit) {
        std::cout << "\n--- THE VAULT ---\n";
        std::cout << "1. Add Site\n";
        std::cout << "2. Show All sites (Encrypted)\n";
        std::cout << "3. Unlock One Site\n";
        std::cout << "4. Exit\n";
        std::cout << "Select: ";
        std::cin >> option;

        if (std::cin.fail()) {
            std::cin.clear(); // Reset the "Fail Flag" so cin works again
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        // 2. Clean the buffer for the next valid read
        std::cin.ignore(1000, '\n');

        switch (option) {
            case 1: {
                    std::cout << "Site: ";
                    std::getline(std::cin, t_site);

                    std::cout << "User: ";
                    std::getline(std::cin, t_user);

                    std::cout << "Pass: ";
                    std::getline(std::cin, t_pass);

                    std::cout << "Key: ";
                    std::getline(std::cin, t_key);

                    vault.push_back(Site(t_pass, t_user, t_site, t_key));

                    save_file(vault);
                    std::cout << "[SAVED]\n";
                    break;
            }
            case 2: {
                std::cout << "\nID | SITE       | USER       | ENCRYPTED_PASS\n";
                std::cout << "---|------------|------------|---------------\n";

                for (int i = 0; i < vault.size(); i++) {
                    std::cout << i+1 << "  | "
                              << vault[i].getSitename() << " | "
                              << vault[i].getUser() << " | "
                              << vault[i].getRawPass() << "\n";
                }
                break;
            }

            case 3: {
                int id;
                std::cout << "Enter ID to unlock: ";
                std::cin >> id;

                std::cin.ignore();
                id--;

                if (id >= 0 && id < vault.size()) {
                    std::cout << "Enter Master Key: ";
                    std::cin >> t_key;

                    std::cin.ignore();

                    if(vault[id].isValidKey(t_key)){
                        std::cout << "---------------------------------\n";
                        std::cout << "Decrypted Password: " << vault[id].getPass(t_key) << "\n";
                        std::cout << "---------------------------------\n";
                    }
                } else {
                    std::cout << "Invalid ID!\n";
                }
                break;
            }

            case 4: {
                save_file(vault);
                std::cout << "Vault saved. Exiting...\n";
                exit = true;
                return 0;
            }

            default: {
                std::cout << "Bro there is no " << option << " Option \n" ;
            }
        }
    }

}
