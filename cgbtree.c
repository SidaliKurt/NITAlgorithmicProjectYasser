#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEGREE 3 // Minimum degree (t)

typedef struct BTreeNode {
    int *keys;                     // Array of keys
    int degree;                    // Minimum degree
    struct BTreeNode **children;   // Array of child pointers
    int numKeys;                   // Current number of keys
    bool isLeaf;                   // True if leaf node
} BTreeNode;

// Create a new B-tree node
BTreeNode* createNode(int degree, bool isLeaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->degree = degree;
    node->isLeaf = isLeaf;
    node->keys = (int*)malloc((2 * degree - 1) * sizeof(int));
    node->children = (BTreeNode**)malloc(2 * degree * sizeof(BTreeNode*));
    node->numKeys = 0;
    return node;
}

// Traverse the B-tree
void traverse(BTreeNode* root) {
    if (!root) return;
    int i;
    for (i = 0; i < root->numKeys; i++) {
        if (!root->isLeaf)
            traverse(root->children[i]);
        printf("%d ", root->keys[i]);
    }
    if (!root->isLeaf)
        traverse(root->children[i]);
}

// Search for a key in the B-tree
BTreeNode* search(BTreeNode* root, int key) {
    if (!root) return NULL;
    int i = 0;
    while (i < root->numKeys && key > root->keys[i])
        i++;
    if (i < root->numKeys && root->keys[i] == key)
        return root;
    if (root->isLeaf)
        return NULL;
    return search(root->children[i], key);
}

// Split a child node
void splitChild(BTreeNode* parent, int i, BTreeNode* child) {
    int degree = child->degree;
    BTreeNode* newChild = createNode(degree, child->isLeaf);
    newChild->numKeys = degree - 1;

    for (int j = 0; j < degree - 1; j++)
        newChild->keys[j] = child->keys[j + degree];
    
    if (!child->isLeaf) {
        for (int j = 0; j < degree; j++)
            newChild->children[j] = child->children[j + degree];
    }

    child->numKeys = degree - 1;

    for (int j = parent->numKeys; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = newChild;

    for (int j = parent->numKeys - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = child->keys[degree - 1];
    parent->numKeys++;
}

// Insert a non-full node
void insertNonFull(BTreeNode* node, int key) {
    int i = node->numKeys - 1;
    if (node->isLeaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->numKeys++;
    } else {
        while (i >= 0 && key < node->keys[i])
            i--;
        i++;
        if (node->children[i]->numKeys == 2 * node->degree - 1) {
            splitChild(node, i, node->children[i]);
            if (key > node->keys[i])
                i++;
        }
        insertNonFull(node->children[i], key);
    }
}

// Insert a key into the B-tree
BTreeNode* insert(BTreeNode* root, int key) {
    if (!root) {
        root = createNode(DEGREE, true);
        root->keys[0] = key;
        root->numKeys = 1;
        return root;
    }
    if (root->numKeys == 2 * DEGREE - 1) {
        BTreeNode* newRoot = createNode(DEGREE, false);
        newRoot->children[0] = root;
        splitChild(newRoot, 0, root);
        int i = (key > newRoot->keys[0]) ? 1 : 0;
        insertNonFull(newRoot->children[i], key);
        return newRoot;
    }
    insertNonFull(root, key);
    return root;
}

// Merge two children
void merge(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx + 1];
    int degree = node->degree;

    child->keys[degree - 1] = node->keys[idx];

    for (int i = 0; i < sibling->numKeys; i++)
        child->keys[i + degree] = sibling->keys[i];

    if (!child->isLeaf) {
        for (int i = 0; i <= sibling->numKeys; i++)
            child->children[i + degree] = sibling->children[i];
    }
    for (int i = idx + 1; i < node->numKeys; i++)
        node->keys[i - 1] = node->keys[i];

    for (int i = idx + 2; i <= node->numKeys; i++)
        node->children[i - 1] = node->children[i];

    child->numKeys += sibling->numKeys + 1;
    node->numKeys--;

    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

// Remove a key
void removeKey(BTreeNode* root, int key) {
    int idx = 0;
    while (idx < root->numKeys && root->keys[idx] < key)
        idx++;

    if (idx < root->numKeys && root->keys[idx] == key) {
        if (root->isLeaf) {
            for (int i = idx + 1; i < root->numKeys; i++)
                root->keys[i - 1] = root->keys[i];
            root->numKeys--;
        } else if (root->children[idx]->numKeys >= root->degree) {
            BTreeNode* pred = root->children[idx];
            while (!pred->isLeaf)
                pred = pred->children[pred->numKeys];
            int predKey = pred->keys[pred->numKeys - 1];
            root->keys[idx] = predKey;
            removeKey(root->children[idx], predKey);
        } else if (root->children[idx + 1]->numKeys >= root->degree) {
            BTreeNode* succ = root->children[idx + 1];
            while (!succ->isLeaf)
                succ = succ->children[0];
            int succKey = succ->keys[0];
            root->keys[idx] = succKey;
            removeKey(root->children[idx + 1], succKey);
        } else {
            merge(root, idx);
            removeKey(root->children[idx], key);
        }
    } else {
        if (root->isLeaf) return;
        bool flag = (idx == root->numKeys);
        if (root->children[idx]->numKeys < root->degree)
            merge(root, idx);
        if (flag && idx > root->numKeys)
            removeKey(root->children[idx - 1], key);
        else
            removeKey(root->children[idx], key);
    }
}

// Driver function
int main() {
    BTreeNode* root = NULL;

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 5);
    root = insert(root, 6);
    root = insert(root, 12);
    root = insert(root, 30);
    root = insert(root, 7);
    root = insert(root, 17);

    printf("Traversal of the B-tree: ");
    traverse(root);
    printf("\n");

    root = insert(root, 40);
    printf("After inserting 40: ");
    traverse(root);
    printf("\n");

    removeKey(root, 6);
    printf("After removing 6: ");
    traverse(root);
    printf("\n");

    return 0;
}
