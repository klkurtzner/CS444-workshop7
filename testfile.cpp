#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct User {
    string name;
    int age;
    string bio;
};

bool isSafePath(const string &path) {
    if (path.empty()) return false;
    if (path.find("..") != string::npos) return false;
    if (!path.empty() && path[0] == '/') return false;
    return true;
}

vector<User> loadUsers(const string &path) {
    vector<User> users;
    if (!isSafePath(path)) return users;
    ifstream in(path);
    if (!in.is_open()) return users;
    string line;
    while (getline(in, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        if (pos1 == string::npos) continue;
        User u;
        u.name = line.substr(0, pos1);
        u.age = 0;
        if (pos2 != string::npos) {
            u.age = stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            u.bio = line.substr(pos2 + 1);
        } else {
            u.age = stoi(line.substr(pos1 + 1));
            u.bio = "";
        }
        users.push_back(u);
    }
    return users;
}

User* findUserByName(vector<User> &users, const string &name) {
    for (auto &u : users) {
        if (u.name == name) return &u;
    }
    return nullptr;
}

void exportUser(const User* u, const string &filename) {
    if (!u || !isSafePath(filename)) return;
    ofstream out(filename, ios::app);
    if (!out.is_open()) return;
    out << u->name << endl;
    out.close();
}

int sumAges(const vector<User> &users) {
    int total = 0;
    for (const auto &u : users) total += u.age;
    return total;
}

string readRaw(const string &path) {
    if (!isSafePath(path)) return "";
    ifstream f(path, ios::binary);
    if (!f.is_open()) return "";
    string contents((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    f.close();
    return contents;
}

void safeOpen(const string &path) {
    if (!isSafePath(path)) return;
    ifstream f(path);
    if (!f.is_open()) return;
    char buf[50];
    f.read(buf, sizeof(buf) - 1);
    f.close();
}

int main(int argc, char* argv[]) {
    vector<User> users;

    if (argc > 1) {
        string path = argv[1];
        users = loadUsers(path);
    }

    cout << "Loaded " << users.size() << " users\n";

    if (!users.empty()) {
        cout << "First user name: " << users[0].name << "\n";
    }

    if (argc > 2) {
        string name = argv[2];
        User* u = findUserByName(users, name);
        exportUser(u, "out.txt");
    }

    cout << "Total ages: " << sumAges(users) << "\n";

    if (argc > 3) {
        string path = argv[3];
        string raw = readRaw(path);
        if (!raw.empty()) cout << "File starts with: " << raw[0] << "\n";
    }

    if (argc > 4) {
        string path = argv[4];
        safeOpen(path);
    }

    return 0;
}
