#pragma once

#include "Component.h"
#include <vector>

class Entity
{
    private:
        int ID;
        std::vector<Component*> components;
        
    public:
        Entity();
        ~Entity();

        void AddComponent(Component* newComponent);
        template <typename T>
        Component GetComponent(int which);

        int GetID();
};

