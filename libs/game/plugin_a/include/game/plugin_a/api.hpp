#pragma once

#ifndef GAME_PLUGIN_A_API
    #ifdef GAME_PLUGIN_A_API_IMPLMENTATION
        #define GAME_PLUGIN_A_API __declspec(dllexport) 
    #else 
        #define GAME_PLUGIN_A_API __declspec(dllimport)
    #endif 
#endif