#include <sycl/sycl.hpp>
#include <dpct/dpct.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
extern "C" {
#include "MasterCacheImages.h"
}
using namespace sycl;
ImgInfo_t* searchByImgIdAndSlaveId(int imgId, int slaveId, ImgInfo_t* imgArray)
{
    ImgInfo_t* find = nullptr;

    queue q;  // SYCL queue to manage tasks

    // Allocate device memory for imgArray and result
    ImgInfo_t* d_imgArray = malloc_device<ImgInfo_t>(CACHE_SIZE, q);
    if (!d_imgArray) {
        std::cerr << "Memory allocation for d_imgArray failed." << std::endl;
        return nullptr;
    }

    // Copy imgArray to device memory
    q.memcpy(d_imgArray, imgArray, CACHE_SIZE * sizeof(ImgInfo_t)).wait();

    ImgInfo_t* d_find = malloc_device<ImgInfo_t>(1, q); // Allocate device memory for result
    if (!d_find) {
        std::cerr << "Memory allocation for d_find failed." << std::endl;
        sycl::free(d_imgArray, q); // Free allocated memory on failure
        return nullptr;
    }

    // Initialize d_find with a default value (empty ImgInfo_t)
    ImgInfo_t init_find = {};
    q.memcpy(d_find, &init_find, sizeof(ImgInfo_t)).wait();

    // Submit SYCL kernel to perform the search
    q.submit([&](handler& h) {
        h.parallel_for(CACHE_SIZE, [=](id<1> i) {
            // Check if the current entry matches the criteria
            if (d_imgArray[i].cachePtr != nullptr && 
                d_imgArray[i].imgId == imgId && 
                d_imgArray[i].slaveId == slaveId) 
            {
                d_find[0] = d_imgArray[i]; // Store the found result
            }
        });
    }).wait();

    // Allocate host memory to store the result
    find = (ImgInfo_t*)malloc(sizeof(ImgInfo_t));
    if (!find) {
        std::cerr << "Memory allocation for host find failed." << std::endl;
        sycl::free(d_imgArray, q);  // Free device memory
        sycl::free(d_find, q);
        return nullptr;
    }

    // Copy the result from device memory to host memory
    q.memcpy(find, d_find, sizeof(ImgInfo_t)).wait();

    // Check if no match was found (cachePtr is nullptr)
    if (find->cachePtr == nullptr) {
        free(find);
        find = nullptr;  // Return nullptr if no result was found
    }

    // Free device memory
    sycl::free(d_imgArray, q);
    sycl::free(d_find, q);

    return find; // Return the found result or nullptr
}

ImgInfo_t** findImagesInRange(const ImgInfo_t* imgArray, Point_t TL, Point_t BR, int* result_size) {
    // Create SYCL queue for managing tasks
    queue q{default_selector{}};

    // Allocate shared memory for results (ImgInfo_t pointers)
    ImgInfo_t** found = (ImgInfo_t**)malloc_shared(CACHE_SIZE * sizeof(ImgInfo_t*), q);
    for (int i = 0; i < CACHE_SIZE; i++) {
        found[i] = (ImgInfo_t*)malloc_shared(sizeof(ImgInfo_t), q);
    }

    if (found == NULL) {
        *result_size = 0; // No results if memory allocation failed
        return NULL;
    }
    *result_size = 0; // Initialize the result count to 0

    // Create a buffer for imgArray to be read on the device
    buffer<const ImgInfo_t, 1> imgArray_buf(imgArray, CACHE_SIZE);

    // Create a buffer for result_size with atomic access
    buffer<int, 1> result_size_buf(result_size, 1);

    // Submit SYCL kernel to search for images within the specified range
    q.submit([&](handler& h) {
        // Access the imgArray buffer
        auto imgArray_acc = imgArray_buf.get_access<access::mode::read>(h);
        auto result_size_acc = result_size_buf.get_access<access::mode::read_write>(h);

        // Kernel to check each image's coordinates and find matches
        h.parallel_for(range<1>(CACHE_SIZE), [=](id<1> i) {
            if (imgArray_acc[i].cachePtr != NULL) {
                // Check if image is within the defined range (TL, BR)
                if (imgArray_acc[i].TL.x >= TL.x && imgArray_acc[i].TL.y >= TL.y &&
                    imgArray_acc[i].BR.x <= BR.x && imgArray_acc[i].BR.y <= BR.y) {
                    
                    // Use atomic_ref to ensure atomic updates to result_size
                    atomic_ref<int, memory_order::relaxed, memory_scope::device> atomic_result_size(result_size_acc[0]);
                    int index = atomic_result_size.fetch_add(1);

                    // Copy the found image to the shared memory
                    *found[index] = imgArray_acc[i];
                }
            }
        });
    }).wait();

    return found; // Return the found images (array of pointers)
}


int main() {
    std::cout << "hallo word" << std::endl;
    return 0;
}
