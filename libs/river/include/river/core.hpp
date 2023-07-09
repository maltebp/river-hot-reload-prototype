#pragma once

#ifndef RV_API
    #ifdef RV_API_IMPLEMENTATION
        #define RV_API __declspec(dllexport) 
    #else 
        #define RV_API __declspec(dllimport)
    #endif 
#endif