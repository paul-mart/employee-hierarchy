#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node Struct
typedef struct TreeNode {
    int id;
    char name[1001];
    struct TreeNode* parent;
    struct TreeNode* children;
    struct TreeNode* next;
} TreeNode;

// Create a new tree node
TreeNode* createNode(int id, const char* name) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->id = id;
    strcpy(node->name, name);
    node->parent = NULL;
    node->children = NULL;
    node->next = NULL;
    return node;
}

// Add a child node to a parent
void addChild(TreeNode* parent, TreeNode* child) {
    if (!parent || !child) {
        fprintf(stderr, "Invalid ADD operation: parent or child is NULL\n");
        return;
    }
    child->parent = parent;
    child->next = parent->children;
    parent->children = child;
}

// Find a node by ID
TreeNode* findNode(TreeNode* root, int id) {
    if (!root) return NULL;
    if (root->id == id) return root;
    TreeNode* child = root->children;
    while (child) {
        TreeNode* result = findNode(child, id);
        if (result) return result;
        child = child->next;
    }
    return NULL;
}

// Remove a node and promote its children
void removeNode(TreeNode* root, int id) {
    TreeNode* node = findNode(root, id);
    if (!node || node->parent == NULL) {
        fprintf(stderr, "Invalid REMOVE operation: node not found or root node cannot be removed\n");
        return;
    }

    TreeNode* parent = node->parent;
    TreeNode* child = node->children;

    if (parent->children == node) {
        parent->children = node->next;
    } else {
        TreeNode* sibling = parent->children;
        while (sibling && sibling->next != node) {
            sibling = sibling->next;
        }
        if (sibling) sibling->next = node->next;
    }

    while (child) {
        TreeNode* nextChild = child->next;
        addChild(parent, child);
        child = nextChild;
    }

    free(node);
}

// Print the children of a node
void printChildren(TreeNode* node) {
    if (!node) {
        fprintf(stderr, "Invalid PRINT operation: node not found\n");
        return;
    }
    TreeNode* child = node->children;
    int first = 1;
    while (child) {
        if (!first) printf(",");
        printf("%s", child->name);
        first = 0;
        child = child->next;
    }
    printf("\n");
}

// Sort children by ID
void sortChildrenById(TreeNode* parent) {
    if (!parent || !parent->children) return;

    for (TreeNode* i = parent->children; i; i = i->next) {
        for (TreeNode* j = i->next; j; j = j->next) {
            if (i->id > j->id) {
                    int tempId = i->id;
                    char tempName[1001];
                    strcpy(tempName, i->name);
    
                    i->id = j->id;
                    strcpy(i->name, j->name);
    
                    j->id = tempId;
                    strcpy(j->name, tempName);
            }
        }
    }
}

// Sort children by name
void sortChildrenByName(TreeNode* parent) {
    if (!parent || !parent->children) return;

    for (TreeNode* i = parent->children; i; i = i->next) {
        for (TreeNode* j = i->next; j; j = j->next) {
            if (strcmp(i->name, j->name) > 0) {
                int tempId = i->id;
                char tempName[1001];
                strcpy(tempName, i->name);

                i->id = j->id;
                strcpy(i->name, j->name);

                j->id = tempId;
                strcpy(j->name, tempName);
            }
        }
    }
}

int main() {
    int n;
    scanf("%d", &n);

    TreeNode* nodes[n];
    TreeNode* root = NULL;

    for (int i = 0; i < n; i++) {
        int id;
        char name[1001];
        scanf("%d %s", &id, name);
        nodes[i] = createNode(id, name);
        if (id == 0) root = nodes[i];  // Set the root node
    }

    char command[20];
    while (scanf("%s", command) != EOF) {
        if (strcmp(command, "ADD") == 0) {
            int x, y;
            scanf("%d %d", &x, &y);
            TreeNode* parent = findNode(root, x);
            TreeNode* child = NULL;
            for (int i = 0; i < n; i++) {
                if (nodes[i]->id == y) {
                    child = nodes[i];
                    break;
                }
            }
            if (!parent || !child) {
                fprintf(stderr, "Invalid ADD operation: parent or child node not found\n");
                continue;
            }
            addChild(parent, child);
        } else if (strcmp(command, "REMOVE") == 0) {
            int x;
            scanf("%d", &x);
            removeNode(root, x);
        } else if (strcmp(command, "SORT_ID") == 0) {
            int x;
            scanf("%d", &x);
            TreeNode* node = findNode(root, x);
            if (node) sortChildrenById(node);
        } else if (strcmp(command, "SORT_NAME") == 0) {
            int x;
            scanf("%d", &x);
            TreeNode* node = findNode(root, x);
            if (node) sortChildrenByName(node);
        } else if (strcmp(command, "PRINT") == 0) {
            int x;
            scanf("%d", &x);
            TreeNode* node = findNode(root, x);
            printChildren(node);
        }
    }

    return 0;
}
