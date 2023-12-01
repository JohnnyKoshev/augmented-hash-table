#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

struct Node {
    string key; // Should be arbitrary URL
    string value; // Should be arbitrary HTML content
    int counter; // Access Frequency Counter
    Node *next; // Pointer to the next element

    // Default values
    Node(string k, string v) : key(k), value(v), counter(1), next(nullptr) {}
};


class AugmentedHashTable {
private:
    vector<Node *> table;
    int capacity;
    int size;

    unsigned int hashFunction(const string &key) {
        unsigned int hashValue = 0;
        for (char ch: key) {
            hashValue = hashValue * 31 + ch;
        }
        return hashValue % capacity;
    }

    void reorderList(Node *&head, Node *node) {
        // Reorders the list based on the counter value of the node
        if (head == node) return; // Node is already at the front

        // Detach the node from its current position
        Node *prev = nullptr;
        Node *current = head;
        while (current != node) {
            prev = current;
            current = current->next;
        }
        if (prev) prev->next = node->next;

        // Find the new position for the node
        Node *newPrev = nullptr;
        current = head;
        while (current != nullptr && current->counter >= node->counter) {
            newPrev = current;
            current = current->next;
        }

        // Insert the node at its new position
        if (newPrev == nullptr) {
            node->next = head;
            head = node;
        } else {
            node->next = newPrev->next;
            newPrev->next = node;
        }
    }

public:
    AugmentedHashTable(int cap) : capacity(cap), size(0) {
        table.resize(capacity, nullptr);
    }

    ~AugmentedHashTable() {
        for (Node *head: table) {
            while (head) {
                Node *temp = head;
                head = head->next;
                delete temp;
            }
        }
    }

    void display() {
        cout << "Augmented Hash Table:" << endl;
        for (int i = 0; i < capacity; ++i) {
            Node *node = table[i];
            cout << "Index " << i << ": ";
            while (node) {
                cout << "[Key: " << node->key << ", Value: " << node->value
                     << ", Access Count: " << node->counter << "] -> ";
                node = node->next;
            }
            cout << "NULL" << endl;
        }
    }

    void insert(const string &key, const string &value) {
        unsigned int index = hashFunction(key);
        Node *newNode = new Node(key, value);

        // Insert at the beginning of the linked list
        newNode->next = table[index];
        table[index] = newNode;
        size++;
    }

    string get(const string &key) {
        using namespace chrono; // For demonstration of better efficiency
        auto start = high_resolution_clock::now(); // For demonstration of better efficiency
        unsigned int index = hashFunction(key);
        Node *head = table[index];
        Node *current = head;
        while (current) {
            if (current->key == key) {
                current->counter++;
                reorderList(head, current);
                table[index] = head;
                auto end = high_resolution_clock::now(); // For demonstration of better efficiency
                duration<double, milli> duration = end - start; // For demonstration of better efficiency
                cout << "Search Time: " << duration.count() << " milliseconds."
                     << endl; // For demonstration of better efficiency
                return current->value;
            }
            current = current->next;
        }
        auto end = high_resolution_clock::now(); // For demonstration of better efficiency
        duration<double, milli> duration = end - start; // For demonstration of better efficiency
        cout << "Search Time: " << duration.count() << " milliseconds."
             << endl; // For demonstration of better efficiency
        return "Not found";
    }

    void remove(const string &key) {
        unsigned int index = hashFunction(key);
        Node *current = table[index];
        Node *prev = nullptr;

        while (current) {
            if (current->key == key) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    table[index] = current->next;
                }
                delete current;
                size--;
                return;
            }
            prev = current;
            current = current->next;
        }
        cout << "Key not found" << endl;
    }
};

int main() {
    AugmentedHashTable hashTable(10); // Create a hash table with capacity 10
    // Insert URL-content pairs
    hashTable.insert("http://example.com", "<html>Content of example.com</html>");
    hashTable.insert("http://example.org", "<html>Content of example.org</html>");
    hashTable.insert("http://example.net", "<html>Content of example.net</html>");
    hashTable.insert("http://example.uz", "<html>Content of example.uz</html>");
    hashTable.insert("http://example.eng", "<html>Content of example.eng</html>");
    hashTable.insert("http://example.kr", "<html>Content of example.kr</html>");
    hashTable.insert("http://example.co", "<html>Content of example.co</html>");
    hashTable.insert("http://example.kg", "<html>Content of example.kg</html>");
    hashTable.insert("http://example.ru", "<html>Content of example.ru</html>");
    hashTable.insert("http://example.mn", "<html>Content of example.mn</html>");
    hashTable.insert("http://example.fr", "<html>Content of example.fr</html>");
    hashTable.insert("http://example.es", "<html>Content of example.es</html>");
    hashTable.insert("http://example.it", "<html>Content of example.it</html>");
    hashTable.insert("http://example.de", "<html>Content of example.de</html>");
    hashTable.insert("http://example.jp", "<html>Content of example.jp</html>");
    hashTable.insert("http://example.cn", "<html>Content of example.cn</html>");
    hashTable.insert("http://example.in", "<html>Content of example.in</html>");
    hashTable.insert("http://example.br", "<html>Content of example.br</html>");
    hashTable.insert("http://example.ng", "<html>Content of example.ng</html>");
    hashTable.insert("http://example.mx", "<html>Content of example.mx</html>");
    hashTable.insert("http://example.ca", "<html>Content of example.ca</html>");
    hashTable.insert("http://example.au", "<html>Content of example.au</html>");


    hashTable.display();
    // Access elements
    cout << hashTable.get("http://example.com") << endl;
    hashTable.display();
    cout << hashTable.get("http://example.com") << endl;
    cout << hashTable.get("http://example.org") << endl;
    cout << hashTable.get("http://example.mx") << endl;
    cout << hashTable.get("http://example.mx") << endl;
    cout << hashTable.get("http://example.mx") << endl;
    cout << hashTable.get("http://example.jp") << endl;
    hashTable.remove("http://example.ru");
    // The 'http://example.com' URL is now at the top of its linked list due to higher access frequency (counter)
    hashTable.display();
    return 0;
}