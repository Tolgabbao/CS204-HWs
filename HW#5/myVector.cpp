#include "myVector.h"
#include <iostream>
#include <cmath>

template <typename T1, typename T2>
myVector<T1, T2>::myVector() {
    this->name = "";
    this->vec = {};
}

template <typename T1, typename T2>
myVector<T1, T2>::myVector(const myVector& other) {
    this->vec = other.vec;
}

template <typename T1, typename T2>
void myVector<T1, T2>::push_back(T1 value, T2 key) {
    this->vec.push_back(make_pair(value, key));
}

template <typename T1, typename T2>
void myVector<T1, T2>::remove(T2 key) {
    for (int i = 0; i < this->vec.size(); i++) {
        if (this->vec[i].second == key) {
            this->vec.erase(this->vec.begin() + i);
            return;
        }
    }
}

template <typename T1, typename T2>
void myVector<T1, T2>::replace_key(T2 old_key, T2 new_key) {
    for (int i = 0; i < this->vec.size(); i++) {
        if (this->vec[i].second == old_key) {
            this->vec[i].second = new_key;
            return;
        }
    }
}

template <typename T1, typename T2>
void myVector<T1, T2>::replace_value(T1 key, T2 new_value) {
    for (int i = 0; i < this->vec.size(); i++) {
        if (this->vec[i].second == key) {
            this->vec[i].first = new_value;
            return;
        }
    }
}

template <typename T1, typename T2>
void myVector<T1, T2>::print(T2 key) {
    for (int i = 0; i < this->vec.size(); i++) {
        if (this->vec[i].second == key) {
            cout << this->vec[i].first << endl;
            return;
        }
    }
}

template <typename T1, typename T2>
void myVector<T1, T2>::print_vector() {
    for (int i = 0; i < this->vec.size(); i++) {
        cout << this->vec[i].first << " " << this->vec[i].second << endl;
    }
}

template <typename T1, typename T2>
void myVector<T1, T2>::copy(myVector& other) {
    this->vec = other.vec;
}

template <typename T1, typename T2>
bool myVector<T1, T2>::check_equal(myVector& other) {
    if (this->vec.size() != other.vec.size()) {
        return false;
    }
    for (int i = 0; i < this->vec.size(); i++) {
        if (this->vec[i].first != other.vec[i].first || this->vec[i].second != other.vec[i].second) {
            return false;
        }
    }
    return true;
}



/*Process data should find the Mean, Median, Standart Deviation, Max and Mode
of the vectors value's if it is arithmetic. If it is string then
Most frequent character and most frequent word should be found
Since type specifications of the myVector is assigned during compile-time, type-specific functionalities of
the class must be decided also during compile-time. To check the specified type during the compilation,
you can use expressions like std::is same or std::is arithmetic along with constexpr specifier.*/
template <typename T1, typename T2> // do not use map ,do not use sort
void myVector<T1, T2>::process_data() {
    if constexpr (is_arithmetic<T1>::value) {
        // arithmetic
        // find mean
        double sum = 0;
        for (int i = 0; i < this->vec.size(); i++) {
            sum += this->vec[i].first;
        }
        double mean = sum / this->vec.size();
        cout << "Mean: " << mean << endl;
        // find median
        T1 median;
        T1 numsList[this->vec.size()];
        for (int i = 0; i < this->vec.size(); i++) {
            numsList[i] = this->vec[i].first;
        }
        for (int i = 0; i < this->vec.size(); i++) {
            for (int j = 0; j < this->vec.size() - 1; j++) {
                if (numsList[j] > numsList[j + 1]) {
                    T1 temp = numsList[j];
                    numsList[j] = numsList[j + 1];
                    numsList[j + 1] = temp;
                }
            }
        }
        if (this->vec.size() % 2 == 0) {
            median = (numsList[this->vec.size() / 2] + numsList[this->vec.size() / 2 - 1]) / 2;
        }
        else {
            median = numsList[this->vec.size() / 2];
        }
        cout << "Median: " << median << endl;
        // find standard deviation
        double sum_of_squares = 0;
        for (int i = 0; i < this->vec.size(); i++) {
            sum_of_squares += pow(this->vec[i].first - mean, 2);
        }
        double standard_deviation = sqrt(sum_of_squares / this->vec.size());
        cout << "Standard Deviation: " << standard_deviation << endl;
        // find max
        double max = this->vec[0].first;
        for (int i = 1; i < this->vec.size(); i++) {
            if (this->vec[i].first > max) {
                max = this->vec[i].first;
            }
        }
        cout << "Max: " << max << endl;
        // find mode
        double mode;
        int max_count = 0;
        for (int i = 0; i < this->vec.size(); i++) {
            int count = 0;
            for (int j = 0; j < this->vec.size(); j++) {
                if (this->vec[i].first == this->vec[j].first) {
                    count++;
                }
            }
            if (count > max_count) {
                max_count = count;
                mode = this->vec[i].first;
            }
        }
        cout << "Mode: " << mode << endl;
    }
    else {
        // string
        // find most frequent character use frequencies of the characters
        char most_frequent_character;
        int charList[256] = {0};
        int max_count = 0;
        for (int i = 0; i < this->vec.size(); i++) {
            for (int j = 0; j < this->vec[i].first.length(); j++) {
                charList[this->vec[i].first[j]]++;
            }
        }
        for (int i = 0; i < 256; i++) {
            if (charList[i] > max_count) {
                max_count = charList[i];
                most_frequent_character = i;
            }
        }
        cout << "Most Frequent Character: '" << most_frequent_character << "' (Frequency: " << max_count << ")"<< endl;
        // find most frequent word if the frequency is the same lexigraphically smaller word should be chosen
        string most_frequent_word;
        max_count = 0;
        for (int i = 0; i <  this->vec.size(); i++) {
            int count = 0;
            for (int j = 0; j < this->vec.size(); j++) {
                if (this->vec[i].first == this->vec[j].first) {
                    count++;
                }
            }
            if (count > max_count) {
                max_count = count;
                most_frequent_word = this->vec[i].first;
            }
            else if (count == max_count) {
                if (this->vec[i].first < most_frequent_word) {
                    most_frequent_word = this->vec[i].first;
                }
            }
        }
        cout << "Most Frequent Word: \"" << most_frequent_word << "\" (Frequency: " << max_count << ")"<< endl;
    }
}

       


template <typename T1, typename T2>
bool myVector<T1, T2>::isEmpty() {
    return this->vec.empty();
}

template <typename T1, typename T2>
string myVector<T1, T2>::getName() {
    return this->name;
}

template <typename T1, typename T2>
void myVector<T1, T2>::setName(const string& name) {
    this->name = name;
}

template <typename T1, typename T2>
T1* myVector<T1, T2>::operator[] (const T2& key) {
    for (int i = 0; i < this->vec.size(); i++) {
        if (this->vec[i].second == key) {
            return &this->vec[i].first;
        }
    }
    return nullptr;
}

template <typename T1, typename T2>
bool myVector<T1, T2>::operator== (myVector& other) {
    if (this->vec.size() != other.vec.size()) {
        return false;
    }
    for (int i = 0; i < this->vec.size(); i++) {
        if (this->vec[i].first != other.vec[i].first || this->vec[i].second != other.vec[i].second) {
            return false;
        }
    }
    return true;
}

template <typename T1, typename T2>
typename myVector<T1, T2>::Iterator myVector<T1, T2>::begin() {
    typename myVector<T1, T2>::Iterator it;
    it.it = this->vec.begin();
    // take a reference to the vector as well
    it.vec = &this->vec;
    return it;
}

template <typename T1, typename T2>
myVector<T1, T2>::Iterator::Iterator() {
    this->index = 0;
}

template <typename T1, typename T2>
myVector<T1, T2>::Iterator::Iterator(const Iterator& other) {
    this->index = other.index;
    this->it = other.it;
    this->vec = other.vec;
}

template <typename T1, typename T2>
void myVector<T1, T2>::Iterator::print() {
    cout << this->it->first << " " << this->it->second << endl;
}

template <typename T1, typename T2> 
void myVector<T1, T2>::Iterator::printVector() {
    for (int i = 0; i < this->vec->size(); i++) {
        cout << "Value: " <<this->vec->at(i).first << ", Key: " << this->vec->at(i).second << endl;
    }
}

template <typename T1, typename T2>
bool myVector<T1, T2>::Iterator::find(T2 key, myVector& other) {
    for (int i = 0; i < other.vec.size(); i++) {
        if (other.vec[i].second == key) {
            this->it = other.vec.begin() + i;
            return true;
        }
    }
    return false;
}

template <typename T1, typename T2>
void myVector<T1, T2>::Iterator::replaceKey(T2 new_key) {
    this->it->second = new_key;
}


