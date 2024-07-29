#include "Cache_Mng.h"
#include "Cache_Mng_API.h"

Cache_Management_CB_t* cache_mng_CB;

LinkedListNode_t* node_create() {
    LinkedListNode_t* newNode = (LinkedListNode_t*)allocate_memory(sizeof(LinkedListNode_t), "Failed to allocate memory for new linkedListNode", "node_create");
    newNode->imageInfo = NULL;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void node_delete(LinkedListNode_t* node)
{
    if (node!=NULL)
    {
        free(node);
    }
}

void imageInfo_delete(ImageInfo_t* imageInfo)
{
    if (imageInfo!=NULL)
    {
        free(imageInfo);
    }
}
ImageInfo_t* imageInfo_create(int imageId, HashInfo_t* hashPointer, int* cachePointer)
{
    ImageInfo_t* imageInfo = (ImageInfo_t*)allocate_memory(sizeof(ImageInfo_t), "Failed to allocate memory for new imageInfo", "imageInfo_create");
    imageInfo->hashPointer = hashPointer;
    imageInfo->cachePointer = cachePointer;
    imageInfo->imageId=imageId;
    return imageInfo;
}
void linkedList_initialize()
{
    cache_mng_CB->cache_LRU = (CacheLRU_t*)allocate_memory(sizeof(CacheLRU_t), "Failed to allocate memory for new linkedList", "linkedList_initialize");
    cache_mng_CB->cache_LRU->head = NULL;
    cache_mng_CB->cache_LRU->tail = NULL;
    cache_mng_CB->cache_LRU->size = 0;
}
void linkedList_insert(LinkedListNode_t* node)
{
    if (cache_mng_CB->cache_LRU->size<CACHE_SIZE)
    {
        if (cache_mng_CB->cache_LRU->head == NULL) {
            cache_mng_CB->cache_LRU->head = cache_mng_CB->cache_LRU->tail = node;
        }
        else {
            cache_mng_CB->cache_LRU->head->prev = node;
            node->next = cache_mng_CB->cache_LRU->head;
            cache_mng_CB->cache_LRU->head = node;
        }
        cache_mng_CB->cache_LRU->size++;

    }
}
void linkedList_deleteNode(LinkedListNode_t* node)
{
    if (node == NULL) {
        return;
    }

    if (node == cache_mng_CB->cache_LRU->head && node == cache_mng_CB->cache_LRU->tail) {
        cache_mng_CB->cache_LRU->head = cache_mng_CB->cache_LRU->tail = NULL;
    }
    else if (node == cache_mng_CB->cache_LRU->head) {
        cache_mng_CB->cache_LRU->head = cache_mng_CB->cache_LRU->head->next;
        cache_mng_CB->cache_LRU->head->prev = NULL;
    }
    else if (node == cache_mng_CB->cache_LRU->tail) {
        cache_mng_CB->cache_LRU->tail = cache_mng_CB->cache_LRU->tail->prev;
        cache_mng_CB->cache_LRU->tail->next = NULL;
    }
    else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    cache_mng_CB->cache_LRU->size--;
}
void linkedList_moveToHead(LinkedListNode_t* node)
{
    if (node!=NULL)
    {
        if (node == cache_mng_CB->cache_LRU->head) {
            return;
        }
        if (node == cache_mng_CB->cache_LRU->tail) {
            cache_mng_CB->cache_LRU->tail = node->prev;
            cache_mng_CB->cache_LRU->tail->next = NULL;
        }
        else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        node->next = cache_mng_CB->cache_LRU->head;
        cache_mng_CB->cache_LRU->head->prev = node;
        node->prev = NULL;
        cache_mng_CB->cache_LRU->head = node;
    }
}
HashInfo_t* hashInfo_create(int imageId, LinkedListNode_t* linkPointer)
{
    HashInfo_t* hashInfo = (HashInfo_t*)allocate_memory(sizeof(HashInfo_t), "Failed to allocate memory for new hashInfo", "hashInfo_create");
    hashInfo->imageId = imageId;
    hashInfo->linkPointer = linkPointer;
    hashInfo->next = NULL;
    return hashInfo;
}

void hashInfo_delete(HashInfo_t* hashInfo)
{
    free(hashInfo);
}
void queue_initialize()
{
    cache_mng_CB->cache_EmptyAddresses = (CacheEmptyAddresses_t*)allocate_memory(sizeof(CacheEmptyAddresses_t), "Failed to allocate memory for new queue", "queue_initialize");
    cache_mng_CB->cache_EmptyAddresses->front = NULL;
    cache_mng_CB->cache_EmptyAddresses->rear = NULL;
    cache_mng_CB->cache_EmptyAddresses->size = 0;
}
void queue_initilaizeWithEmptyAddresses()
{
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        queue_enqueue(cache_mng_CB->ram[i]);
    }
}
QueueNode_t* queue_createQueueNode(int* address)
{
    QueueNode_t* newQueueNode = (QueueNode_t*)allocate_memory(sizeof(QueueNode_t), "Failed to allocate memory for new queueNode", "queue_createQueueNode");
    newQueueNode->address = address;
    newQueueNode->next = NULL;
    return newQueueNode;
}
bool queue_isEmpty()
{
    return (cache_mng_CB->cache_EmptyAddresses->size == 0);
}
void queue_enqueue(int* address)
{
    QueueNode_t* newNode = queue_createQueueNode(address);
    if (cache_mng_CB->cache_EmptyAddresses->size<CACHE_SIZE)
    {
        if (cache_mng_CB->cache_EmptyAddresses->rear == NULL) {
            cache_mng_CB->cache_EmptyAddresses->front = newNode;
            cache_mng_CB->cache_EmptyAddresses->rear = newNode;
        }
        else {
            cache_mng_CB->cache_EmptyAddresses->rear->next = newNode;
            cache_mng_CB->cache_EmptyAddresses->rear = newNode;
        }
        cache_mng_CB->cache_EmptyAddresses->size++;
    }
}
int* queue_dequeue()
{
    if (queue_isEmpty()) {
        return NULL;
    }
    QueueNode_t* temp = cache_mng_CB->cache_EmptyAddresses->front;
    int* address = temp->address;
    cache_mng_CB->cache_EmptyAddresses->front = cache_mng_CB->cache_EmptyAddresses->front->next;
    if (cache_mng_CB->cache_EmptyAddresses->front == NULL) {
        cache_mng_CB->cache_EmptyAddresses->rear = NULL;
    }
    cache_mng_CB->cache_EmptyAddresses->size--;
    return address;
}
int* queue_top()
{
    return cache_mng_CB->cache_EmptyAddresses->front;
}


void hashTable_initialize()
{
    cache_mng_CB->cache_ByImageId =(CacheByImageId_t*) allocate_memory(sizeof(CacheByImageId_t), "Failed to allocate memory for new hash table", "hashTable_initialize");
    cache_mng_CB->cache_ByImageId->length = 0;
    cache_mng_CB->cache_ByImageId->size = CACHE_SIZE;
    //?
    cache_mng_CB->cache_ByImageId->entries = calloc(cache_mng_CB->cache_ByImageId->size, sizeof(HashInfo_t*));
    if (cache_mng_CB->cache_ByImageId->entries == NULL)
    {
        test_writeExceptionToFile(Error_When_Allocating_Memory_Space, "hashTable_initialize-entries");
        free(cache_mng_CB->cache_ByImageId);
        perror("Failed to allocate memory for entries");
        return NULL;
    }
}

void hashTable_delete(HashInfo_t* hashInfo) {
    if (hashInfo!=NULL)
    {
        int index = hashTable_function(hashInfo->imageId);
        HashInfo_t* hashInfo1 = cache_mng_CB->cache_ByImageId->entries[index];
        HashInfo_t* prevHashInfo = NULL;
        while (hashInfo1 != NULL)
        {
            if (hashInfo1->imageId == hashInfo->imageId)
            {
                if (prevHashInfo == NULL)
                {
                    cache_mng_CB->cache_ByImageId->entries[index] = hashInfo1->next;
                }
                else
                {
                    prevHashInfo->next = hashInfo1->next;
                }
                cache_mng_CB->cache_ByImageId->length--;
                return;
            }
            prevHashInfo = hashInfo1;
            hashInfo1 = hashInfo1->next;
        }
    }
}
int hashTable_function(int imageId) {
    return (int)imageId % CACHE_SIZE;
}
void hashTable_insert(HashInfo_t* hashInfo)
{
    if (cache_mng_CB->cache_ByImageId->length<CACHE_SIZE)
    {
        int index = hashTable_function(hashInfo->imageId);
        hashInfo->next = cache_mng_CB->cache_ByImageId->entries[index];
        cache_mng_CB->cache_ByImageId->entries[index] = hashInfo;
        cache_mng_CB->cache_ByImageId->length++;
    }
}
HashInfo_t* hashTable_search(int imageId)
{
    int index = hashTable_function(imageId);
    HashInfo_t* hashInfo = cache_mng_CB->cache_ByImageId->entries[index];
    while (hashInfo!=NULL)
    {
        if (hashInfo->imageId==imageId)
        {
            return hashInfo;
        }
       hashInfo= hashInfo->next;
    }
    return NULL;
}
int** cache_getImagesPointersInRangeFromMaster(Point_t topLeft, Point_t bottomRight)
{
    if (prefetch_IsTheImageInPrefetchLoading(topLeft,bottomRight))
    {
       cache_mng_CB->sourceForAPICall = PREFETCH_AND_MASTER_CALL;//set the status of source call
    }
    else
    {
        cache_mng_CB->sourceForAPICall = MASTER_CALL;//set the status of source call
        return cache_getRange_internal(topLeft, bottomRight);//load the images in range to the cache and send pointers to the master
    }
    return NULL;
}
void cache_LoadingImagesInRangeToCache(Point_t topLeft, Point_t bottomRight)
{
    cache_mng_CB->sourceForAPICall = PREFETCH_CALL;//set the status of source call
    (void)cache_getRange_internal(topLeft, bottomRight);//load the images in range to the cache 
    prefetch_removeRangeFromRangesInLoadingArray(topLeft, bottomRight);//remove the range from prefetch array after loading to cache
}
void cache_deleteImageByImageId(int imageId)
{
    HashInfo_t* hashInfo = hashTable_search(imageId);//search the hashInfo with this image id
    if (hashInfo!=NULL)//there is object with this image id
    {
        LinkedListNode_t* linkedPointer = hashInfo->linkPointer;
        cache_deleteImageFromCacheMapping(linkedPointer);//delete this object from the cache mapping
    }
}
void ram_initialize()
{
    cache_mng_CB->ram = (int**)allocate_memory(CACHE_SIZE * sizeof(int*), "Failed to allocate memory for cache", "ram_initialize");
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache_mng_CB->ram[i] = (int*)allocate_memory(sizeof(int), "Failed to allocate memory for image in cache", "ram_initialize-pointer");
    }
}

void cache_addImagePointerToTheArray(int** imagePointersInCache, int indexInArray, HashInfo_t* hashInfo)
{
    LinkedListNode_t* linkedListNode= hashInfo->linkPointer;
    imagePointersInCache[indexInArray] = linkedListNode->imageInfo->cachePointer;//fill in the buffer with the cache pointer of this image
}
bool isContinuInLoop(int** imagePointersInCache, int indexInArray, int counter)
{
    return counter > 0 && indexInArray < CACHE_SIZE && imagePointersInCache[indexInArray] != 0;
}

int ** cache_getRange_internal(Point_t topLeft, Point_t bottomRight)
{
    int arrayOfImagesId[CACHE_SIZE]={0};//buffer to images id in range
    int indexInArray = 0;//index to go over the array
    HashInfo_t* hashInfo;//pointer to the hash object with the found image id
    bool isImageLoadedFromDisk, isImageMappingInCache;//is the load from disk to cache success?
   
    int countOfImagesInRange=disk_Mng_getImagesIdInRangedByTwoPoints(topLeft, bottomRight, arrayOfImagesId,CACHE_SIZE);//send API to disk -disk fill in the buffer all the images id in range
    int** imagePointersInCache = (int**)allocate_memory(CACHE_SIZE * sizeof(int*), "Failed to allocate memory for image pointers array", "cache_getRange_internal");//an array for saving pointers to images in the cache
    while (isContinuInLoop(imagePointersInCache,indexInArray,countOfImagesInRange))//go over the filled cells in the array
    {
        hashInfo = hashTable_search(arrayOfImagesId[indexInArray]);//find object with this id in the hash table
        isImageMappingInCache = (hashInfo == NULL);
        if (isImageMappingInCache)//check if the image mapping in the cache
        {
            if (queue_isEmpty())//Is the cache full?
            {
                cache_deleteImageByLRU();//delete objects from the cache by LRU
            }
            isImageLoadedFromDisk = disk_Mng_loadImageFromDiskToCache(arrayOfImagesId[indexInArray],queue_top());//load image from disk to cache
            if (isImageLoadedFromDisk)//was the loading image from disk to cache successful?
            {
               hashInfo=cache_addImageToTheCacheMapping(arrayOfImagesId[indexInArray]);//add image to the cache mapping
            }
            else
            {
                test_writeExceptionToFile(Failed_To_Load_Image_From_Disk_To_Cache, "disk_Mng_loadImageFromDiskToCache");//the load fail - write exception to errors file
                countOfImagesInRange--;
                continue;
            }
        }
        else
        {
            linkedList_moveToHead(hashInfo->linkPointer);//when we access a member in the list, it moves to the top priority (LRU)
        }
        cache_addImagePointerToTheArray(imagePointersInCache, indexInArray, hashInfo);
        indexInArray++;
        countOfImagesInRange--;
    }
    return cache_TreatmentOfReturningAnswers(topLeft,bottomRight,imagePointersInCache);
}
int** cache_TreatmentOfReturningAnswers(Point_t topLeft, Point_t bottomRight,int ** imagePointersInCache)
{
    if (cache_mng_CB->sourceForAPICall == PREFETCH_AND_MASTER_CALL)
    {
        //Since there is currently no real parallel,
        //There is no way to return to the master the images he requested when the prefetch is the one that actually activated the function
    }
    if (cache_mng_CB->sourceForAPICall == MASTER_CALL)
    {
        prefetch_InsertNewRangeToPrefetch(topLeft,bottomRight);//in case that start a new range - insert to prefetch loading
    }
    return imagePointersInCache;
}
void cache_deleteImageFromCacheMapping(LinkedListNode_t* node)
{
    ImageInfo_t* imageInfo = node->imageInfo;//saving a direct pointer to the object in node
    queue_enqueue(imageInfo->cachePointer);//insert the Ram address to the empty addresses queue
    hashTable_delete(imageInfo->hashPointer);//delete the hashInfo from the hash table
    hashInfo_delete(imageInfo->hashPointer);//delete the hashInfo
    linkedList_deleteNode(node);//delete the node from the linked list
    imageInfo_delete(imageInfo);//delete the imageInfo from the node
    node_delete(node);//delete the node
}
void cache_deleteImageByLRU()
{
    LinkedListNode_t* lruPointer;
    for (int i = 0; i < CACHE_SIZE / 10; i++)//loop on 10% from the tail
    {
        lruPointer = cache_mng_CB->cache_LRU->tail;
        if (lruPointer!=NULL)
        {
            cache_deleteImageFromCacheMapping(lruPointer);
        }
    }
}
HashInfo_t* cache_addImageToTheCacheMapping(int imageId)
{
    LinkedListNode_t* newLinkedListNode = node_create();//create new node for the linked list
    HashInfo_t* newHashInfo = hashInfo_create(imageId, newLinkedListNode);//create new hashInfo for the hash table
    hashTable_insert(newHashInfo);//insert the new hashInfo to the hash table
    int* address = queue_dequeue();//pop empty address from the queue
    newLinkedListNode->imageInfo = imageInfo_create(imageId, newHashInfo, address);//create imageInfo for the linked list node
    linkedList_insert(newLinkedListNode);//insert the new node to the linked list
    return  newHashInfo;
}

void cache_initialize_CB()
{
    cache_mng_CB = (Cache_Management_CB_t*)allocate_memory(sizeof(Cache_Management_CB_t), "Failed to allocate memory for cache control block", Error_When_Allocating_Memory_Space, "cache_initialize_CB");
}

void cache_initialize()
{
    cache_initialize_CB();
    ram_initialize();
    hashTable_initialize();
    linkedList_initialize();
    queue_initialize();
    queue_initilaizeWithEmptyAddresses();
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
    case Failed_To_Load_Image_From_Disk_To_Cache:
        error_message = "Error: Failed to load image from disk to cache.";
        break;
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


void* allocate_memory(size_t size, const char* description, const char* functionName) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        test_writeExceptionToFile(Error_When_Allocating_Memory_Space, functionName);
        perror(description);
        return NULL;
    }
    return ptr;
}


void printHashTable(CacheByImageId_t* hashTable) {
    if (hashTable == NULL) {
        printf("Hash Table is NULL.\n");
        return;
    }

    printf("Hash Table (Size: %zu, Length: %zu):\n", hashTable->size, hashTable->length);
    for (int i = 0; i < hashTable->size; i++) {
        printf("Bucket %zu:\n", i);
        HashInfo_t* current = hashTable->entries[i];
        while (current != NULL) {
            printf("  Image ID: %d\n", current->imageId);
            printf("  Link Pointer: %p\n", (void*)current->linkPointer);
            current = current->next;
        }
    }
}

