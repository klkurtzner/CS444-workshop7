#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct User {
    string name;
    int age;
    string bio;
};

vector<User> loadUsers(const string &path) {
    vector<User> users;
    ifstream in(path);
    if (!in.is_open()) return users;

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        string name, ageStr, bio;
        if (!getline(ss, name, ',')) continue;
        if (!getline(ss, ageStr, ',')) continue;
        getline(ss, bio); // optional bio
        User u;
        u.name = name;
        u.age = stoi(ageStr);
        u.bio = bio;
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
    if (!u.empty() || filename.empty()) return;
    ofstream out(filename, ios::app);
    if (!out.is_open()) return;
    out << u->name << endl;
}

int sumAges(const vector<User> &users) {
    int total = 0;
    for (const auto &u : users) total += u.age;
    return total;
}

string readRaw(const string &path) {
    ifstream f(path, ios::binary);
    if (!f.is_open()) return "";
    string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    f.close();
    return content;
}

void safeOpen(const string &path) {
    ifstream f(path);
    if (!f.is_open()) return;
    char buf[50];
    f.read(buf, sizeof(buf) - 1);
    f.close();
}

int main(int argc, char* argv[]) {
    vector<User> users;

    if (argc > 1) users = loadUsers(argv[1]);

    cout << "Loaded " << users.size() << " users\n";

    if (!users.empty()) cout << "First user name: " << users[0].name << "\n";

    if (argc > 2) {
        User* u = findUserByName(users, argv[2]);
        if (u) exportUser(u, "out.txt");
    }

    cout << "Total ages: " << sumAges(users) << "\n";

    if (argc > 3) {
        string raw = readRaw(argv[3]);
        if (!raw.empty()) cout << "File starts with: " << raw[0] << "\n";
    }

    if (argc > 4) safeOpen(argv[4]);

    return 0;
}
