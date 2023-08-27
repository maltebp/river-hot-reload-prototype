# Plugin System



**[!] Note: **this some brief notes for when I continue developing this.



- **Construction capture**
  Construction of objects (game objects and systems) are done within a lambda that capture the constructor parameters. This allow us to re-construct the object after hot-reloading with the same parameters.
  - This will be expensive in terms of memory, but I think it's worth it.
  - This allows us to add new members to a class where the member is depending on a parameter that only exists at construction time. 
    - By extension, this allows us to add new components to entities while hotreloading
  - It should be turned off for standalone builds (i.e. when disabling hot-reloading)
  - The captured constructor has to be a function within the owning plugin, which is exposed by mapping the type of the object to this function.
    - We cannot capture the actual constructor, because the constructor will be reloaded
- **Serialization**
  Deserialization of reloaded objects (game objects, plugin systems) is done by constructing the object via the construction capture (see above) and then calling a *deserialize method* on the object. Only *non-constant* variables are serialized. This makes sense because only non-const variables may have changed after construction.
  - This is only with regards to hot reloading - I haven't considered if general serialization should follow same approach.