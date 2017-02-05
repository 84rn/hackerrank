#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node{
    Node* next;
    Node* prev;
    int value;
    int key;
    Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
    Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{

    protected:
        map<int,Node*> mp; //map the key to the node in the linked list
        int cp;  //capacity
        Node* tail; // double linked list tail pointer
        Node* head; // double linked list head pointer
        virtual void set(int, int) = 0; //set function
        virtual int get(int) = 0; //get function

};
class LRUCache : public Cache {

    public:
        LRUCache(int cap) {
            cp = cap;
            tail = head = nullptr;
        }

        virtual int get(int key) {
            auto it = mp.find(key);

            if(it == mp.end())
                return -1;

            Node *n = it->second;

            if(n == head) {
                return n->value;
            }

            n->prev->next = n->next;
            if(n->next)
                n->next->prev = n->prev;

            Node *new_head = new Node(nullptr, head, n->key, n->value);
            head->prev = new_head;
            head = new_head;

            if(n == tail) {
                tail = tail->prev;
            }

            mp[key] = new_head;
            free(n);

            return new_head->value;
        }

        virtual void set(int key, int value) {

            if(!mp.size()) {
                Node *n = new Node(key, value);
                mp[key] = n;
                head = tail = n;
                return;
            }

            auto it = mp.find(key);

            if(it == mp.end())
            {
                Node *new_node = new Node(nullptr, head, key, value);
                head->prev = new_node;
                head = new_node;

                if(cp == mp.size())
                {
                    mp.erase(tail->key);
                    tail = tail->prev;
                    free(tail->next);
                    tail->next = nullptr;
                }

                mp[new_node->key] = new_node;

                return;
            }

            Node *n = it->second;

            if(n == head) {
                n->value = value;
                return;
            }

            n->prev->next = n->next;
            if(n->next)
                n->next->prev = n->prev;

            Node *new_node = new Node(nullptr, head, key, value);

            head->prev = new_node;
            head = new_node;

            if(n == tail) {
                tail = tail->prev;
            }

            mp[key] = new_node;
            free(n);
        }


};
int main() {
    int n, capacity,i;
    cin >> n >> capacity;
    LRUCache l(capacity);
    for(i=0;i<n;i++) {
        string command;
        cin >> command;
        if(command == "get") {
            int key;
            cin >> key;
            cout << l.get(key) << endl;
        }
        else if(command == "set") {
            int key, value;
            cin >> key >> value;
            l.set(key,value);
        }
    }
    return 0;
}

