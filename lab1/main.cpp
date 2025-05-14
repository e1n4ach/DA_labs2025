#include <iostream>
using namespace std;

struct Entry {
    char key_str[7]; 
    unsigned long long value;
};

unsigned long long parse_value(const char* s) {
    unsigned long long val = 0;
    while (*s) {
        val = val * 10 + (*s - '0');
        ++s;
    }
    return val;
}

char* find_tab(char* buffer) {
    char* p = buffer;
    while (*p && *p != '\t') ++p;
    return (*p == '\t') ? p : nullptr;
}

int main() {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    Entry* entries = nullptr;
    int capacity = 0, size = 0;

    while (cin.getline(buffer, BUFFER_SIZE)) {
        if (buffer[0] == '\0') continue;

        char* tab_pos = find_tab(buffer);
        if (!tab_pos) continue;

        Entry new_entry;
        for (int i = 0; i < 6; ++i)
            new_entry.key_str[i] = buffer[i];
        new_entry.key_str[6] = '\0';

        char* value_str = tab_pos + 1;
        new_entry.value = parse_value(value_str);

        if (size >= capacity) {
            int new_capacity = capacity ? capacity * 2 : 1;
            Entry* new_entries = new Entry[new_capacity];
            for (int i = 0; i < size; ++i)
                new_entries[i] = entries[i];
            delete[] entries;
            entries = new_entries;
            capacity = new_capacity;
        }
        entries[size++] = new_entry;
    }

    const int MAX_KEY = 1000000;
    int* count = new int[MAX_KEY]();

    for (int i = 0; i < size; ++i) {
        const char* k = entries[i].key_str;
        int key = 0;
        for (int j = 0; j < 6; ++j)
            key = key * 10 + (k[j] - '0');
        count[key]++;
    }

    int total = 0;
    for (int i = 0; i < MAX_KEY; ++i) {
        int old = count[i];
        count[i] = total;
        total += old;
    }

    Entry* output = new Entry[size];
    for (int i = 0; i < size; ++i) {
        const char* k = entries[i].key_str;
        int key = 0;
        for (int j = 0; j < 6; ++j)
            key = key * 10 + (k[j] - '0');
        int pos = count[key]++;
        output[pos] = entries[i];
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < 6; ++j)
            cout << output[i].key_str[j];
        cout << '\t' << output[i].value << '\n';
    }

    delete[] entries;
    delete[] count;
    delete[] output;
    return 0;
}