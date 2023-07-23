#pragma once

#include <river/plugin_system.hpp>

#include <game/plugin_a/api.hpp>

class rv::SerializedObject;

namespace game::plugin_a {

    class System1 : public rv::PluginSystem {
    public:

        GAME_PLUGIN_A_API System1(
            const rv::PluginSystemParameters& base_parameters,
            int some_id
        );

        GAME_PLUGIN_A_API System1(
            const rv::PluginSystemParameters& base_parameters,
            const rv::SerializedObject* serialized_object
        );

        GAME_PLUGIN_A_API void hello();

        GAME_PLUGIN_A_API int get_hello_count() const;

    protected:

        void serialize(rv::SerializedObject* object_to_serialize_to) const override;

    private:

        // Just there to test constness when deserializing
        const int some_id;

        int hello_count = 0;
        
    };

}