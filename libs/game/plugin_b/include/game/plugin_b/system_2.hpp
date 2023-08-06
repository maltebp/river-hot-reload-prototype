#pragma once

#include <river/plugin_system.hpp>

#include <game/plugin_b/api.hpp>

namespace game::plugin_b {

    class System2 : public rv::PluginSystem {
    public:

        GAME_PLUGIN_B_API System2(
            const rv::PluginSystemParameters& base_parameters
        );

        GAME_PLUGIN_B_API System2(
            const rv::PluginSystemParameters& base_parameters,
            const rv::SerializedObject* serialized_object
        );

        GAME_PLUGIN_B_API void hello();

        GAME_PLUGIN_B_API int get_hello_count() const;

    protected:

        // TODO: Why is this necessary, when it is not necessary in plugin_a::System1?
        void serialize(rv::SerializedObject* object_to_serialize_to) const override;

    private:

        int hello_count = 0;

    };

}