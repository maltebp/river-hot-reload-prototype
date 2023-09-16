#include <iostream>

#include <river/plugin.hpp>
#include <river/plugin_system.hpp>
#include <river/serialization.hpp>
#include <river/serializers.hpp>
#include <river/hello.hpp>

#include <game/plugin_a/system_1.hpp>
#include <game/plugin_a/game_objects.hpp>

using namespace game::plugin_a;


rv::Plugin* plugin;

rv::GameObject* GameObjectA1_constructor_1(const rv::GameObjectArgs* base_args, const rv::SerializedList* serialized_args) {
    GameObjectA1* game_object = new GameObjectA1(
        *base_args,
        deserialize<int>(serialized_args->elements[0])
    );
    
    return static_cast<GameObjectA1*>(game_object);
}

extern "C" __declspec(dllexport) rv::Plugin* plugin_start(
    rv::PluginManager* manager, 
    rv::Plugin** dependencies, 
    int dependencies_count
) {
    plugin = new rv::Plugin(
        manager,
        "game.plugin_a",
        std::vector<rv::Plugin*>(dependencies, dependencies + dependencies_count)
    );

    plugin->register_system<System1>("System1");

    plugin->register_game_object_type<GameObjectA1>("game::plugin_a::GameObjectA1", &GameObjectA1_constructor_1);

    // TODO: Ensure that passed dependencies match expected

    return plugin;
}

extern "C" __declspec(dllexport) void plugin_stop() {
    delete plugin;
}