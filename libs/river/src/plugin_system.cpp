#include <river/plugin_system.hpp>

#include <river/plugin.hpp>
#include <river/plugin_manager.hpp>

using namespace rv;


PluginSystem::PluginSystem(Plugin* plugin)
    :   plugin(plugin),
        id(plugin->get_manager()->generate_system_id())
{ }