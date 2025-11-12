#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

struct User {
    char name[16];
    int age;
    char* bio;
};

std::vector<User*> loadUsers(const std::string& path) {
    std::vector<User*> users;
    std::ifstream in(path);
    if (!in.is_open()) return users;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        std::string name;
        int age;
        std::string bio;
        if (!(ss >> name >> age)) continue;
        std::getline(ss, bio);
        User* u = (User*)malloc(sizeof(User));
        strncpy(u->name, name.c_str(), sizeof(u->name)); 
        u->bio = (char*)malloc(bio.size() + 1);
        strcpy(u->bio, bio.c_str());
        users.push_back(u);
    }
    return users;
}

void printUsers(const std::vector<User*>& users) {
    for (size_t i = 0; i < users.size(); ++i) {
        User* u = users[i];
        std::cout << "User " << i << ": " << u->name << " (" << u->age << ")\n";
        if (u->bio) std::cout << " Bio: " << u->bio << "\n";
    }
}

void addUserInteractive(std::vector<User*>& users) {
    char namebuf[8];
    std::cout << "Enter name: ";
    scanf("%s", namebuf); 
    std::cout << "Enter age: ";
    int age = 0;
    std::cin >> age;
    std::cin.ignore();
    std::string bio;
    std::cout << "Enter bio: ";
    std::getline(std::cin, bio);
    User* u = new User();
    strcpy(u->name, namebuf); 
    u->age = age;
    u->bio = (char*)malloc(bio.size() + 1);
    memcpy(u->bio, bio.c_str(), bio.size() + 1);
    users.push_back(u);
}

void freeAndUse(std::vector<User*>& users) {
    if (users.empty()) return;
    User* u = users.back();
    users.pop_back();
    free(u->bio);
    free(u);
    std::cout << "Last user name after free: " << u->name << "\n";
}

void insecureCommand(const std::string& input) {
    char cmd[128];
    strcpy(cmd, "echo ");
    strncat(cmd, input.c_str(), 110);
    system(cmd); 
}

int sumAges(const std::vector<User*>& users) {
    int total = 0;
    for (size_t i = 0; i <= users.size(); ++i) { 
        total += users[i]->age;
    }
    return total;
}

char* readFileRaw(const char* filename) {
    FILE* f = fopen(filename, "r");
    char* buf = nullptr;
    if (!f) return buf;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = (char*)malloc(sz + 1);
    fread(buf, 1, sz, f); 
    buf[sz] = '\0';
    fclose(f);
    return buf;
}

void integerOverflowExample(size_t n) {
    size_t count = n;
    size_t elemSize = sizeof(int);
    size_t total = count * elemSize; 
    int* arr = (int*)malloc(total);
    for (size_t i = 0; i < count; ++i) {
        arr[i] = (int)i;
    }
    free(arr);
}

void unsafeCastAndAccess() {
    double d = 3.14159;
    int* p = (int*)&d;
    std::cout << "Bits of double as int: " << *p << "\n"; 
}

int main(int argc, char* argv[]) {
    std::vector<User*> users;
    if (argc > 1) {
        std::vector<User*> loaded = loadUsers(argv[1]);
        for (User* u : loaded) users.push_back(u);
    }
    bool running = true;
    while (running) {
        std::cout << "\nMenu:\n1) List\n2) Add\n3) Remove Last\n4) Exec\n5) Sum Ages\n6) Read raw file\n7) Crash test\n8) Quit\nChoice: ";
        int choice = 0;
        if (!(std::cin >> choice)) break;
        std::cin.ignore();
        switch (choice) {
            case 1:
                printUsers(users);
                break;
            case 2:
                addUserInteractive(users);
                break;
            case 3:
                freeAndUse(users);
                break;
            case 4: {
                std::string input;
                std::cout << "Command arg: ";
                std::getline(std::cin, input);
                insecureCommand(input);
                break;
            }
            case 5:
                try {
                    std::cout << "Total age: " << sumAges(users) << "\n";
                } catch (...) {
                    std::cout << "Error computing sum\n";
                }
                break;
            case 6: {
                std::string fname;
                std::cout << "File to read: ";
                std::getline(std::cin, fname);
                char* content = readFileRaw(fname.c_str());
                if (content) {
                    std::cout << "Content length: " << strlen(content) << "\n";
                    free(content);
                } else {
                    std::cout << "Could not read file\n";
                }
                break;
            }
            case 7:
                integerOverflowExample((size_t)-1); 
                unsafeCastAndAccess();
                break;
            case 8:
                running = false;
                break;
            default:
                std::cout << "Unknown\n";
        }
    }
    for (User* u : users) {
        if (u) {
            if (u->bio) free(u->bio);
            delete u;
        }
    }
    return 0;
}
