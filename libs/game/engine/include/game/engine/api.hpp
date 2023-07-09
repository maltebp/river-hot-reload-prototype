#pragma once

#ifndef GAME_ENGINE_API
    #ifdef GAME_ENGINE_API_IMPLEMENTATION
        #define GAME_ENGINE_API __declspec(dllexport) 
    #else 
        #define GAME_ENGINE_API __declspec(dllimport)
    #endif 
#endif