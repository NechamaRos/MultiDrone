#pragma once

// Define DLL_EXPORT for Windows or as empty for other platforms
#ifdef _WIN32
    #ifdef MYLIBRARY_EXPORTS
        #define DLL_EXPORT __declspec(dllexport)
    #else
        #define DLL_EXPORT __declspec(dllimport)
    #endif
#else
    #define DLL_EXPORT
#endif

extern "C" {
    DLL_EXPORT void multiply_arrays(const int* array1, const int* array2, int* result, int size);
}