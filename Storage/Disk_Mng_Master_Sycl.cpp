//#include <CL/sycl.hpp>
//#include <iostream>
//#include <vector>
//#include "Disk_Mng_Master_API.h"
//#include "Disk_Mng_Master.h"
//using namespace cl::sycl;
//extern "C" {
//#include "Disk_Mng_Master_API.h"
//#include "Disk_Mng_Master.h"
//}
//int disk_mng_getMapsIdsInRange(MapRange_t* rangeFromCache, ArrayInfo_t** arrayOfMapsIdsInRange, int arraySize) {
//    int count = 0;
//
//
//    queue q;
//    buffer<int> bufferCount(&count, 1);
//    buffer<ArrayInfo_t> bufferInputArray(disk_mng_CB->arrayForAllMApsInformation, range<1>(arraySize));
//    buffer<ArrayInfo_t> bufferOutputArray(*arrayOfMapsIdsInRange, range<1>(arraySize));
//
//    q.submit([&](handler& h) {
//        auto inputArrayAccess = bufferInputArray.get_access<access::mode::read>(h);
//        auto outputArrayAccess = bufferOutputArray.get_access<access::mode::write>(h);
//        auto countAccess = bufferCount.get_access<access::mode::read_write>(h);
//
//        h.parallel_for(range<1>(arraySize), [=](id<1> i) {
//            if (inputArrayAccess[i].range != NULL &&
//                disk_mng_isTheMapInRange(rangeFromCache, inputArrayAccess[i].range)) {
//                int idx = atomic_fetch_add(countAccess[0], 1);
//                outputArrayAccess[idx] = inputArrayAccess[i];
//                disk_mng_CB->disk_MapsInLoadedToCache[idx] = arrayInLoaded_create(inputArrayAccess[i]->mapid, i);
//            }
//            });
//        }).wait();
//
//        count = bufferCount.get_access<access::mode::read>()[0];
//
//        return count;
//}