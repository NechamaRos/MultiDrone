#include "Disk_Mng_Master.h"

// Global pointer to the Disk Management Control Block
Disk_Managment_CB_t* disk_mng_CB;

void* allocate_memory(size_t size, const char* description, const char* functionName) {
    void* ptr = malloc(size);
    if (!ptr) {
        perror(description);
        exit(EXIT_FAILURE);
    }
    return ptr;
}
void test_writeExceptionToFile(Exception exception, const char* source) {
    FILE* file;
    errno_t err = fopen_s(&file, "errors.log.txt", "a");
    if (err != 0) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    const char* error_message;
    switch (exception) {
    case Error_When_Allocating_Memory_Space:
        error_message = "Error: Error when allocating memory space.";
        break;
    default:
        error_message = "Error: Unknown exception.";
        break;
    }

    fprintf(file, "%s:\n%s\n", source, error_message);
    fclose(file);
}


void disk_mng_initialize_CB() {
    disk_mng_CB = (Disk_Managment_CB_t*)allocate_memory(sizeof(Disk_Managment_CB_t), "Failed to allocate memory for disk control block", "disk_mng_initialize_CB");
}

void disk_mng_initialize() {
    disk_mng_initialize_CB();
    avlTree_firstInitialize();
}

int height(AVLNode_t* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

//AVL node info
AVLNodeInfo_t* avlNodeInfo_create(int mapSize, int arrayIndex, int lru)
{
    AVLNodeInfo_t* newAVLNodeInfo = (AVLNodeInfo_t*)allocate_memory(sizeof(AVLNodeInfo_t), "Failed to allocate memory for AVL node info", "avlNodeInfo_create");
    newAVLNodeInfo->mapSize = mapSize;
    newAVLNodeInfo->arrayIndex = arrayIndex;
    newAVLNodeInfo->lru = lru;
    return newAVLNodeInfo;
}

void avlNodeInfo_delete(AVLNodeInfo_t* nodeInfo)
{
    if (nodeInfo != NULL) {
        free(nodeInfo);
    }
}

//AVL node 
AVLNode_t* avlNode_create(AVLNodeInfo_t* avlNodeInfo) {
    AVLNode_t* node = (AVLNode_t*)allocate_memory(sizeof(AVLNode_t), "Failed to allocate memory for AVL node", "avlTree_insert");
    node->avlNodeInfo = avlNodeInfo;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}
void avlNode_delete(AVLNode_t* node) {
    if (node != NULL) {
        avlNodeInfo_delete(node->avlNodeInfo);
        free(node);
    }
}


AVLNode_t* rightRotate(AVLNode_t* y) {
    AVLNode_t* x = y->left;
    AVLNode_t* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode_t* leftRotate(AVLNode_t* x) {
    AVLNode_t* y = x->right;
    AVLNode_t* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(AVLNode_t* N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
AVLNode_t* avlTree_minValueNode(AVLNode_t* node) {
    AVLNode_t* current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}
AVLNode_t* avlTree_insert(AVLNode_t* node, AVLNodeInfo_t* data, int lruCounter) {
    if (node == NULL) 
    {
        data->lru = lruCounter;
        return avlNode_create(data);
    }

    if (data->mapSize < node->avlNodeInfo->mapSize)
        node->left = avlTree_insert(node->left, data, lruCounter);
    else if (data->mapSize > node->avlNodeInfo->mapSize)
        node->right = avlTree_insert(node->right, data, lruCounter);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && data->mapSize < node->left->avlNodeInfo->mapSize)
        return rightRotate(node);

    if (balance < -1 && data->mapSize > node->right->avlNodeInfo->mapSize)
        return leftRotate(node);

    if (balance > 1 && data->mapSize > node->left->avlNodeInfo->mapSize) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && data->mapSize < node->right->avlNodeInfo->mapSize) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void avlTree_firstInitialize() {
    disk_mng_CB->disk_SortByMapSize = (DiskSortByMapSize_t*)allocate_memory(sizeof(DiskSortByMapSize_t), "Failed to allocate memory for AVL Tree", "avlTree_initialize");
    disk_mng_CB->disk_SortByMapSize->root = NULL;
    disk_mng_CB->disk_SortByMapSize->totalElements = 0;
    disk_mng_CB->disk_SortByMapSize->lruCounter = 0;
}

void avlTree_insertElement(AVLNodeInfo_t* data) {
    disk_mng_CB->disk_SortByMapSize->lruCounter++;
    disk_mng_CB->disk_SortByMapSize->root = avlTree_insert(disk_mng_CB->disk_SortByMapSize->root, data, disk_mng_CB->disk_SortByMapSize->lruCounter);
    disk_mng_CB->disk_SortByMapSize->totalElements++;
}


AVLNode_t* findEligibleForDeletion(AVLNode_t* node) {
    if (node == NULL) {
        return NULL;
    }

    AVLNode_t* eligibleNode = NULL;

    // Check the right subtree first for the largest node
    AVLNode_t* rightResult = findEligibleForDeletion(node->right);
    if (rightResult != NULL && rightResult->avlNodeInfo->lru <= disk_mng_CB->disk_SortByMapSize->lruCounter * 0.7) {
        eligibleNode = rightResult;
    }

    // Check if the current node is eligible for deletion
    if (node->avlNodeInfo->lru <= disk_mng_CB->disk_SortByMapSize->lruCounter * 0.7) {
        if (eligibleNode == NULL || node->avlNodeInfo->mapSize > eligibleNode->avlNodeInfo->mapSize) {
            eligibleNode = node;
        }
    }

    // Check the left subtree if no eligible node found in the right subtree or current node
    AVLNode_t* leftResult = findEligibleForDeletion(node->left);
    if (leftResult != NULL && leftResult->avlNodeInfo->lru <= disk_mng_CB->disk_SortByMapSize->lruCounter * 0.7) {
        if (eligibleNode == NULL || leftResult->avlNodeInfo->mapSize > eligibleNode->avlNodeInfo->mapSize) {
            eligibleNode = leftResult;
        }
    }

    return eligibleNode;
}
AVLNode_t* avlTree_deleteNode(AVLNode_t* root, AVLNode_t* node) {
    if (root == NULL) {
        return root;
    }

    // Traverse the tree to find the node
    if (node->avlNodeInfo->mapSize < root->avlNodeInfo->mapSize) {
        root->left = avlTree_deleteNode(root->left, node->avlNodeInfo->mapSize);
    }
    else if (node->avlNodeInfo->mapSize > root->avlNodeInfo->mapSize) {
        root->right = avlTree_deleteNode(root->right, node->avlNodeInfo->mapSize);
    }
    else {
        // Node with mapSize found
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode_t* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
            }
            free(temp->avlNodeInfo); // Free AVLNodeInfo_t
            free(temp); // Free AVLNode_t
        }
        else {
            AVLNode_t* temp = avlTree_minValueNode(root->right);
            root->avlNodeInfo = temp->avlNodeInfo;
            root->right = avlTree_deleteNode(root->right, temp->avlNodeInfo->mapSize);
        }
    }

    if (root == NULL) {
        return root;
    }

    // Update height and balance the node
    root->height = max(height(root->left), height(root->right)) + 1;
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) {
        return rightRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0) {
        return leftRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

