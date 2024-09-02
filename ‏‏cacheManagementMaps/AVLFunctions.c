#pragma region avlTreeCompareFunctions
#include <stdio.h>
#include <stdlib.h>
#include "cacheManagement.h"

extern controlBlock_t* controlBlock;

//compare by mapID
int compareMapInfo(const void* a, const void* b) {
	MapInfo_t* mapInfoA = (MapInfo_t*)a;
	MapInfo_t* mapInfoB = (MapInfo_t*)b;
	return mapInfoA->mapID - mapInfoB->mapID;
}

//compare by location
int compareRangeByLocation(const void* a, const void* b) {
	RangeInDataStorage_t* rangeA = (RangeInDataStorage_t*)a;
	RangeInDataStorage_t* rangeB = (RangeInDataStorage_t*)b;
	return rangeA->location - rangeB->location;
}

//compare by sizeOfBytes
int compareRangeBySize(const void* a, const void* b) {
	RangeInDataStorage_t* rangeA = (RangeInDataStorage_t*)a;
	RangeInDataStorage_t* rangeB = (RangeInDataStorage_t*)b;
	return  rangeA->sizeOfBytes - rangeB->sizeOfBytes;
}
#pragma endregion

//get height
int height(AVLNode_t* node) {
	return node ? node->height : 0;
}


//update height
void updateHeight(AVLNode_t* node) {
	int leftHeight = height(node->left);
	int rightHeight = height(node->right);
	node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

int getBalance(AVLNode_t* node) {
	return node ? height(node->left) - height(node->right) : 0;
}

int Abs(int x) {
	return x < 0 ? x * -1 : x;
}

AVLNode_t* rightRotate(AVLNode_t* y) {
	AVLNode_t* x = y->left;
	AVLNode_t* T2 = x->right;

	x->right = y;
	y->left = T2;

	updateHeight(y);
	updateHeight(x);

	return x;
}

AVLNode_t* leftRotate(AVLNode_t* x) {
	AVLNode_t* y = x->right;
	AVLNode_t* T2 = y->left;

	y->left = x;
	x->right = T2;

	updateHeight(x);
	updateHeight(y);

	return y;
}

//recursive function of insertion new node to avlTree
AVLNode_t* insertNode(AVLNode_t* node, void* data, CompareFunc compare) {

	if (node == NULL) {
		AVLNode_t* newNode = (AVLNode_t*)malloc(sizeof(AVLNode_t));
		newNode->data = data;
		newNode->left = newNode->right = NULL;
		newNode->height = 1;
		return newNode;
	}

	int cmp = compare(data, node->data);
	if (cmp < 0) {
		node->left = insertNode(node->left, data, compare);
	}
	else {
		node->right = insertNode(node->right, data, compare);
	}

	updateHeight(node);

	int balance = height(node->left) - height(node->right);

	if (balance > 1 && compare(data, node->left->data) < 0) {
		return rightRotate(node);
	}

	if (balance < -1 && compare(data, node->right->data) > 0) {
		return leftRotate(node);
	}

	if (balance > 1 && compare(data, node->left->data) > 0) {
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if (balance < -1 && compare(data, node->right->data) < 0) {
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

//function of insertion new node to avlTree
void addNode(AVLTree_t* tree, void* data)
{
	tree->root = insertNode(tree->root, data, tree->compare);
}

#pragma region print functions

void printMapInfo(void* data) {
	MapInfo_t* mapInfo = (MapInfo_t*)data;
	printLinkedList(mapInfo->linkedList);
	printf("MapID: %d, MapSizeInBytes: %d\n", mapInfo->mapID, mapInfo->mapSizeInBytes);
}

void inOrderTraversal(AVLNode_t* node, PrintFunc printFunc) {
	if (node != NULL) {
		inOrderTraversal(node->left, printFunc);
		printFunc(node->data);
		inOrderTraversal(node->right, printFunc);
	}
}

void printAVLTree(AVLTree_t* tree, PrintFunc printFunc) {
	inOrderTraversal(tree->root, printFunc);
}

void printRangeInDataStorage(const void* data) {
	RangeInDataStorage_t* rangeData = (RangeInDataStorage_t*)data;
	printf("Location: %d, SizeOfBytes: %d\n", rangeData->location, rangeData->sizeOfBytes);
}

void printMapInfoTree(AVLTree_t* tree) {
	inOrderTraversal(tree->root, printMapInfo);
}

void printRangeInDataStorageTree(AVLTree_t* tree) {
	inOrderTraversal(tree->root, printRangeInDataStorage);
}
#pragma endregion

#pragma region functions to find the next or prev node in tree


AVLNode_t* findMin(AVLNode_t* node) {
	while (node && node->left != NULL) {
		node = node->left;
	}
	return node;
}
// חיפוש האיבר העוקב בעץ
AVLNode_t* findNextNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc) {
	if (currentNode == NULL) return NULL;

	// מקרה 1: אם יש לצומת הנוכחי צומת ימין
	if (currentNode->right != NULL) {
		return findMin(currentNode->right);
	}

	// מקרה 2: אם אין לצומת הנוכחי צומת ימין
	AVLNode_t* successor = NULL;
	AVLNode_t* ancestor = root;

	while (ancestor != NULL) {
		int cmp = compareFunc(currentNode->data, ancestor->data);

		if (cmp < 0) {
			successor = ancestor;
			ancestor = ancestor->left;
		}
		else if (cmp > 0) {
			ancestor = ancestor->right;
		}
		else {
			break;
		}
	}

	return successor;
}




// חיפוש הצומת הגדול ביותר בתת-העץ השמאלי
AVLNode_t* findMax(AVLNode_t* node) {
	while (node && node->right != NULL) {
		node = node->right;
	}
	return node;
}

// חיפוש האיבר הקודם בעץ
AVLNode_t* findPrevNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc) {
	if (currentNode == NULL) return NULL;

	// מקרה 1: אם לצומת הנוכחי יש צומת שמאלי
	if (currentNode->left != NULL) {
		return findMax(currentNode->left);
	}

	// מקרה 2: אם אין לצומת הנוכחי צומת שמאלי
	AVLNode_t* predecessor = NULL;
	AVLNode_t* ancestor = root;

	while (ancestor != NULL) {
		int cmp = compareFunc(currentNode->data, ancestor->data);

		if (cmp < 0) {
			ancestor = ancestor->left;
		}
		else if (cmp > 0) {
			predecessor = ancestor;
			ancestor = ancestor->right;
		}
		else {
			break;
		}
	}

	return predecessor;
}

#pragma endregion

//if we find next or prev node (or both), we want to merge them to one updated node
//void mergeNodes(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc)
//{
//	AVLNode_t* prev = findPrevNode(root, currentNode, compareFunc);
//	AVLNode_t* next = findNextNode(root, currentNode, compareFunc);
//}

//wrap function of delete from emptyPlacesByLocation
void deleteNodeFromEmptyPlacesByLocation(int location)
{
	RecursiveDeleteNodeFromEmptyPlacesByLocation(controlBlock->emptyPlacesByLocation->root, location);
}

//wrap function of delete from emptyPlacesBySize
void deleteNodeFromEmptyPlacesBySize(RangeInDataStorage_t* range)
{

	printf("\nrange->location %d\n", range->location);
	printf("\nrange->sizeOfBytes %d\n", range->sizeOfBytes);

	RecursiveDeleteNodeFromEmptyPlacesBySize(controlBlock->emptyPlacesBySize->root, range);
}

//wrap function of delete from MapsSortedByID
void deleteNodeFromMapsSortedByID(int id)
{
	RucursiveDeleteMapFromMapsSortedByID(controlBlock->MapsSortedByID, id);
}

//recursive function of delete EmptyPlacesByLocation
AVLNode_t* RecursiveDeleteNodeFromEmptyPlacesByLocation(AVLNode_t* root, int locationToSearch) {

	if (root == NULL) {
		return NULL;
	}

	RangeInDataStorage_t* range = (RangeInDataStorage_t*)root->data;
	printf("\nrange->location %d\n", range->location);
	int cmp = locationToSearch - range->location;
	if (cmp < 0) {
		root->left = RecursiveDeleteNodeFromEmptyPlacesByLocation(root->left, locationToSearch);
	}
	else if (cmp > 0) {
		root->right = RecursiveDeleteNodeFromEmptyPlacesByLocation(root->right, locationToSearch);
	}
	else {
		// Node to be deleted
		if (root->left == NULL) {
			AVLNode_t* temp = root->right;
			free(root->data); // Free the data if necessary
			free(root);
			return temp;
		}

		else if (root->right == NULL) {
			AVLNode_t* temp = root->left;
			free(root->data); // Free the data if necessary
			free(root);
			return temp;
		}

		AVLNode_t* temp = findMin(root->right);
		root->data = temp->data;
		root->right = RecursiveDeleteNodeFromEmptyPlacesByLocation(root->right, ((RangeInDataStorage_t*)temp)->location);
	}

	updateHeight(root);

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

//recursive function of delete MapsSortedByID
AVLNode_t* RucursiveDeleteMapFromMapsSortedByID(AVLNode_t* root, int mapID) {

	if (root == NULL) {
		return NULL;
	}

	MapInfo_t* mapInfo = (MapInfo_t*)root->data;

	int cmp = mapID - mapInfo->mapID;
	if (cmp < 0) {
		root->left = RucursiveDeleteMapFromMapsSortedByID(root->left, mapID);
	}
	else if (cmp > 0) {
		root->right = RucursiveDeleteMapFromMapsSortedByID(root->right, mapID);
	}
	else {
		// Node to be deleted
		if (root->left == NULL) {
			AVLNode_t* temp = root->right;
			free(root->data); // Free the data if necessary
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			AVLNode_t* temp = root->left;
			free(root->data); // Free the data if necessary
			free(root);
			return temp;
		}

		AVLNode_t* temp = findMin(root->right);
		root->data = temp->data;
		root->right = RucursiveDeleteMapFromMapsSortedByID(root->right, ((MapInfo_t*)temp->data)->mapID);
	}

	updateHeight(root);

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

//recursive function of delete EmptyPlacesBySize
AVLNode_t* RecursiveDeleteNodeFromEmptyPlacesBySize(AVLNode_t* root, RangeInDataStorage_t* item) {

	if (root == NULL) {
		return NULL;
	}

	RangeInDataStorage_t* range = (RangeInDataStorage_t*)root->data;
	printf("\n- - - - - is enter  \n");

	//printf("\nitem->sizeOfBytes %d\n", item->sizeOfBytes);
	//printf("\nrange->sizeOfBytes %d\n", range->sizeOfBytes);

	//printf("\nitem->location %d\n", item->location);
	//printf("\nrange->location %d\n", range->location);

	int cmp = item->sizeOfBytes - range->sizeOfBytes;


	if (cmp < 0) {
		printf("\ncmp < 0\n");
		root->left = RecursiveDeleteNodeFromEmptyPlacesBySize(root->left, item);
	}
	//if found another object with same size, continue search
	else if (cmp >= 0 && item->location != range->location) {
		printf("\ncmp >= 0 && item->location != range->location\n");
		root->right = RecursiveDeleteNodeFromEmptyPlacesBySize(root->right, item);
	}
	else
	{
		printf("heidad\n");
		// Node to be deleted
		if (root->left == NULL) {
			AVLNode_t* temp = root->right;
			//if (root->data != NULL)
			//{
			//	free(root->data); // Free the data if necessary
				free(root);
			//}
			printf("\n");
			printRangeInDataStorage(temp);
			return temp;
		}

		else if (root->right == NULL) {
			AVLNode_t* temp = root->left;
			//if (root->data != NULL)
			//{
			//	free(root->data); // Free the data if necessary
				free(root);
			//}
			printf("\n");
			printRangeInDataStorage(temp);
			return temp;
		}

		AVLNode_t* temp = findMin(root->right);
		printf("s %d\n", ((RangeInDataStorage_t*)temp->data)->location);
		printf("l %d\n", ((RangeInDataStorage_t*)temp->data)->sizeOfBytes);
		root->data = temp->data;//הבן הימני והאבא שווים
		root->right = RecursiveDeleteNodeFromEmptyPlacesBySize(root->right, ((RangeInDataStorage_t*)temp));
		printRangeInDataStorage(root->right->data);
		printf("\n");
		//printRangeInDataStorageTree(controlBlock->emptyPlacesBySize);
		printf("\n");
		//הוא לא מצליח למחוק את האיבר המינימלי, הוא מכפיל אותו
	}

	updateHeight(root);

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

//update range when I filled only part of the range
void UpdateNodeInRangeByLocation(AVLNode_t* root, AVLNode_t* nodeToUpdate, int updatedSize)
{
	((RangeInDataStorage_t*)&nodeToUpdate->data)->sizeOfBytes = updatedSize;
}

//update range when I filled only part of the range
void UpdateNodeInRangeBySize(AVLNode_t* root, RangeInDataStorage_t* nodeToUpdate, int updatedSize)
{
	RangeInDataStorage_t* r = (RangeInDataStorage_t*)malloc(sizeof(RangeInDataStorage_t)); ;
	r->location = nodeToUpdate->location;
	r->sizeOfBytes = updatedSize;
	deleteNodeFromEmptyPlacesBySize(nodeToUpdate);
	addNode(root, (AVLNode_t*)&r);
}


AVLNode_t* FindRangeByLocation(AVLNode_t* root, int location) {
	if (root == NULL) {
		return NULL;
	}

	RangeInDataStorage_t* range = (RangeInDataStorage_t*)root->data;
	if (location < range->location) {
		return FindRangeByLocation(root->left, location);
	}
	else if (location > range->location) {
		return FindRangeByLocation(root->right, location);
	}
	else {
		// location תואם ל-location של הצומת הנוכחי
		return root;
	}
}

AVLNode_t* FindMapInfoByID(AVLNode_t* root, int id) {
	if (root == NULL) {
		return NULL;
	}

	MapInfo_t* mapinfo = (MapInfo_t*)root->data;
	if (id < mapinfo->mapID) {
		return FindMapInfoByID(root->left, id);
	}
	else if (id > mapinfo->mapID) {
		return FindMapInfoByID(root->right, id);
	}
	else {
		// location תואם ל-location של הצומת הנוכחי
		return root;
	}
}



