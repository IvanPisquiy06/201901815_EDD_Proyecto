#include <iostream>

const int MAX_KEYS = 3; // Maximum number of keys in a node

struct Node {
    int keys[MAX_KEYS];
    Node* children[MAX_KEYS + 1];
    int numKeys;
    bool isLeaf;
};

class BTree {
private:
    Node* root;

public:
    BTree() {
        root = nullptr;
    }

    void insert(int key) {
        if (root == nullptr) {
            root = createNode();
            root->keys[0] = key;
            root->numKeys = 1;
            root->isLeaf = true;
        } else {
            if (root->numKeys == MAX_KEYS) {
                Node* newRoot = createNode();
                newRoot->children[0] = root;
                splitChild(newRoot, 0, root);
                insertNonFull(newRoot, key);
                root = newRoot;
            } else {
                insertNonFull(root, key);
            }
        }
    }

    void print() {
        printNode(root);
    }

private:
    Node* createNode() {
        Node* newNode = new Node;
        newNode->numKeys = 0;
        newNode->isLeaf = false;
        for (int i = 0; i < MAX_KEYS + 1; i++) {
            newNode->children[i] = nullptr;
        }
        return newNode;
    }

    void insertNonFull(Node* node, int key) {
        int i = node->numKeys - 1;
        if (node->isLeaf) {
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
            node->numKeys++;
        } else {
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;
            if (node->children[i]->numKeys == MAX_KEYS) {
                splitChild(node, i, node->children[i]);
                if (key > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], key);
        }
    }

    void splitChild(Node* parent, int index, Node* child) {
        Node* newNode = createNode();
        newNode->isLeaf = child->isLeaf;
        newNode->numKeys = MAX_KEYS / 2;

        for (int i = 0; i < MAX_KEYS / 2; i++) {
            newNode->keys[i] = child->keys[i + MAX_KEYS / 2];
        }

        if (!child->isLeaf) {
            for (int i = 0; i < MAX_KEYS / 2 + 1; i++) {
                newNode->children[i] = child->children[i + MAX_KEYS / 2];
            }
        }

        child->numKeys = MAX_KEYS / 2;

        for (int i = parent->numKeys; i > index; i--) {
            parent->children[i + 1] = parent->children[i];
        }

        parent->children[index + 1] = newNode;

        for (int i = parent->numKeys - 1; i >= index; i--) {
            parent->keys[i + 1] = parent->keys[i];
        }

        parent->keys[index] = child->keys[MAX_KEYS / 2];
        parent->numKeys++;
    }

    void printNode(Node* node) {
        if (node != nullptr) {
            for (int i = 0; i < node->numKeys; i++) {
                std::cout << node->keys[i] << " ";
            }
            std::cout << std::endl;

            if (!node->isLeaf) {
                for (int i = 0; i <= node->numKeys; i++) {
                    printNode(node->children[i]);
                }
            }
        }
    }
};