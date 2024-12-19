#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct treenode {
    struct treenode* firstchild;
    struct treenode* sibling;
    int ID;
    char name[1001];
} treenode;

treenode* createNode(int idnumber, char employeename[])
{
    treenode* new = (treenode*)malloc(sizeof(treenode)); // allocate space for node
    if(new != NULL) {  //if allocation successful, do this:
    new->ID = idnumber;
    strncpy(new->name, employeename, sizeof(new->name) - 1);
    new->name[sizeof(new->name) - 1] = '\0'; // Ensure null-termination
    new->firstchild = NULL;
    new->sibling = NULL;
    }
    
    return new; //return node or NULL depending on if allocation worked

}


// Find a node by it's ID in the tree
treenode* findNode(treenode* root, int targetID)
{
    if(root == NULL) { return NULL; } // if no root
    if(root->ID == targetID) { return root; } // found employee!

    // Recursively search the 1st child
    treenode* res = findNode(root->firstchild, targetID);
    if (res != NULL) return res; // If found in children, return it
    //If not, ->
    // Search for siblings recursively
    return findNode(root->sibling, targetID);
}

// Add a node Y as a child of X 
void addNode(treenode* parent, treenode* child) {
    if (parent->firstchild == NULL) { // if the parent doesnt have any children
        parent->firstchild = child; } // child we wanted to add is now the 1st child
     else //parent has a child or children
     {
        treenode* temp = parent->firstchild; // make temp node ptr to first child of parent
        while (temp->sibling != NULL) { // loop through all children until reaching end
            temp = temp->sibling; // Move to the last sibling
        }
        temp->sibling = child; // Add as a sibling
    }
}

// Function to find a node and its parent
void findNodeAndParent(treenode* root, int targetID, treenode** parent, treenode** target) {
    if (root == NULL) return;

    treenode* current = root->firstchild;  // Start with the first child
    while (current != NULL) {
        if (current->ID == targetID) {
            *target = current;
            return;
        }
        if (current->firstchild != NULL) {
            *parent = root;
            findNodeAndParent(current, targetID, parent, target);
        }
        current = current->sibling;  // Move to siblings
    }
}

// Function to remove a node
void removeNode(treenode* root, int targetID) {
    if (root == NULL || targetID == 0) return;  // Do not remove root or handle NULL tree

    treenode* parent = NULL;
    treenode* target = NULL;

    // Step 1: Find the target node and its parent
    findNodeAndParent(root, targetID, &parent, &target);
    if (target == NULL) return;  // Node not found

    // Step 2: Handle case where target has no children
    if (target->firstchild == NULL) {
        // Unlink target from parent
        if (parent->firstchild == target) {
            parent->firstchild = target->sibling;
        } else {
            treenode* temp = parent->firstchild;
            while (temp->sibling != target) temp = temp->sibling;
            temp->sibling = target->sibling;
        }
        free(target);  // Free the target node
        return;
    }

    // Step 3: Promote the child with the smallest ID
    treenode* promoted = target->firstchild;
    treenode* prev = NULL;
    treenode* temp = target->firstchild;

    // Find the child with the smallest ID
    while (temp != NULL) {
        if (temp->ID < promoted->ID) {
            promoted = temp;
        }
        temp = temp->sibling;
    }

    // Step 4: Rebuild the siblings excluding the promoted child
    temp = target->firstchild;
    treenode* newSiblings = NULL;
    treenode* lastSibling = NULL;

    while (temp != NULL) {
        if (temp != promoted) {
            if (newSiblings == NULL) {
                newSiblings = temp;
                lastSibling = temp;
            } else {
                lastSibling->sibling = temp;
                lastSibling = temp;
            }
        }
        temp = temp->sibling;
    }
    if (lastSibling != NULL) lastSibling->sibling = NULL;

    // Step 5: Relink promoted child
    promoted->sibling = target->sibling;  // Promoted takes over target's position
    promoted->firstchild = newSiblings;   // Attach remaining children as siblings

    if (parent->firstchild == target) {
        parent->firstchild = promoted;
    } else {
        temp = parent->firstchild;
        while (temp->sibling != target) temp = temp->sibling;
        temp->sibling = promoted;
    }

    free(target);  // Free the removed node

}

void printChildrenOf(treenode* parent)
{
    if(parent->firstchild == NULL) { return;}
    
    treenode curr* = parent->firstchild;
    
    printf("%s", curr);
    
}

void executeCommand(treenode* root, char command[], int x, int y) {
    if (strcmp(command, "ADD") == 0) {
        treenode* parent = findNode(root, x);
        treenode* child = createNode(y, "Placeholder"); // Replace with actual name from input
        addNode(parent, child);
    } else if (strcmp(command, "REMOVE") == 0) {
        removeNode(root, x);
    } else if (strcmp(command, "MOVE") == 0) {
        treenode* parent = findNode(root, x);
        treenode* child = findNode(root, y);
        moveNode(parent, child);
    } else if (strcmp(command, "SORT_ID") == 0) {
        treenode* parent = findNode(root, x);
        sortbyID(parent);
    } else if (strcmp(command, "SORT_NAME") == 0) {
        treenode* parent = findNode(root, x);
        sortByName(parent);
    } else if (strcmp(command, "PRINT") == 0) {
        treenode* parent = findNode(root, x);
        treenode* temp = parent->firstchild;
        while (temp != NULL) {
            printf("%s", temp->name);
            if (temp->sibling != NULL) printf(",");
            temp = temp->sibling;
        }
        printf("\n");
    }
}

void sortByName(treenode* parent) {
    if (parent == NULL || parent->firstchild == NULL) return;

    treenode* childList[100]; // Adjust size or dynamically allocate if necessary
    int count = 0;

    // Collect all children in an array
    treenode* temp = parent->firstchild;
    while (temp != NULL) {
        childList[count++] = temp;
        temp = temp->sibling;
    }

    // Sort array by name
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (strcmp(childList[i]->name, childList[j]->name) > 0) {
                treenode* swap = childList[i];
                childList[i] = childList[j];
                childList[j] = swap;
            }
        }
    }

    // Rebuild sibling links
    parent->firstchild = childList[0];
    for (int i = 0; i < count - 1; ++i) {
        childList[i]->sibling = childList[i + 1];
    }
    childList[count - 1]->sibling = NULL;
}

// Function to sort children by ID
void sortbyID(treenode* parent) {
    if (parent == NULL || parent->firstchild == NULL) return; //if parents and kid are null

    treenode* childList[100]; // array of children
    int count = 0;

    // First, put all children into array
    treenode* temp = parent->firstchild;
    while (temp != NULL) {
        childList[count++] = temp;
        temp = temp->sibling;
    }

    // Sort array by ID
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (childList[i]->ID > childList[j]->ID) {
                treenode* swap = childList[i];
                childList[i] = childList[j];
                childList[j] = swap;
            }
        }
    }

    // Rebuild sibling links
    parent->firstchild = childList[0];
    for (int i = 0; i < count - 1; ++i) {
        childList[i]->sibling = childList[i + 1];
    }
    childList[count - 1]->sibling = NULL;
}

int main()
{
    int num_employees;
    
    scanf("%d",&num_employees);

    treenode* root = createNode(0, "CEO");
    
    
    treenode* root = createNode(0, "CEO");

    // Read employees and commands
    for (int i = 0; i < num_employees; i++) {
        int id;
        char name[1001];
        scanf("%d %1000s", &id, name);
        if (id == 0) continue; // Root already created
        treenode* node = createNode(id, name);
        // Add node to the root as a temporary placeholder
        addNode(root, node);
    }

    char command[10];
    int x, y;
    while (scanf("%s", command) != EOF) {
        if (strcmp(command, "ADD") == 0 || strcmp(command, "MOVE") == 0) {
            scanf("%d %d", &x, &y);
            executeCommand(root, command, x, y);
        } else if (strcmp(command, "REMOVE") == 0 || strcmp(command, "SORT_ID") == 0 || strcmp(command, "SORT_NAME") == 0 || strcmp(command, "PRINT") == 0) {
            scanf("%d", &x);
            executeCommand(root, command, x, 0);
        }
    }

    
    
    return 0;
}