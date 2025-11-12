#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct User {
    char name[24];
    int age;
    char *bio;
};

bool isSafePath(const string &path) {
    if (path.empty()) return false;
    if (path.find("..") != string::npos) return false;
    if (!path.empty() && path[0] == '/') return false;
    return true;
}

vector<User*> loadUsers(const string &path) {
    vector<User*> users;
    if (!isSafePath(path)) return users;
    ifstream in(path);
    if (!in.is_open()) return users;
    string line;
    while (getline(in, line)) {
        char buf[256];
        strncpy(buf, line.c_str(), sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        char *token = strtok(buf, ",");
        if (!token) continue;
        User *u = (User*)malloc(sizeof(User));
        memset(u, 0, sizeof(User));
        strncpy(u->name, token, sizeof(u->name) - 1);
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
    if (!isSafePath(filename)) return;
    ofstream out(filename, ios::app);
    if (!out.is_open()) return;
    out << u->name << endl;
    out.close();
}

int sumAges(const vector<User*> &users) {
    int total = 0;
    for (size_t i = 0; i < users.size(); ++i) total += users[i]->age;
    return total;
}

char* readRaw(const string &path) {
    if (!isSafePath(path)) return nullptr;
    ifstream f(path, ios::binary | ios::ate);
    if (!f.is_open()) return nullptr;
    streamsize size = f.tellg();
    f.seekg(0, ios::beg);
    char* buf = (char*)malloc(size + 1);
    if (!buf) return nullptr;
    f.read(buf, size);
    buf[size] = '\0';
    f.close();
    return buf;
}

void safeOpen(const string &path) {
    if (!isSafePath(path)) return;
    ifstream f(path, ios::binary);
    if (!f.is_open()) return;
    char buf[50] = {};
    f.read(buf, sizeof(buf) - 1);
    f.close();
}

int main(int argc, char* argv[]) {
    vector<User*> users;

    string safeLoadPath;
    if (argc > 1) {
        safeLoadPath = argv[1];
        if (isSafePath(safeLoadPath)) {
            vector<User*> loaded = loadUsers(safeLoadPath);
            for (User* u : loaded) users.push_back(u);
        }
    }

    cout << "Loaded " << users.size() << " users\n";

    if (!users.empty()) {
        User* u = users[0];
        cout << "First user name: " << u->name << "\n";
    }

    string safeExportPath = "out.txt";
    if (argc > 2) {
        User* u = findUserByName(users, string(argv[2]));
        if (isSafePath(safeExportPath)) exportUser(u, safeExportPath);
    }

    int total = sumAges(users);
    cout << "Total ages: " << total << "\n";

    string safeReadPath;
    char* raw = nullptr;
    if (argc > 3) {
        safeReadPath = argv[3];
        if (isSafePath(safeReadPath)) raw = readRaw(safeReadPath);
    }
    if (raw) {
        cout << "File starts with: " << raw[0] << "\n";
        free(raw);
    }

    string safeOpenPath;
    if (argc > 4) {
        safeOpenPath = argv[4];
        if (isSafePath(safeOpenPath)) safeOpen(safeOpenPath);
    }

    if (!users.empty()) {
        User* u = users.back();
        if (u->bio) free(u->bio);
        free(u);
        users.pop_back();
    }

    for (User* u : users) {
        if (u) {
            if (u->bio) free(u->bio);
            free(u);
        }
    }

    return 0;
}
