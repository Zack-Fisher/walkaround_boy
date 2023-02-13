#include "Entity.h"
#include <iostream>

int entityIDCounter = 1;

Entity::Entity()
{
    ID = entityIDCounter;

    entityIDCounter++;
}

void Entity::AddComponent(Component *newComponent)
{
    components.push_back(newComponent);

    std::cout << "ENTITY " << ID << ": added component " << newComponent << " of type " << newComponent->ReturnTypeName() << std::endl;
}

//return the which'th component of the specified type.
template <typename T>
Component Entity::GetComponent(int which)
{
    //if not found, return a dummy component?
    Component returnComponent;

    for (int i = 0; i < components.size(); i++)
    {
    }

    return returnComponent;
}

Entity::~Entity()
{
}

int Entity::GetID()
{
    return ID;
}