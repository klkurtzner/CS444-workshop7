#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct User {
    string name;
    int age;
    string bio;
};

vector<User> loadUsers(const fs::path &path) {
    vector<User> users;
    ifstream in(path);
    if (!in.is_open()) return users;

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string name, ageStr, bio;
        if (!getline(ss, name, ',')) continue;
        if (!getline(ss, ageStr, ',')) continue;
        getline(ss, bio);
        users.push_back({name, stoi(ageStr), bio});
    }
    return users;
}

User* findUserByName(vector<User> &users, const string &name) {
    for (auto &u : users)
        if (u.name == name) return &u;
    return nullptr;
}

void exportUser(const User* u, const fs::path &filename) {
    if (!u) return;
    ofstream out(filename, ios::app);
    if (!out.is_open()) return;
    out << u->name << endl;
}

int sumAges(const vector<User> &users) {
    int total = 0;
    for (const auto &u : users) total += u.age;
    return total;
}

string readRaw(const fs::path &path) {
    ifstream f(path, ios::binary);
    if (!f.is_open()) return "";
    string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    return content;
}

void safeOpen(const fs::path &path) {
    ifstream f(path);
    if (!f.is_open()) return;
    char buf[50];
    f.read(buf, sizeof(buf) - 1);
}

bool isSafeFileName(const string &name) {
    return !name.empty() && name.find_first_of("/\\") == string::npos;
}

int main(int argc, char* argv[]) {
    vector<User> users;

    if (argc > 1 && isSafeFileName(argv[1])) users = loadUsers(argv[1]);

    cout << "Loaded " << users.size() << " users\n";

    if (!users.empty()) cout << "First user name: " << users[0].name << "\n";

    if (argc > 2 && isSafeFileName(argv[2])) {
        User* u = findUserByName(users, argv[2]);
        exportUser(u, "out.txt");
    }

    cout << "Total ages: " << sumAges(users) << "\n";

    if (argc > 3 && isSafeFileName(argv[3])) {
        string raw = readRaw(argv[3]);
        if (!raw.empty()) cout << "File starts with: " << raw[0] << "\n";
    }

    if (argc > 4 && isSafeFileName(argv[4])) safeOpen(argv[4]);

    return 0;
}
