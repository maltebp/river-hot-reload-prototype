#pragma once

#include <river/plugin_system.hpp>

#include <game/plugin_b/api.hpp>

namespace game::plugin_b {

    class System1 : public rv::PluginSystem {
    public:

        GAME_PLUGIN_B_API System1(
            const rv::PluginSystemParameters& base_parameters
        );

        GAME_PLUGIN_B_API System1(
            const rv::PluginSystemParameters& base_parameters,
            const rv::SerializedObject* serialized_object
        );

        GAME_PLUGIN_B_API void hello();

        GAME_PLUGIN_B_API int get_hello_count() const;

    protected:

        void serialize(rv::SerializedObject* object_to_serialize_to) const override;

    private:

        int hello_count = 0;

    };

}