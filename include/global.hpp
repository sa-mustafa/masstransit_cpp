#pragma once

#define MASSTRANSIT_VERSION_MAJOR 1
#define MASSTRANSIT_VERSION_MINOR 3
#define MASSTRANSIT_VERSION_PATCH 0

#if defined _WIN32 || defined __CYGWIN__
#define MASSTRANSIT_HELPER_DLL_IMPORT __declspec(dllimport)
#define MASSTRANSIT_HELPER_DLL_EXPORT __declspec(dllexport)
#define MASSTRANSIT_HELPER_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define MASSTRANSIT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#define MASSTRANSIT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#define MASSTRANSIT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define MASSTRANSIT_HELPER_DLL_IMPORT
#define MASSTRANSIT_HELPER_DLL_EXPORT
#define MASSTRANSIT_HELPER_DLL_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define MASSTRANSIT_API and MASSTRANSIT_LOCAL.
// MASSTRANSIT_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// MASSTRANSIT_LOCAL is used for non-api symbols.

#ifdef MASSTRANSIT_DLL // defined if MASSTRANSIT is compiled as a DLL
#ifdef MASSTRANSIT_DLL_EXPORTS // defined if we are building the MASSTRANSIT DLL (instead of using it)
#define MASSTRANSIT_API MASSTRANSIT_HELPER_DLL_EXPORT
#else
#define MASSTRANSIT_API MASSTRANSIT_HELPER_DLL_IMPORT
#endif // MASSTRANSIT_DLL_EXPORTS
#define MASSTRANSIT_LOCAL MASSTRANSIT_HELPER_DLL_LOCAL
#else // MASSTRANSIT_DLL is not defined: this means MASSTRANSIT is a static lib.
#define MASSTRANSIT_API
#define MASSTRANSIT_LOCAL
#endif // MASSTRANSIT_DLL

