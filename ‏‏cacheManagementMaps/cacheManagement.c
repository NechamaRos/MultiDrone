#include <stdio.h>
#include <stdlib.h>
#include "cacheManagement.h"

extern controlBlock_t* controlBlock;

void removeMapFromCache(int id)
{
	//The map I search
	AVLNode_t* map = FindMapInfoByID(controlBlock->MapsSortedByID->root, id);

	//If the map is found
	if (map->data != NULL)
	{
		MapInfo_t* mi = (MapInfo_t*)malloc(sizeof(MapInfo_t));
		mi->linkedList = ((MapInfo_t*)map->data)->linkedList;
		mi->mapID = ((MapInfo_t*)map->data)->mapID;
		mi->mapSizeInBytes = ((MapInfo_t*)map->data)->mapSizeInBytes;


		//delete the map from MapsSortedByID, and free the linkedList
		deleteNodeFromMapsSortedByID(mi->mapID);
		freeLinkedList(mi->linkedList);

		//increase the counter of empty places in cache
		controlBlock->EmptyPlaceInCache += mi->mapSizeInBytes;
		free(mi);

	}
}

ValuesOfReadingMap_t ReadPieceOfMap(Node_t* linkedListOfLocations, int size, int offset, int id)
{
	//The head of binaryTree of empty places - the max empty place
	RangeInDataStorage_t* placeToInsert = findMax(controlBlock->emptyPlacesBySize);

	//item of reading map state
	ValuesOfReadingMap_t mapReadingInfo;

	AVLNode_t* emptyPlaceInLocations;

	//If the size of the map is larger than the empty space to insert
	if (placeToInsert->sizeOfBytes > size)
	{
		//remove the empty place from the empty places sorted by size
		deleteNodeFromEmptyPlacesBySize(placeToInsert);

		//find the empty place in emptyPlacesByLocation
		emptyPlaceInLocations = FindRangeByLocation(controlBlock->emptyPlacesByLocation, placeToInsert->location);

		//remove the empty place from the empty places sorted by location
		deleteNodeFromEmptyPlacesByLocation(((RangeInDataStorage_t*)&emptyPlaceInLocations->data)->location);

		//add node of location and size to the map locations linkedList
		insertInTail(linkedListOfLocations, placeToInsert->location, placeToInsert->sizeOfBytes);

		//fetch of insert - id, offset of map, location in cache, and size of bytes  
		fetchFromDisk(id, offset, placeToInsert->location, placeToInsert->sizeOfBytes);

		//set values of state of reading map

		//linkedList = linked list with new location
		mapReadingInfo.linkedList = linkedListOfLocations;

		//offset = offset + the bytes read
		mapReadingInfo.offset = offset + placeToInsert->sizeOfBytes;

		return mapReadingInfo;
	}
	else if (size > placeToInsert->sizeOfBytes)
	{
		//update the max-size node in emptyPlacesBySize with the new size
		UpdateNodeInRangeBySize(controlBlock->emptyPlacesBySize, controlBlock->emptyPlacesBySize, placeToInsert->sizeOfBytes - size);

		//find the empty place in emptyPlacesByLocation
		emptyPlaceInLocations = FindRangeByLocation(controlBlock->emptyPlacesByLocation, placeToInsert->location);
		
		//update the max-size node in emptyPlacesByLocation with the new size
		UpdateNodeInRangeByLocation(controlBlock->emptyPlacesByLocation, emptyPlaceInLocations, placeToInsert->sizeOfBytes - size);

		//add node of location and size to the map locations linkedList
		insertInTail(linkedListOfLocations, placeToInsert->location, size);

		//fetch of insert - id, offset of map, location in cache, and size of bytes  
		fetchFromDisk(id, offset, placeToInsert->location, size);

		//set values of state of reading map

		//linkedList = linked list with new location
		mapReadingInfo.linkedList = linkedListOfLocations;

		//offset = offset + the bytes read
		mapReadingInfo.offset = offset + size;

		return mapReadingInfo;
	}
}

void ReadNormalMap(int id,int size)
{	
	MapInfo_t* newMap = (MapInfo_t*)malloc(sizeof(MapInfo_t));
	Node_t* linkedListOfLocations = createNode(id,size);
	int offset = 0;

	//Make sure there is enough space in the cache
	MakesRoomForCurrentIncome(size);
	
	//As long as I haven't finished loading the image
	while (size > offset)
	{
		ValuesOfReadingMap_t readingInfo = ReadPieceOfMap(linkedListOfLocations,size,offset,id);
		offset = readingInfo.offset;
		linkedListOfLocations = readingInfo.linkedList;
	}

	//set object to contain my map
	newMap->linkedList = linkedListOfLocations;
	newMap->mapID = id;
	newMap->mapSizeInBytes = size;

	//insert the map to my structures:

	//insert the map to MapsSortedByID
	addNode(controlBlock->MapsSortedByID,newMap);

	//insert the map to QueueArray
	insertToQueueArray(newMap);
}

void fetchFromDisk(int id,int offset, int location, int size)
{
	return;
}

void overrideInternal(int size) 
{
	MapInfo_t* temp = (MapInfo_t*)malloc(sizeof(MapInfo_t));
	int counter = 0;
	while (size > counter)
	{
		freeLinkedList(temp->linkedList);

		temp = removeMaxMapFromQueueArray();
		if (temp != NULL)
		{
			counter += temp->mapSizeInBytes;
			controlBlock->EmptyPlaceInCache += temp->mapSizeInBytes;
		}
	}
	if (counter > size) {
		freeLinkedList(temp->linkedList);
		//the not complete using of empty place will be when I put the values in the array, not now.......
	}
}

void MakesRoomForCurrentIncome(int size) {
	if (size > controlBlock->EmptyPlaceInCache)
	{
		overrideInternal(size);
	}
}


//void printLinkedList(Node_t* head) {
//    Node_t* temp = head;
//    while (temp != NULL) {
//        printf("Location: %d, SizeOfBytes: %d -> ", temp->data.location, temp->data.sizeOfBytes); // הדפסת הנתון של הנקודה
//        temp = (Node_t*)temp->next; // מעבר לנקודה הבאה
//    }
//    printf("NULL\n"); // סוף הרשימה
//}
//
//// פונקציה ליצירת נקודה חדשה
//Node_t* createNode(int location, int sizeOfBytes) {
//    Node_t* newNode = (Node_t*)malloc(sizeof(Node_t)); // אלוקציה של זיכרון עבור הנקודה
//    if (newNode == NULL) {
//        printf("Error allocating memory\n");
//        exit(1);
//    }
//    newNode->data.location = location;// הגדרת הנתון של הנקודה
//    newNode->data.sizeOfBytes = sizeOfBytes;
//    newNode->next = NULL; // התחלתה של הרשימה
//    return newNode;
//}
//
////פונקציה להוספת האיבר להיות שני ברשימה
//void insertInTail(Node_t** tail, int location, int sizeOfBytesOfBytes) {
//    if (tail == NULL || *tail == NULL) {
//        return;
//    }
//    Node_t* newNode = createNode(location, sizeOfBytesOfBytes);
//    (*tail)->next = newNode;
//    *tail = newNode;
//}
//
//
//
//
//// פונקציה לשחרור כל הזיכרון שהוקצה ללינקדליסט
//void freeLinkedList(Node_t* head) {
//    Node_t* temp;
//    while (head != NULL) {
//        temp = head;
//        //head = head->next;
//        head = (Node_t*)head->next;
//        free(temp);
//    }
//}
//
////- - - - - - - - - - maxheap - - - - - - - - - - 
//int compare_RangeInDataStorage_t(const void* a, const void* b) {
//    RangeInDataStorage_t* ep1 = (RangeInDataStorage_t*)a;
//    RangeInDataStorage_t* ep2 = (RangeInDataStorage_t*)b;
//    printf("location1 %d\n", ep1->location);
//    printf("sizeOfBytes1 %d\n", ep1->sizeOfBytes);
//
//    printf("location2 %d\n", ep2->location);
//    printf("sizeOfBytes2 %d\n", ep2->sizeOfBytes);
//    printf("comp %d\n", (ep2->location+ep2->sizeOfBytes) - ep1->location);
//    //return (ep2->location - (ep1->location+ep1->sizeOfBytes));
//    //return ((ep2->location+ep2->sizeOfBytes) - ep1->location);
//    return (ep2->location - ep1->location);
//
//}
//
//int compare_RangeInDataStorage_t_and_marge_1(const void* a, const void* b) {
//    RangeInDataStorage_t* ep1 = (RangeInDataStorage_t*)a;
//    RangeInDataStorage_t* ep2 = (RangeInDataStorage_t*)b;
//    printf("location1 %d\n", ep1->location);
//    printf("sizeOfBytes1 %d\n", ep1->sizeOfBytes);
//
//    printf("location2 %d\n", ep2->location);
//    printf("sizeOfBytes2 %d\n", ep2->sizeOfBytes);
//    return (ep2->location - (ep1->location + ep1->sizeOfBytes));
//}
//
//int compare_RangeInDataStorage_t_and_marge_2(const void* a, const void* b) {
//    RangeInDataStorage_t* ep1 = (RangeInDataStorage_t*)a;
//    RangeInDataStorage_t* ep2 = (RangeInDataStorage_t*)b;
//
//    return ((ep2->location + ep2->sizeOfBytes) - ep1->location);
//}
//
////- - - - - - - - - - - avlTreeCompareFunctions - - - - - - - - - - - - 
//
//// השוואה לפי mapID
//int compareMapInfo(const void* a, const void* b) {
//    MapInfo_t* mapInfoA = (MapInfo_t*)a;
//    MapInfo_t* mapInfoB = (MapInfo_t*)b;
//    return mapInfoA->mapID - mapInfoB->mapID;
//}
//
//// השוואה לפי location
//int compareRangeByLocation(const void* a, const void* b) {
//    RangeInDataStorage_t* rangeA = (RangeInDataStorage_t*)a;
//    RangeInDataStorage_t* rangeB = (RangeInDataStorage_t*)b;
//    return rangeA->location - rangeB->location;
//}
//
//// השוואה לפי sizeOfBytes
//int compareRangeBySize(const void* a, const void* b) {
//    RangeInDataStorage_t* rangeA = (RangeInDataStorage_t*)a;
//    RangeInDataStorage_t* rangeB = (RangeInDataStorage_t*)b;
//    return  rangeB->sizeOfBytes - rangeA->sizeOfBytes;
//}
//
//int height(AVLNode_t* node) {
//    return node ? node->height : 0;
//}
//
//void updateHeight(AVLNode_t* node) {
//    int leftHeight = height(node->left);
//    int rightHeight = height(node->right);
//    node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
//}
//
//AVLNode_t* rightRotate(AVLNode_t* y) {
//    AVLNode_t* x = y->left;
//    AVLNode_t* T2 = x->right;
//
//    x->right = y;
//    y->left = T2;
//
//    updateHeight(y);
//    updateHeight(x);
//
//    return x;
//}
//
//AVLNode_t* leftRotate(AVLNode_t* x) {
//    AVLNode_t* y = x->right;
//    AVLNode_t* T2 = y->left;
//
//    y->left = x;
//    x->right = T2;
//
//    updateHeight(x);
//    updateHeight(y);
//
//    return y;
//}
//
//AVLNode_t* insertNode(AVLNode_t* node, void* data, CompareFunc compare) {
//    if (node == NULL) {
//        AVLNode_t* newNode = (AVLNode_t*)malloc(sizeof(AVLNode_t));
//        newNode->data = data;
//        newNode->left = newNode->right = NULL;
//        newNode->height = 1;
//        return newNode;
//    }
//
//    int cmp = compare(data, node->data);
//    if (cmp < 0) {
//        node->left = insertNode(node->left, data, compare);
//    }
//    else if (cmp > 0) {
//        node->right = insertNode(node->right, data, compare);
//    }
//    else {
//        return node;  // Duplicate data is not allowed
//    }
//
//    updateHeight(node);
//
//    int balance = height(node->left) - height(node->right);
//
//    if (balance > 1 && compare(data, node->left->data) < 0) {
//        return rightRotate(node);
//    }
//
//    if (balance < -1 && compare(data, node->right->data) > 0) {
//        return leftRotate(node);
//    }
//
//    if (balance > 1 && compare(data, node->left->data) > 0) {
//        node->left = leftRotate(node->left);
//        return rightRotate(node);
//    }
//
//    if (balance < -1 && compare(data, node->right->data) < 0) {
//        node->right = rightRotate(node->right);
//        return leftRotate(node);
//    }
//
//    return node;
//}
//
//void addNode(AVLTree_t* tree, void* data) 
//{
//    tree->root = insertNode(tree->root, data, tree->compare);
//}
//
//
//// פונקציה להדפסת ערך כל צומת לפי הפונקציה מתאימה
//void printMapInfo(void* data) {
//    MapInfo_t* mapInfo = (MapInfo_t*)data;
//    printLinkedList(mapInfo->linkedList);
//    printf("MapID: %d, MapSizeInBytes: %d\n", mapInfo->mapID, mapInfo->mapSizeInBytes);
//}
//
//void inOrderTraversal(AVLNode_t* node, PrintFunc printFunc) {
//    if (node != NULL) {
//        inOrderTraversal(node->left, printFunc);
//        printFunc(node->data);
//        inOrderTraversal(node->right, printFunc);
//    }
//}
//
//void printAVLTree(AVLTree_t* tree, PrintFunc printFunc) {
//    inOrderTraversal(tree->root, printFunc);
//}
//
//
//
//void printRangeInDataStorage(const void* data) {
//    RangeInDataStorage_t* rangeData = (RangeInDataStorage_t*)data;
//    printf("Location: %d, SizeOfBytes: %d\n", rangeData->location, rangeData->sizeOfBytes);
//}
//
//void printMapInfoTree(AVLTree_t* tree) {
//    inOrderTraversal(tree->root, printMapInfo);
//}
//
//void printRangeInDataStorageTree(AVLTree_t* tree) {
//    inOrderTraversal(tree->root, printRangeInDataStorage);
//}
//// חיפוש הצומת המינימלי בתת-העץ
//AVLNode_t* findMin(AVLNode_t* node) {
//    while (node && node->left != NULL) {
//        node = node->left;
//    }
//    return node;
//}
//
//// חיפוש האיבר העוקב בעץ
//AVLNode_t* findNextNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc) {
//    if (currentNode == NULL) return NULL;
//
//    // מקרה 1: אם יש לצומת הנוכחי צומת ימין
//    if (currentNode->right != NULL) {
//        return findMin(currentNode->right);
//    }
//
//    // מקרה 2: אם אין לצומת הנוכחי צומת ימין
//    AVLNode_t* successor = NULL;
//    AVLNode_t* ancestor = root;
//
//    while (ancestor != NULL) {
//        int cmp = compareFunc(currentNode->data, ancestor->data);
//
//        if (cmp < 0) {
//            successor = ancestor;
//            ancestor = ancestor->left;
//        }
//        else if (cmp > 0) {
//            ancestor = ancestor->right;
//        }
//        else {
//            break;
//        }
//    }
//
//    return successor;
//}
//
//
//
//
//// חיפוש הצומת הגדול ביותר בתת-העץ השמאלי
//AVLNode_t* findMax(AVLNode_t* node) {
//    while (node && node->right != NULL) {
//        node = node->right;
//    }
//    return node;
//}
//
//// חיפוש האיבר הקודם בעץ
//AVLNode_t* findPrevNode(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc) {
//    if (currentNode == NULL) return NULL;
//
//    // מקרה 1: אם לצומת הנוכחי יש צומת שמאלי
//    if (currentNode->left != NULL) {
//        return findMax(currentNode->left);
//    }
//
//    // מקרה 2: אם אין לצומת הנוכחי צומת שמאלי
//    AVLNode_t* predecessor = NULL;
//    AVLNode_t* ancestor = root;
//
//    while (ancestor != NULL) {
//        int cmp = compareFunc(currentNode->data, ancestor->data);
//
//        if (cmp < 0) {
//            ancestor = ancestor->left;
//        }
//        else if (cmp > 0) {
//            predecessor = ancestor;
//            ancestor = ancestor->right;
//        }
//        else {
//            break;
//        }
//    }
//
//    return predecessor;
//}
//
//void mergeNodes(AVLNode_t* root, AVLNode_t* currentNode, CompareFunc compareFunc)
//{
//    AVLNode_t* prev = findPrevNode(root, currentNode, compareFunc);
//    AVLNode_t* next = findNextNode(root, currentNode, compareFunc);
//
//}
//
////
////// פונקציה ליצירת תור חדש
////Queue_t* createQueue() {
////    Queue_t* queue = (Queue_t*)malloc(sizeof(Queue_t));
////    queue->front = NULL;
////    queue->rear = NULL;
////    return queue;
////}
////
////// פונקציה להוספת פריט לתור (enqueue)
////void enqueue(Queue_t* queue, MapInfo_t mapInfo) {
////    QueueNode_t* newNode = (QueueNode_t*)malloc(sizeof(QueueNode_t));
////    newNode->data = mapInfo;
////    newNode->next = NULL;
////
////    if (queue->rear == NULL) {
////        // אם התור ריק
////        queue->front = newNode;
////        queue->rear = newNode;
////    }
////    else {
////        // הוספת פריט בסוף התור
////        queue->rear->next = newNode;
////        queue->rear = newNode;
////    }
////}
////
////// פונקציה להסרת פריט מהתור (dequeue)
////MapInfo_t dequeue(Queue_t* queue) {
////    if (queue->front == NULL) {
////        fprintf(stderr, "Queue is empty\n");
////        exit(EXIT_FAILURE);
////    }
////
////    QueueNode_t* temp = queue->front;
////    MapInfo_t data = temp->data;
////
////    queue->front = queue->front->next;
////    if (queue->front == NULL) {
////        queue->rear = NULL;
////    }
////
////    free(temp);
////    return data;
////}
////
////// פונקציה לבדוק אם התור ריק
////int isQueueEmpty(Queue_t* queue) {
////    return (queue->front == NULL);
////}
////
////// פונקציה לשחרור זיכרון התור
////void freeQueue(Queue_t* queue) {
////    while (!isQueueEmpty(queue)) {
////        dequeue(queue);
////    }
////    free(queue);
////}