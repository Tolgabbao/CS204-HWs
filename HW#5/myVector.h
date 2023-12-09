#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename T1, typename T2> // T1 is the value type, T2 is the key type
class myVector {
public:
    myVector();
    myVector(const myVector& other);
    void push_back(T1 value, T2 key);
    void remove(T2 key);
    void replace_key(T2 old_key, T2 new_key);
    void replace_value(T1 key, T2 new_value);
    void setName(const string& name);
    void print(T2 key);
    void print_vector();
    void copy(myVector& other);
    bool check_equal(myVector& other);
    void process_data();
    bool isEmpty();
    string getName();
    T1* operator[] (const T2& key);
    bool operator== (myVector& other);
    vector<pair<T1, T2>> vec;
    class Iterator {
    public:
        Iterator();
        Iterator(const Iterator& other);
        void print();
        void printVector();
        bool find(T2 key, myVector& other);
        void replaceKey(T2 new_key);
        typename vector<pair<T1, T2>>::iterator it;
        vector<pair<T1, T2>>* vec;
    private:
        int index;
    };
    typename myVector<T1, T2>::Iterator begin();
private:
    string name;
    
};

#include "myVector.cpp"
#endif
/*******************************************************************************************************/
