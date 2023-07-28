#pragma once

#include <cassert>

#include <river/core.hpp>
#include <river/plugin_system.hpp>

namespace rv {

    class PluginManager;
    class PluginSystem;
    class PluginSystemType;

    // TODO: Prevent construction of this
    class PluginSystemRefBase {
    public: 

        PluginSystemRefBase() 
            :   type_id(),
                manager(nullptr),
                id(0)
        { }

        RV_API PluginSystemRefBase(
            PluginManager* manager, 
            SerializedObject* serialized_system_ref
        );

        [[nodiscard]] PluginManager* get_manager() const {
            return manager;
        }

        [[nodiscard]] PluginSystemId get_id() const {
            return id;
        }

        [[nodiscard]] bool is_empty() const {
            return id == 0 || type_id.is_empty() || manager == nullptr;
        }

        RV_API [[nodiscard]] SerializedObject* serialize() const;

    protected:

        RV_API PluginSystemRefBase(PluginSystem* system);

        PluginSystemRefBase(const PluginSystemRefBase& other_ref)
            :   type_id(other_ref.type_id),
                id(other_ref.id),
                manager(other_ref.manager)
        { }

        RV_API [[nodiscard]] PluginSystem* get_system_raw() const;

        PluginSystemTypeId type_id;

        PluginSystemId id;

        PluginManager* manager;

    };

    template<class S>
    class PluginSystemRef : public PluginSystemRefBase {

        static_assert(std::is_base_of<PluginSystem, S>::value);

        friend class PluginManager;

        using PluginSystemRefBase::PluginSystemRefBase;

    public:

        PluginSystemRef(S* system) 
            : PluginSystemRefBase(system) 
        { }

        PluginSystemRef(const PluginSystemRef<S>& other_ref) 
            :   PluginSystemRefBase(other_ref)
        { }

        template<class T>
        PluginSystemRef(const PluginSystemRef<T>& other_ref)
            :   PluginSystemRefBase(other_ref)
        { 
            this->static_assert_cast_validity<T>();
        }

        template<class T>
        PluginSystemRef<T> cast() {
            // Note: it seems this may not be sufficient in all cases
            this->static_assert_cast_validity<T>();
            // TODO: Implement
        }

        [[nodiscard]] S* get_system() const {
            // TODO: Try and see if static cast works here
            return (S*)this->get_system_raw();
        }

        PluginSystemRef<S>& operator=(const PluginSystemRef<S>& other_ref) {
            this->type_id = other_ref.type_id;
            this->id = other_ref.id;
            this->manager = other_ref.manager;
            return *this;
        }

        template<class T>
        PluginSystemRef<S>& operator=(const PluginSystemRef<T>& other_ref) {
            this->static_assert_cast_validity<T>();
            this->type_id = other_ref.type_id;
            this->id = other_ref.id;
            this->manager = other_ref.manager;
            return *this;
        }

        // Do "cast" assignment operator

        // TODO: Implement -> operator

    private:
        
        template<class T>
        void static_assert_cast_validity() {
            static_assert(std::is_base_of<PluginSystem,T>::value);
            static_assert(std::is_base_of<S,T>::value || std::is_base_of<T,S>::value);            
        }

    };

}