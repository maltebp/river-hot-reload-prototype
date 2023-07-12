# Project Goal

### River Hot-Reload Prototype notes



I want to do a prototype of *most* of hot-reloading for most of the game components. It should work like following:



- Game Engine 

  - Agnostic to all entity types
  - References systems via interfaces
  - It handles the hot-reloading

- Hot Reloading Lib

  - Contains classes related to hot reloading (e.g. `IPlugin` and `PluginSystem`)
  - May be part of Core lib

- Plugin

  - The dll that is hot-reloaded - implements the plugin API

- Plugin API 

  - Class representation of the plugin
  - Constructs the

- System:

  - The dll that is hot-reloaded - implements the system interface

  

Flows:

- A system is constructed in a lambda, which can be re-triggered after hot-reloading to re-construct the system
  - [?] How to differentiate between serialization and construction of new system?
    - Maybe bool in the interface constructor
- If an entity is changed (in the header), it triggers all including files to be recompiled. This will trigger hot-reloading of all of those files.
  - [?] This requires changes to entities to trigger serialization/deserialization of entities - how do we do that?
    - We could register some plugin that the entities are part of, which in turn can trigger a reload of the relevant entities in the ECS system
- Systems need some kind of dependency management between them, to ensure they are loaded in the correct order (e.g. when a one system references entities from another system)
  - [?] How to do this?
  - Note: dependency management, is not necessary without entity system
  
- It shouldn't be necessary to have a project per *Entity* system - we should be able to have multiple systems per project (dll)