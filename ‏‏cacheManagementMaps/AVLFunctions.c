#pragma region avlTreeCompareFunctions
#include <stdio.h>
#include <stdlib.h>
#include "cacheManagement.h"

extern controlBlock_t* controlBlock;
RangeInDataStorage_t r;

#pragma region compare functions

//compare by mapID
int CompareMapInfo(const void* a, const void* b) {
	MapInfo_t* mapInfoA = (MapInfo_t*)a;
	MapInfo_t* mapInfoB = (MapInfo_t*)b;
	return mapInfoA->mapID - mapInfoB->mapID;
}

//compare by location
int CompareRangeByLocation(const void* a, const void* b) {
	RangeInDataStorage_t* rangeA = (RangeInDataStorage_t*)a;
	RangeInDataStorage_t* rangeB = (RangeInDataStorage_t*)b;
	return rangeA->location - rangeB->location;
}

//compare by sizeOfBytes
int CompareRangeBySize(const void* a, const void* b) {
	RangeInDataStorage_t* rangeA = (RangeInDataStorage_t*)a;
	RangeInDataStorage_t* rangeB = (RangeInDataStorage_t*)b;
	if (rangeA->sizeOfBytes - rangeB->sizeOfBytes != 0)
	{
		return rangeA->sizeOfBytes - rangeB->sizeOfBytes;
	}
	else if (rangeA->location - rangeB->location == 0)
	{
		return 0;
	}
	else {
		return rangeA->location - rangeB->location;
	}
}
#pragma endregion

int Abs(int x) {
	return x < 0 ? x * -1 : x;
}

#pragma region function for keep the tree balanced 

//get Height
int Height(AVLNode_t* node) {
	return node ? node->height : 0;
}

//update Height
void UpdateHeight(AVLNode_t* node) {
	int leftHeight = Height(node->left);
	int rightHeight = Height(node->right);
	node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

int GetBalance(AVLNode_t* node) {
	return node ? Height(node->left) - Height(node->right) : 0;
}

AVLNode_t* RightRotate(AVLNode_t* y) {
	AVLNode_t* x = y->left;
	AVLNode_t* T2 = x->right;

	x->right = y;
	y->left = T2;

	UpdateHeight(y);
	UpdateHeight(x);

	return x;
}

AVLNode_t* LeftRotate(AVLNode_t* x) {
	AVLNode_t* y = x->right;
	AVLNode_t* T2 = y->left;

	y->left = x;
	x->right = T2;

	UpdateHeight(x);
	UpdateHeight(y);

	return y;
}
#pragma endregion

//function of insertion new node to avlTree, calls insertNode
void AddNodeToAvlTree(AVLTree_t* tree, void* data)
{
	tree->root = InsertNode(tree->root, data, tree->compare);
}

//recursive function of insertion new node to avlTree
AVLNode_t* InsertNode(AVLNode_t* node, void* data, CompareFunc compare) 
{
	if (node == NULL) {
		AVLNode_t* newNode = (AVLNode_t*)malloc(sizeof(AVLNode_t));
		newNode->data = data;
		newNode->left = newNode->right = NULL;
		newNode->height = 1;
		return newNode;
	}
	int cmp = compare(data, node->data);
	if (cmp < 0)
		node->left = InsertNode(node->left, data, compare);
	else
		node->right = InsertNode(node->right, data, compare);
	UpdateHeight(node);
	int balance = Height(node->left) - Height(node->right);
	if (balance > 1 && compare(data, node->left->data) < 0)
		return RightRotate(node);
	if (balance < -1 && compare(data, node->right->data) > 0)
		return LeftRotate(node);
	if (balance > 1 && compare(data, node->left->data) > 0) {
		node->left = LeftRotate(node->left);
		return RightRotate(node);
	}
	if (balance < -1 && compare(data, node->right->data) < 0) {
		node->right = RightRotate(node->right);
		return LeftRotate(node);
	}
	return node;
}

#pragma region Delete functions
//wrap function of Delete from emptyPlacesByLocation
void DeleteNodeFromEmptyPlacesByLocation(int location)
{
	RecursiveDeleteNodeFromEmptyPlacesByLocation(controlBlock->emptyPlacesByLocation->root, location);
}

//wrap function of Delete from emptyPlacesBySize
void DeleteNodeFromEmptyPlacesBySize(RangeInDataStorage_t* range)
{
	RecursiveDeleteNodeFromEmptyPlacesBySize(controlBlock->emptyPlacesBySize->root, range);
}

//wrap function of Delete from MapsSortedByID
void DeleteNodeFromMapsSortedByID(int id)
{
	RecursiveDeleteMapFromMapsSortedByID(controlBlock->MapsSortedByID->root, id);
}

//recursive function of Delete EmptyPlacesByLocation
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
		// Node to be Deleted
		if (root->left == NULL) {
			AVLNode_t* temp = root->right;
			//free(root->data); // Free the data if necessary
			//free(root);
			return temp;
		}
		else if (root->right == NULL) {
			AVLNode_t* temp = root->left;
			//free(root->data); // Free the data if necessary
			//free(root);
			return temp;
		}
		AVLNode_t* temp = FindMin(root->right);
		root->data = temp->data;
		root->right = RecursiveDeleteNodeFromEmptyPlacesByLocation(root->right, ((RangeInDataStorage_t*)temp->data)->location);
	}
	UpdateHeight(root);
	int balance = GetBalance(root);
	if (balance > 1 && GetBalance(root->left) >= 0)
		return RightRotate(root);
	if (balance > 1 && GetBalance(root->left) < 0) {
		root->left = LeftRotate(root->left);
		return RightRotate(root);
	}
	if (balance < -1 && GetBalance(root->right) <= 0)
		return LeftRotate(root);
	if (balance < -1 && GetBalance(root->right) > 0) {
		root->right = RightRotate(root->right);
		return LeftRotate(root);
	}
	return root;
}

//recursive function of Delete map from MapsSortedByID tree 
AVLNode_t* RecursiveDeleteMapFromMapsSortedByID(AVLNode_t* root, int mapID) {

	if (root == NULL) {
		return NULL;
	}
	MapInfo_t* mapInfo = (MapInfo_t*)root->data;
	int cmp = mapID - mapInfo->mapID;
	if (cmp < 0)
		root->left = RecursiveDeleteMapFromMapsSortedByID(root->left, mapID);
	else if (cmp > 0)
		root->right = RecursiveDeleteMapFromMapsSortedByID(root->right, mapID);
	else {
		// Node to be Deleted
		if (root->left == NULL) {
			AVLNode_t* temp = root->right;
			//free(root->data); // Free the data if necessary
			//free(root);
			return temp;
		}
		else if (root->right == NULL) {
			AVLNode_t* temp = root->left;
			//free(root->data); // Free the data if necessary
			//free(root);
			return temp;
		}
		AVLNode_t* temp = FindMin(root->right);
		root->data = temp->data;
		root->right = RecursiveDeleteMapFromMapsSortedByID(root->right, ((MapInfo_t*)temp->data)->mapID);
	}
	UpdateHeight(root);
	int balance = GetBalance(root);
	if (balance > 1 && GetBalance(root->left) >= 0)
		return RightRotate(root);
	if (balance > 1 && GetBalance(root->left) < 0) {
		root->left = LeftRotate(root->left);
		return RightRotate(root);
	}
	if (balance < -1 && GetBalance(root->right) <= 0)
		return LeftRotate(root);
	if (balance < -1 && GetBalance(root->right) > 0) {
		root->right = RightRotate(root->right);
		return LeftRotate(root);
	}
	return root;
}

//recursive function of Delete EmptyPlacesBySize
AVLNode_t* RecursiveDeleteNodeFromEmptyPlacesBySize(AVLNode_t* root, RangeInDataStorage_t* item) {
	if (root == NULL) {
		return NULL;
	}
	RangeInDataStorage_t* range = (RangeInDataStorage_t*)root->data;
	int cmp = item->sizeOfBytes - range->sizeOfBytes;
	if (cmp < 0) {
		root->left = RecursiveDeleteNodeFromEmptyPlacesBySize(root->left, item);
	}
	//if found another object with same size, continue search
	else if (cmp >= 0 && item->location != range->location) {
		root->right = RecursiveDeleteNodeFromEmptyPlacesBySize(root->right, item);
	}
	else
	{
		// Node to be Deleted
		if (root->left == NULL) {
			AVLNode_t* temp = root->right;
			//if (root->data != NULL)
		//{
		//	free(root->data); // Free the data if necessary
			//free(root);
		//}

			return temp;
		}

		else if (root->right == NULL) {
			AVLNode_t* temp = root->left;
			//if (root->data != NULL)
			//{
			//	free(root->data); // Free the data if necessary
				//free(root);
			//}
			return temp;
		}

		AVLNode_t* temp = FindMin(root->right);
		root->data = temp->data;//הבן הימני והאבא שווים
		root->right = RecursiveDeleteNodeFromEmptyPlacesBySize(root->right, (RangeInDataStorage_t*)(temp->data));
		//printRangeInDataStorage(root->right->data);
		//PrintRangeInDataStorageTree(controlBlock->emptyPlacesBySize);
	}

	UpdateHeight(root);
	int balance = GetBalance(root);
	if (balance > 1 && GetBalance(root->left) >= 0)
		return RightRotate(root);
	if (balance > 1 && GetBalance(root->left) < 0) {
		root->left = LeftRotate(root->left);
		return RightRotate(root);
	}
	if (balance < -1 && GetBalance(root->right) <= 0)
		return LeftRotate(root);
	if (balance < -1 && GetBalance(root->right) > 0) {
		root->right = RightRotate(root->right);
		return LeftRotate(root);
	}
	return root;
}

#pragma endregion

#pragma region Find & update functions
//Find range node in the tree sorted
AVLNode_t* FindRangeByLocation(AVLNode_t* root, int location) {
	if (root == NULL) {
		return NULL;
	}
	RangeInDataStorage_t* range = (RangeInDataStorage_t*)root->data;
	if (location < range->location)
		return FindRangeByLocation(root->left, location);
	else if (location > range->location)
		return FindRangeByLocation(root->right, location);
	else		// the location of current node same to location i got
		return root;
}

AVLNode_t* FindMapInfoByID(AVLNode_t* root, int id) {
	if (root == NULL)
		return NULL;
	MapInfo_t* mapinfo = (MapInfo_t*)root->data;
	if (id < mapinfo->mapID)
		return FindMapInfoByID(root->left, id);
	else if (id > mapinfo->mapID)
		return FindMapInfoByID(root->right, id);
	else // the current node id same to id i got
		return root;
}

//update range when I filled only part of the range
void UpdateNodeInRangeByLocation(AVLNode_t* nodeToUpdate, int updatedSize)
{
	((RangeInDataStorage_t*)&nodeToUpdate->data)->sizeOfBytes = updatedSize;
}

//update range when I filled only part of the range
void UpdateNodeInRangeBySize(RangeInDataStorage_t* nodeToUpdate, int updatedSize)
{
	r.location = nodeToUpdate->location;
	r.sizeOfBytes = updatedSize;
	DeleteNodeFromEmptyPlacesBySize(nodeToUpdate);
	PrintRangeInDataStorageTree(controlBlock->emptyPlacesBySize);
	AddNodeToAvlTree(controlBlock->emptyPlacesBySize, (AVLNode_t*)&r);
	nodeToUpdate->sizeOfBytes = updatedSize;
	//AddNodeToAvlTree(controlBlock->emptyPlacesBySize, (AVLNode_t*)nodeToUpdate);
	PrintRangeInDataStorageTree(controlBlock->emptyPlacesBySize);
}
#pragma endregion

// run on maps tree and free each node memory
void FreeMapInfoTree(AVLNode_t* root)
{
	if (root == NULL)
		return;
	if (root->left != NULL)
		FreeMapInfoTree(root->left);
	if (root->right != NULL)
		FreeMapInfoTree(root->right);
	DeleteNodeFromMapsSortedByID(((MapInfo_t*)((AVLNode_t*)(root->data)))->mapID);
}

#pragma region functions to Find the next or prev node in tree

//Find the minimal value in tree
AVLNode_t* FindMin(AVLNode_t* node) {
	while (node && node->left != NULL) {
		node = node->left;
	}
	return node;
}

// חיפוש האיבר העוקב בעץ
//AVLNode_t* FindNextNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc) 
//{
//if (currentNode == NULL) return NULL;
//
//	// מקרה 1: אם יש לצומת הנוכחי צומת ימין
//	if (currentNode->right != NULL) {
//		return FindMin(currentNode->right);
//	}
//
//	// מקרה 2: אם אין לצומת הנוכחי צומת ימין
//	AVLNode_t* successor = NULL;
//	AVLNode_t* ancestor = root;
//
//	while (ancestor != NULL) {
//		int cmp = compareFunc(currentNode->data, ancestor->data);
//
//		if (cmp < 0) {
//			successor = ancestor;
//			ancestor = ancestor->left;
//		}
//		else if (cmp > 0) {
//			ancestor = ancestor->right;
//		}
//		else {
//			break;
//		}
//	}
//
//	return successor;
//}




//Find the maximal value in tree
AVLNode_t* FindMax(AVLNode_t* node) {
	while (node && node->right != NULL) {
		node = node->right;
	}
	return node;
}

// חיפוש האיבר הקודם בעץ
//AVLNode_t* FindPrevNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc) {
//	if (currentNode == NULL) return NULL;
//
//	// מקרה 1: אם לצומת הנוכחי יש צומת שמאלי
//	if (currentNode->left != NULL) {
//		return FindMax(currentNode->left);
//	}
//
//	// מקרה 2: אם אין לצומת הנוכחי צומת שמאלי
//	AVLNode_t* predecessor = NULL;
//	AVLNode_t* ancestor = root;
//
//	while (ancestor != NULL) {
//		int cmp = compareFunc(currentNode->data, ancestor->data);
//
//		if (cmp < 0) {
//			ancestor = ancestor->left;
//		}
//		else if (cmp > 0) {
//			predecessor = ancestor;
//			ancestor = ancestor->right;
//		}
//		else {
//			break;
//		}
//	}
//
//	return predecessor;
//}

#pragma endregion

//if we Find next or prev node (or both), we want to merge them to one updated node
//void mergeNodes(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc)
//{
//	AVLNode_t* prev = FindPrevNode(root, currentNode, compareFunc);
//	AVLNode_t* next = FindNextNode(root, currentNode, compareFunc);
//}

#pragma region print functions

void PrintMapInfo(const void* data) {
	MapInfo_t* mapInfo = (MapInfo_t*)data;
	PrintLinkedList(mapInfo->linkedList);
	printf("MapID: %d, MapSizeInBytes: %d\n", mapInfo->mapID, mapInfo->mapSizeInBytes);
}

void InOrderTraversal(AVLNode_t* node, PrintFunc printFunc) {
	if (node != NULL) {
		InOrderTraversal(node->left, printFunc);
		printFunc(node->data);
		InOrderTraversal(node->right, printFunc);
	}
}

void PrintAVLTree(AVLTree_t* tree, PrintFunc printFunc) {
	InOrderTraversal(tree->root, printFunc);
}

void PrintRangeInDataStorage(const void* data) {
	RangeInDataStorage_t* rangeData = (RangeInDataStorage_t*)data;
	printf("Location: %d, SizeOfBytes: %d\n", rangeData->location, rangeData->sizeOfBytes);
}

void PrintMapInfoTree(AVLTree_t* tree) {
	printf("printMapInfoTree occurred\n");
	InOrderTraversal(tree->root, PrintMapInfo);
}

void PrintRangeInDataStorageTree(AVLTree_t* tree) {
	printf("PrintRangeInDataStorageTree occurred\n");
	InOrderTraversal(tree->root, PrintRangeInDataStorage);
}
#pragma endregion

