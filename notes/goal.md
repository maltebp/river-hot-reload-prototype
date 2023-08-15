# Project Goal

### River Hot-Reload Prototype notes



2 key goals:

1. Be able to change logic within code and see changes in-game near instantly
2. Be able to compose entities from within code and see changes in-game near instantly
   - Just like you would when composing entities within an editor, only this should work within code.
3. 



Desired features:

1. Change body of a function
2. Change signature of a function (and, of course, change the calling functions as well - see 2)
3. Change non-injected members and layout of a system (i.e. not changing the signature of the constructor)
4. Change non-injected components and layout of an entity



Non-desired features:

- Change layout and members of a component
  - Considering a component is meant to be an isolated unit, it doesn't make sense to change the layout and members, because most of the parameters should be passed by the constructor.



Key challenges:

- A new component in an entity may rely on an argument that was to the entity itself on construction, meaning the entity has to be reconstructed with the same parameters for them to be passed to the new component.
  - Note: it may be that the parameter is not saved as a member on the entity, meaning it only exists within the construction phase



### Old

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