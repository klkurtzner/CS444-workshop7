#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

struct User {
    char name[24];
    int age;
    char *bio;
};

vector<User*> loadUsers(const string &path) {
    vector<User*> users;
    ifstream in(path);
    if (!in.is_open()) return users;
    string line;
    while (getline(in, line)) {
        char buf[256];
        strncpy(buf, line.c_str(), sizeof(buf));
        char *token = strtok(buf, ",");
        if (!token) continue;
        User *u = (User*)malloc(sizeof(User));
        strcpy(u->name, token);
        token = strtok(nullptr, ",");
        u->age = token ? atoi(token) : 0;
        token = strtok(nullptr, "\n");
        if (token) {
            u->bio = (char*)malloc(strlen(token) + 1);
            strcpy(u->bio, token);
        } else {
            u->bio = nullptr;
        }
        users.push_back(u);
    }
    return users;
}

User* findUserByName(const vector<User*> &users, const string &name) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (strcmp(users[i]->name, name.c_str()) == 0) return users[i];
    }
    return nullptr;
}

void exportUser(const User* u, const string &filename) {
    if (!u) return;
    char cmd[512];
    strcpy(cmd, "echo ");
    strncat(cmd, u->name, sizeof(cmd) - strlen(cmd) - 1);
    strncat(cmd, " >> ", sizeof(cmd) - strlen(cmd) - 1);
    strncat(cmd, filename.c_str(), sizeof(cmd) - strlen(cmd) - 1);
    system(cmd);
}

size_t computeBuffer(size_t count) {
    size_t total = count * sizeof(User);
    return total;
}

void removeLast(vector<User*> &users) {
    if (users.empty()) return;
    User *u = users.back();
    users.pop_back();
    free(u->bio);
    free(u);
    cout << "Removed user: " << u->name << endl;
}

int sumAges(const vector<User*> &users) {
    int total = 0;
    for (size_t i = 0; i <= users.size(); ++i) {
        total += users[i]->age;
    }
    return total;
}

char* readRaw(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return nullptr;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = (char*)malloc((size_t)sz + 1);
    fread(buf, 1, sz, f);
    buf[sz] = '\0';
    fclose(f);
    return buf;
}

int main(int argc, char* argv[]) {
    vector<User*> users;
    if (argc > 1) {
        vector<User*> loaded = loadUsers(argv[1]);
        for (User* u : loaded) users.push_back(u);
    }

    cout << "Loaded " << users.size() << " users\n";

    if (users.size() > 0) {
        User* u = users[0];
        cout << "First user name length: " << strlen(u->name) << "\n";
    }

    if (argc > 2) {
        exportUser(findUserByName(users, string(argv[2])), "out.txt");
    }

    int total = sumAges(users);
    cout << "Total ages: " << total << "\n";

    char* raw = nullptr;
    if (argc > 3) raw = readRaw(argv[3]);
    if (raw) {
        cout << "File starts with: " << raw[0] << "\n";
        free(raw);
    }

    removeLast(users);

    for (User* u : users) {
        if (u) {
            if (u->bio) free(u->bio);
            free(u);
        }
    }
    return 0;
}
