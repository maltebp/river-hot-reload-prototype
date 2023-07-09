# Miscellaneous

### River Hot-Reload Prototype notes





## Initial brainstorming

- Hot-reloading helps reducing iteration time by reducing:
  - Compile-time
  - Start time (starting program)
  - Setup time (e.g. travelling with character to some particular place)

- Things that can be hot-reloaded
  - Data
    - Static data
    - Construction data (passed as constructor argument)
    - Regular data (e.g. position)
  - Code
    - Content of functions and methods
  - Types
    - Struct/class structure and layouts
    - Function signatures

- Is the most crucial aspect of hot-reloading to reload **data and code**?
- **You cannot hot-reload a type without an abstraction of the type**



- Core questions:
  - Should the entire game code be reloaded, or should we split it up into separate systems that can be reloaded?
    - We may want to make it possible to have some separate systems that can be hot-reloaded, but not everything (e.g. have UI in one .dll, Game Code in another, renderer in one etc.)
    - Types create reload dependencies
      - If .dll D1 defines type T1 and .dll D2 uses type T1, then if D1 is reloaded, D2 must also be reloaded
    - If we reload *everything* then we can have *type reloading*



- Hot-reloadable ECS entity
  - What do we want to hot-reload?
    - Static data: Defined by systems (and thus hot-reloadable via systems)
    - Construction data: Defined by system constructing entity
      - Is this really necessary though?
      - If a system has created an entity with some parameters, those should not have to change I recon.
    - Regular data: 
      - If entity is exposed in editor, then that should be simple enough
  - Interface for every type?
    - Could we do this, just for development build?
    - This would become super tedious
  - Any code that need to reference the type, must include the header containing the type. Hence, that piece of code must be re-compiled.
    - If we just use the regular build system to re-compile, then we should have the hot-reloading be able to figure which files have changed.
    - **[?] Does this require some kind of dependency setup between the plugins?** 
  - If a type changes, then the entity, and all entities using that entity, have to be reconstructed.
    - Can we deploy some kind of mechanism to signal to the parent entity that reconstruction is necessary?
      - We can use the control block of the entity.
- Hot-reloadable ECS system

  - How to reload the system object?
    - Re-construct it
      - Can be done by exposing some function that constructs the system which can be called indirectly
      - Has to have a *handle*
    - Use serialization
      - If the game has to be stored, then we likely want to serialize the state of the system anyway
  - What about system dependencies?
    - Dependencies
      - A system shouldn't rely directly on other systems
      - May depend on *output* of other systems
        - That output would have to passed as arguments to the system
      - May depend on non-system things (e.g. keyboard)
    - What if we reload the entirety of the provider code base?
    - We could just store the pointers to things that cannot be hot-reloaded (e.g. keyboard)
  - What if the interface of the system exists in separate .dll?
    - 3 layers:
      - Engine: Dynamically loads and calls Game Core Interface
      - Game Core: Implements Game Core Interface and dynamically loads game system and calls system interface
      - System: implements and provides game system interface
    - System is a interface is specific to the given system
    - The actual system type only exists in the *System*
    - If *System Interface* needs to change then *Game Core* should be recompiled
      - Reasons for change: *System* needs changes in parameters
    - *Game Core* can be hot-loaded by engine, because core engine should be agnostic to *Game Core*
      - Reasons for change: a new system is added, a system interface is changed etc.
    - Hot-reloadable system construction is wrapped in some kind of lambda, so the system can be reloaded dynamically

  - System has to exist in separate .dll from the type





- How to do serialization?
  - We probably need some kind of reflection library
- What to do if a system fails? Crash entire editor?
- Can we avoid the hot-reloading overhead (so we can provide link-optimizations) when doing release build?



### Scenarios

1. **Character controller**
   I'm creating the controller logic for the main player character. This involves tweaking the speed of the character, what should happen when certain buttons are pressed, and how the camera should follow the player. 
   - *Desired work flow*:
     - The player character entity is some entity that exists outside the controller system.
     - The player character model is a template which the entity is using
     - The movement code is entirely contained, including some static parameters, within a system.
2. **AOE spell**
   I'm creating a spell that shoots bullets in a circle around the player. I'm unsure of how many bullets there should be, how fast they should fly and how much damage they should deal, so I'm tweaking these. I also decide that I want to add some particles to each of the bullets, so I create a particle emitter with a visual tool that I then attach to them.
   - *Desired work flow*:
     - The bullet entity (movement component, damage, model template reference etc. is defined  in non-reloadable code
     - A template is created for the bullet model along with the system (see River templates)
     - The bullet model is created as the template in the editor.  The particle emitter is created via some particle emitter tool, and is added to the entity.
     - The triggering of the spell and the arguments of the spell (speed, damage etc.) is defined in the a hot-reloadable system. This system is also the one that constructs the bullet entity

3. **Particle emitter tool**
   I'm creating the user interface for a creating particle emitter tool. I have a *model* that the tool operates on, which is the particle emitter entity.
   - *Desired work flow*:
     - The view is drawn in a function that takes the model that it operates on. This function exists in a system that can be hot-reloaded.
4. **Creating new enemy entity**
   I'm creating a new enemy entity type. I want to create 1) the core entity, 2) a template for the visual and 3) some system that handles its behavior. I would like to add all of these, without having to close the editor first.
   - *Desired work flow*:
     - No code, except for the core entity, needs to know about the template. The should get this exposed indirectly.
     - The system needs to know about the 