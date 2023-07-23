#pragma once

#include <string>
#include <vector>
#include <functional>
#include <cassert>

#include <river/serialization.hpp>

namespace rv {

    class Plugin;
    class PluginSystem;
    struct PluginSystemParameters;

    class PluginSystemTypeId {
    public:

        PluginSystemTypeId()
            :   plugin_name(""),
                system_name("")
        { }

        PluginSystemTypeId(const std::string& plugin_name, const std::string system_name)
            :   plugin_name(plugin_name),
                system_name(system_name)
        { 
            assert(!plugin_name.empty());
            assert(!system_name.empty());
        }

        PluginSystemTypeId(const PluginSystemTypeId& other) 
            :   plugin_name(other.plugin_name),
                system_name(other.system_name)
        { }

        PluginSystemTypeId(SerializedObject* serialized_object) 
            :   plugin_name(((SerializedString*)(serialized_object->properties["plugin_name"]))->value),
                system_name(((SerializedString*)(serialized_object->properties["system_name"]))->value)
        { }

        PluginSystemTypeId& operator=(const PluginSystemTypeId& other) {
            this->plugin_name = other.plugin_name;
            this->system_name = other.system_name;
            return *this;
        }

        bool operator==(const PluginSystemTypeId& other) const {
            return 
                    this->plugin_name == other.plugin_name 
                &&  this->system_name == other.system_name;
        }

        [[nodiscard]] const std::string& get_plugin_name() const noexcept {
            return plugin_name;
        }

        [[nodiscard]] const std::string& get_system_name() const noexcept {
            return system_name;
        }


        [[nodiscard]] bool is_empty() const noexcept {
            return plugin_name.empty() || system_name.empty();
        }

        [[nodiscard]] SerializedObject* serialize() const {
            SerializedObject* serialized_object = new SerializedObject();
            serialized_object->properties["plugin_name"] = new SerializedString(this->plugin_name);
            serialized_object->properties["system_name"] = new SerializedString(this->system_name);
            return serialized_object;
        }

    private:
        
        std::string plugin_name;

        std::string system_name;

    };

    class PluginSystemType {
    public:

        using DeserializationConstructor = const std::function<PluginSystem*(
            const PluginSystemParameters& base_parameters,
            const rv::SerializedObject*
        )>;

        PluginSystemType(
            const PluginSystemTypeId& type_id,
            const std::string& class_name,
            Plugin* plugin,
            DeserializationConstructor deserialization_constructor
        ) 
            :   type_id(type_id),
                class_name(class_name),
                plugin(plugin),
                deserialization_constructor(deserialization_constructor)
        { }

    public:

        const PluginSystemTypeId type_id;

        const std::string class_name;
        
        Plugin* const plugin;

        const DeserializationConstructor deserialization_constructor;

    };

}

template<> 
struct std::hash<rv::PluginSystemTypeId> {

    std::size_t operator()(const rv::PluginSystemTypeId& id) const noexcept {
        return 
                std::hash<string>{}(id.get_plugin_name()) 
            ^   (std::hash<string>{}(id.get_system_name()) << 1);
    }

};