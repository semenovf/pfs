#pragma once

//
// [Microsoft Support: What is a DLL?](https://support.microsoft.com/en-us/help/815065/what-is-a-dll)
//
#ifdef DLL_API
#    undef DLL_API
#endif
#if defined(_MSC_VER)
#   define DLL_API_EXPORT __declspec(dllexport)
#   define DLL_API_IMPORT __declspec(dllimport)
#   ifdef DLL_EXPORTS
#       define DLL_API DLL_API_EXPORT
#   else
#       define DLL_API DLL_API_IMPORT
#   endif
#else
#   define DLL_API_EXPORT
#   define DLL_API_IMPORT
#   define DLL_API
#endif
