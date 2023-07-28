# Log

### River Hot-Reloading Prototype



- ... A lot of undocumented stuff

- I don't want to use nested classes, because they cannot be forward declared

- `PluginSystemType` is identified by its plugin name and system name (i.e. the *PluginSystemTypeId*)

  - Pointers to `PluginSystemType` cannot be stored, because the object is constructed at plugin restart, which means it will be re-constructed every time plugin is reloaded
  - In the future, the ID could constructed from a more stable name (i.e. could be retrieved via macro when registering the system)
  - We still have to expose a mapping between the `typeid(S).name()` of the system `S` and the `PluginSystemType`. But this string should *not be stored*, because it might change at every compilation.

- **Serialization**

  - The concrete plugin system has to provide the constructor that deserializes, but it should also be auto-generatable (for the future)

    - You can do this, by having the user add a macro to the system which defines the constructor and then provide implementation of constructor via a separate `.cpp` file (this way you don't touch the user's code)

  - Not using templates to deserialize cause some annoying access patterns:
    ```c++
    : hello_count(((rv::SerializedInt*)serialized_object- >properties.at("hello_count"))->value)
    
    ```

  - **Initialization order of systems**

    - Order of constructions *could* matter, because one system may depend on some value from another system. **But** deserialization is automated (and code should be autogenerated), and the only thing it does is that it sets the direct values (i.e. no dereferencing of maybe-uninitialized-system).


- **Main Plugin Serialization**
  - *The Problem:*
    The `MainPlugin` needs some kind of serialization, because it creates the first system(s), and these systems are the roots to the dependency tree of systems. This means the plugin must maintain some kind of reference to the system(s), which has to be  maintained on reload.
  - **Thoughts**
    - We can avoid implementing and handling serialization of the plugin, by having the Plugin provide a *main* system - one that is constructed automatically by the plugin manager, and which can only be constructed once.
      - The `MainPlugin` can just provide a method that constructs this system
      - Then again, the main system differs from other systems, in that it should only be constructed once, and not be exposed as a regular system.
    - Should we maybe consider a generalization of the hotreload serialization setup?
    - **Current opinion:** We don't need generalized hotreloading types, and I think we should just expose some kind of serializable main system - preferably something that just uses existing system setups
      - I started sketching this out
    - 