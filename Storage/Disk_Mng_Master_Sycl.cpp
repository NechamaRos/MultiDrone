//#include <sycl/sycl.hpp>
//#include <iostream>
//#include <vector>
//#include "Disk_Mng_Master_API.h"
//#include "Disk_Mng_Master.h"
//
//extern "C" {
//#include "Disk_Mng_Master_API.h"
//#include "Disk_Mng_Master.h"
//}
//
//int disk_mng_getMapsIdsInRange(MapRange_t* rangeFromCache, ArrayInfo_t** arrayOfMapsIdsInRange, int arraySize) {
//    int count = 0;
//
//    sycl::queue q;
//    sycl::buffer<int> bufferCount(&count, sycl::range<1>(1));
//    sycl::buffer<ArrayInfo_t*> bufferInputArray(disk_mng_CB->arrayForAllMApsInformation, sycl::range<1>(arraySize));
//    sycl::buffer<ArrayInfo_t> bufferOutputArray(*arrayOfMapsIdsInRange, sycl::range<1>(arraySize));
//
//    q.submit([&](sycl::handler& h) {
//        auto inputArrayAccess = bufferInputArray.get_access<sycl::access::mode::read>(h);
//        auto outputArrayAccess = bufferOutputArray.get_access<sycl::access::mode::write>(h);
//        auto countAccess = bufferCount.get_access<sycl::access::mode::read_write>(h);
//
//        h.parallel_for(sycl::range<1>(arraySize), [=](sycl::id<1> i) {
//            if (inputArrayAccess[i].range != NULL &&
//                disk_mng_isTheMapInRange(rangeFromCache, inputArrayAccess[i].range)) {
//                int idx = sycl::atomic_ref<int, sycl::memory_order::relaxed, sycl::memory_scope::device>(countAccess[0]).fetch_add(1);
//                outputArrayAccess[idx] = inputArrayAccess[i];
//                disk_mng_CB->disk_MapsInLoadedToCache[idx] = arrayInLoaded_create(inputArrayAccess[i].mapid, i);
//            }
//            });
//        }).wait();
//
//        count = bufferCount.get_access<sycl::access::mode::read>()[0];
//
//        return count;
//}
