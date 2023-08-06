#pragma once

#include <cassert>

#include <river/core.hpp>
#include <river/plugin_system.hpp>

namespace rv {

    class PluginManager;
    class PluginSystem;
    class PluginSystemType;
    class PluginSystemRefUtility;

    // TODO: Prevent non-inherited construction of this
    class PluginSystemRefBase {

        friend class PluginManager;
        friend class PluginSystemRefUtility;

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

        // This is only used by PluginManager to construct the first reference
        RV_API PluginSystemRefBase(PluginSystem* system);

        RV_API PluginSystemRefBase(const PluginSystemRefBase& other_ref)
            :   type_id(other_ref.type_id),
                id(other_ref.id),
                manager(other_ref.manager)
        { }

        RV_API [[nodiscard]] PluginSystem* get_system_raw() const;

    protected:

        PluginSystemTypeId type_id;

        PluginSystemId id;

        PluginManager* manager;

    };

    template<class S>
    class PluginSystemRef : public PluginSystemRefBase {

        friend class PluginManager;
        friend class PluginSystemRefUtility;

        using PluginSystemRefBase::PluginSystemRefBase;

    public:

        PluginSystemRef(const PluginSystemRef<S>& other_ref) 
            :   PluginSystemRefBase(other_ref)
        { }

        [[nodiscard]] S* get_system() const {
            return (S*)this->get_system_raw();
        }

        PluginSystemRef<S>& operator=(const PluginSystemRef<S>& other_ref) {
            this->type_id = other_ref.type_id;
            this->id = other_ref.id;
            this->manager = other_ref.manager;
            return *this;
        }

        // TODO: Implement -> operator

    private:

        PluginSystemRef(const PluginSystemRefBase& other_ref)
            :   PluginSystemRefBase(other_ref)
        { }

    };

}