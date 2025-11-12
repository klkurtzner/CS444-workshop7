#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

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

void exportUser(const User* u, const string &filename) {
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

string readRaw(const string &path) {
    ifstream f(path, ios::binary);
    if (!f.is_open()) return "";
    string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    return content;
}

void safeOpen() {
    // Open a predefined safe file instead of user input
    ifstream f("safe_file.txt");
    if (!f.is_open()) return;
    char buf[50]{};
    f.read(buf, sizeof(buf) - 1);
}

int main(int argc, char* argv[]) {
    vector<User> users;

    if (argc > 1) users = loadUsers("users.txt"); // hardcoded safe file

    cout << "Loaded " << users.size() << " users\n";

    if (!users.empty()) {
        cout << "First user name: " << users[0].name << "\n";
    }

    if (!users.empty()) {
        User* u = &users[0];
        exportUser(u, "out.txt"); // hardcoded safe output file
    }

    cout << "Total ages: " << sumAges(users) << "\n";

    string raw = readRaw("raw.txt"); // hardcoded safe input
    if (!raw.empty()) cout << "File starts with: " << raw[0] << "\n";

    safeOpen(); // only opens safe predefined file

    return 0;
}
