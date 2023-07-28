#pragma once

#include <river/plugin_system_ref.hpp>

namespace rv {

    class PluginSystemRefUtility {
    public:

        PluginSystemRefUtility() = delete;

        template<class Base, class Derived>
        static PluginSystemRef<Base> up_cast(const PluginSystemRef<Derived>& derived_ref) {
            static_assert(std::is_base_of<PluginSystem, Base>::value);
            static_assert(std::is_base_of<Base, Derived>::value);  
            return PluginSystemRef<Base>(static_cast<const PluginSystemRefBase&>(derived_ref));
        }

    };

}