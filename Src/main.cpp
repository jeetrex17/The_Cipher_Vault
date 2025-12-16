#include <iostream>
#include <vector>
#include "../Header/Site.h"

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
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        std::cin.ignore(1000, '\n');

        switch (option) {
            case 1: {
                std::cout << "Site: "; std::getline(std::cin, t_site);
                std::cout << "User: "; std::getline(std::cin, t_user);
                std::cout << "Pass: "; std::getline(std::cin, t_pass);
                std::cout << "Key: ";  std::getline(std::cin, t_key);

                vault.push_back(Site(t_pass, t_user, t_site, t_key));
                save_file(vault);
                std::cout << "[SAVED]\n";
                break;
            }
            case 2: {
                std::cout << "\nID | SITE        | USER        | ENCRYPTED_PASS\n";
                std::cout << "---|------------|------------|---------------\n";
                for (int i = 0; i < vault.size(); i++) {
                    std::cout << i + 1 << "  | "
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

                if (std::cin.fail()) {
                    std::cout << "Invalid ID!\n";
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    break;
                }
                std::cin.ignore(1000, '\n');
                id--;

                if (id >= 0 && id < vault.size()) {
                    std::cout << "Enter Master Key: ";
                    std::getline(std::cin, t_key);

                    if (vault[id].isValidKey(t_key)) {
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
                std::cout << "Bro there is no " << option << " Option \n";
            }
        }
    }
}