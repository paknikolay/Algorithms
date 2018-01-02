#pragma once
#include <iostream>
#include <fstream>
#include <string>

using std::string;

class HashTable {
public:
    bool search(string s) {
        for (int i = 0; i < capacity; i++) {
            int h = g(s, i);
            if (table[h] == "")
                return false;
            else if (table[h] == s)
                return true;
        }
        return 0;
    }

    bool insert(string s) {
        if (search(s))
            return false;
        for (int i = 0; i < capacity; i++) {
            int h = g(s, i);
            if ((table[h] == "") or (table[h] == "Deleted")) {
                size++;
                table[h] = s;
                if (4 * size >= 3 * capacity) rehash();
                return true;
            }
        }
        return 0;
    }


    HashTable() {
        table = new string[8];
        capacity = 8;
        size = 0;
    }

    ~HashTable() {
        delete[] table;
    }

private:
    int capacity;
    int size;
    string *table;

    int hash_it(string s, int x = 23) {
        int m = capacity;
        int r = 0;
        for (char i : s) r = (r * i + x) % m;
        return (r + (r) % 2 + 1) % m;
    }

    int g(string s, int i) {
        int m = capacity;
        string temp = "";
        for (char i : s) temp = i + temp;
        return (hash_it(s) + i * hash_it(temp)) % m;
    }

    bool expel(string s) {
        if (!search(s)) return 0;
        for (int i = 0; i < capacity; i++) {
            int h = g(s, i);
            if (table[h] == s) {
                table[h] = "Deleted";
                size--;
                return 1;
            }
        }
        return 0;
    }

    void rehash() {
        string *temp = new string[capacity];
        for (int i = 0; i < capacity; i++) temp[i] = table[i];
        delete[] table;
        capacity *= 2;
        table = new string[capacity];
        for (int i = 0; 2 * i < capacity; i++) {
            if ((temp[i] != "") && (temp[i] != "Deleted")) insert(temp[i]);
        }
        delete[] temp;
    }
};
