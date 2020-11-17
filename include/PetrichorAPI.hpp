#pragma once
#ifndef PETRICHOR_API_DEFINITION_HPP
#define PETRICHOR_API_DEFINITION_HPP

#if defined(_MSC_VER)
#define API_C_EXPORT extern "C" __declspec(dllexport)
#define API_C_IMPORT extern "C" __declspec(dllimport)
#define API_EXPORT __declspec(dllexport)
#define API_IMPORT __declspec(dllimport)
#endif

#if defined(__GNUC__)
#define API_C_EXPORT extern "C" __attribute__((visibility("default")))
#define API_EXPORT __attribute__((visibility("default")))
#define API_IMPORT 
#define API_C_IMPORT 
#endif

#ifdef BUILDING_SHARED_LIBRARY
#define PETRICHOR_API API_EXPORT
#elif defined(USING_SHARED_LIBRARY)
#define PETRICHOR_API API_IMPORT
#else
#define PETRICHOR_API 
#endif

#endif //!PETRICHOR_API_DEFINITION_HPP