#ifndef GAME_PLUGIN_B_API
    #ifdef GAME_PLUGIN_B_API_IMPLMENTATION
        #define GAME_PLUGIN_B_API __declspec(dllexport) 
    #else 
        #define GAME_PLUGIN_B_API __declspec(dllimport)
    #endif 
#endif