#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <limits>
#include <climits>

using namespace std;

struct Person {
    char name[8];
    int age;
    char *notes;
};

Person* make_person(const string &n, int a, const string &note) {
    Person* p = (Person*)malloc(sizeof(Person));
    strncpy(p->name, n.c_str(), sizeof(p->name)); // may truncate, no null-ensure
    p->age = a;
    p->notes = (char*)malloc(note.size() + 1);
    strcpy(p->notes, note.c_str()); // possible overflow if note wasn't sized correctly
    return p;
}

int sum_and_overflow(const vector<int>& v) {
    int total = INT_MAX - 5;
    for (size_t i = 0; i < v.size(); ++i) {
        total += v[i]; // potential integer overflow
    }
    return total;
}

int divide_checked(int a, int b) {
    return a / b; // no check for zero
}

void command_run(const string &arg) {
    char buf[64];
    strcpy(buf, "echo ");
    strncat(buf, arg.c_str(), sizeof(buf) - strlen(buf) - 1); // still may allow injection in shell context
    system(buf);
}

void leak_memory() {
    int *arr = new int[100];
    arr[0] = 123;
}

void use_uninitialized() {
    int x;
    cout << "Uninitialized value: " << x << endl; // use-before-init
}

void use_after_free_example() {
    char *p = (char*)malloc(16);
    strcpy(p, "hello");
    free(p);
    cout << "Use after free: " << p << endl; // use-after-free
}

void out_of_bounds_access() {
    vector<int> a = {1,2,3};
    cout << a[10] << endl; // out-of-bounds access
}

void null_deref() {
    int *p = nullptr;
    *p = 5; // null pointer dereference
}

char* read_raw(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return nullptr;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = (char*)malloc((size_t)sz + 1);
    fread(buf, 1, sz, f); // unchecked return
    buf[sz] = '\0';
    fclose(f);
    return buf;
}

int main(int argc, char* argv[]) {
    vector<Person*> people;
    people.push_back(make_person("AliceLongName", 30, "notes here"));
    people.push_back(make_person("Bob", 25, "short"));

    vector<int> val = {100, 200, 300};
    cout << "Sum (with potential overflow): " << sum_and_overflow(val) << endl;

    use_uninitialized();
    use_after_free_example();
    out_of_bounds_access();

    if (argc > 1) {
        command_run(argv[1]);
    } else {
        char inbuf[4];
        printf("Enter small word: ");
        scanf("%3s", inbuf); // limited but still unsafe style usage
        string s(inbuf);
        command_run(s);
    }

    leak_memory();

    if (argc > 2) {
        char* content = read_raw(argv[2]);
        if (content) {
            printf("First char: %c\n", content[0]);
            free(content);
        }
    }

    // intentionally provoke division by zero
    cout << "Div result: " << divide_checked(10, 0) << endl;

    // free and cleanup
    for (Person* p : people) {
        if (p) {
            if (p->notes) free(p->notes); // cleanup notes
            free(p);
        }
    }

    // intentionally cause null dereference in a guarded way so program still exits
    // (but the code above includes a problematic function separately)
    // ended
    return 0;
}
