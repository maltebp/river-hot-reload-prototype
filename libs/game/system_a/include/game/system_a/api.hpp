#pragma once

#ifndef GAME_SYSTEM_A_API
    #ifdef GAME_SYSTEM_A_IMPLEMENTATION
        #define GAME_SYSTEM_A_API __declspec(dllexport) 
    #else 
        #define GAME_SYSTEM_A_API __declspec(dllimport)
    #endif 
#endif