#include <iostream>
#include <memory>

using namespace std;

class LinkedListSet {
private:
    struct Node {
        int data;
        unique_ptr<Node> next;

        Node(int val) : data(val), next(nullptr) {}
    };

    unique_ptr<Node> head;

public:
    LinkedListSet() : head(nullptr) {}

    // Insertion (+ operator overloading)
    LinkedListSet& operator+(int val) {
        insert(val);
        return *this;
    }

    // Deletion (- operator overloading)
    void operator-(int val) {
        remove(val);
    }

    // Set Union (+= operator overloading)
    void operator+=(const LinkedListSet& otherSet) {
        Node* temp = otherSet.head.get();
        while (temp) {
            insert(temp->data);
            temp = temp->next.get();
        }
    }

    // Set Difference (-= operator overloading)
    void operator-=(const LinkedListSet& otherSet) {
        Node* temp = otherSet.head.get();
        while (temp) {
            remove(temp->data);
            temp = temp->next.get();
        }
    }

    // Set Intersection (&= operator overloading)
    void operator&=(const LinkedListSet& otherSet) {
        LinkedListSet intersectionSet;
        Node* temp = otherSet.head.get();

        while (temp) {
            if (contains(temp->data)) {
                intersectionSet.insert(temp->data);
            }
            temp = temp->next.get();
        }

        // Keep only the elements that are in the intersection set
        Node* current = head.get();
        while (current) {
            if (!intersectionSet.contains(current->data)) {
                Node* toDelete = current;
                current = current->next.get();
                remove(toDelete->data);
            } else {
                current = current->next.get();
            }
        }
    }

    // Display the set
    void display() const {
        Node* temp = head.get();
        cout << "LinkedListSet{";
        while (temp) {
            cout << temp->data;
            temp = temp->next.get();
            if (temp) {
                cout << ", ";
            }
        }
        cout << ", }" << endl;
    }

private:
    void insert(int val) {
        if (!contains(val)) {
            unique_ptr<Node> newNode = make_unique<Node>(val);
            if (!head || val < head->data) {
                newNode->next = move(head);
                head = move(newNode);
            } else {
                Node* temp = head.get();
                while (temp->next && val > temp->next->data) {
                    temp = temp->next.get();
                }
                newNode->next = move(temp->next);
                temp->next = move(newNode);
            }
        }
    }

    void remove(int val) {
        Node* temp = head.get();
        Node* prev = nullptr;

        if (temp != nullptr && temp->data == val) {
            head = move(temp->next);
            return;
        }

        while (temp != nullptr && temp->data != val) {
            prev = temp;
            temp = temp->next.get();
        }

        if (temp == nullptr) {
            return;
        }

        prev->next = move(temp->next);
    }

    bool contains(int val) const {
        Node* temp = head.get();
        while (temp) {
            if (temp->data == val) {
                return true;
            }
            temp = temp->next.get();
        }
        return false;
    }
};



// Driver code remains the same as previously provided

int main() {
    LinkedListSet set1;
    set1 + 1 + 2 + 3 + 17; // Insertion
    set1.display();

    set1 - 2; // Deletion
    set1.display();

    LinkedListSet set2;
    set2 + 3 + 4 + 5;
    set2.display();

    set2 += set1 + 7 + 3 + 19; // Insertion with multiple right-hand values
    set2.display();

    LinkedListSet set3;
    set3 + 3 + 4 + 7 + 17 + 41;
    set3.display();

    LinkedListSet set4;
    set4 + 41 + 37 + 7 + 19 + 41;
    set4.display();

    // Set Union
    set1 += set2;
    set1.display();

    // Set Difference
    set1 -= set3;
    set1.display();

    // Set Intersection
    set1 &= set4;
    set1.display();

    return 0;
}
