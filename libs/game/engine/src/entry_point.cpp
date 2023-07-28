#include <game/engine/entry_point.hpp>

#include <game/plugin_a/plugin.hpp>
#include <game/plugin_a/system_1.hpp>
#include <game/plugin_b/plugin.hpp>

using namespace game::engine;

void EntryPoint::serialize(rv::SerializedObject* object_to_serialize_to) const {
    object_to_serialize_to->properties["initialized"] = new rv::SerializedBool(this->initialized);
    object_to_serialize_to->properties["system_1"] = system_1.serialize();
}

rv::EntryPointSystem::UpdateReturnCode EntryPoint::update() {
    if( !this->initialized ) {
        this->initialized = true;   

        this->system_1 = this->manager->create_system<plugin_a::System1>(42);
    }
    
    std::cout << std::endl << "> ";

    std::string input;
    std::getline(std::cin, input);

    if( input == "hello" ) {
        this->manager->get_plugin<game::plugin_a::Plugin>()->hello();
        this->manager->get_plugin<game::plugin_b::Plugin>()->hello();
        system_1.get_system()->hello();
        return rv::EntryPointSystem::UpdateReturnCode::Continue;
    }
    
    if( input == "reload" ) {
        return rv::EntryPointSystem::UpdateReturnCode::Reload;
    }
    
    if( input == "quit" ) {
        std::cout << "Quitting.." << std::endl;
        return rv::EntryPointSystem::UpdateReturnCode::Shutdown;
    }

    std::cout << "Error: unknown command '" << input << "'" << std::endl;
    return rv::EntryPointSystem::UpdateReturnCode::Continue;
}