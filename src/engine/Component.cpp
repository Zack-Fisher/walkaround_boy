#include <iostream>
#include "Component.h"

#include "Query.h"
#include "Entity.h"

std::vector<Component*> componentPool;

void PoolAdd(Component* newComponentPtr)
{
    componentPool.push_back(newComponentPtr);

    std::cout << "COMPONENT ADDED TO POOL: " << newComponentPtr << std::endl;
}

Component::Component(Entity entity)
{
    entity.AddComponent((Component*)this);
    assocEntityID = entity.GetID();
    //every component is added to the component pool
    PoolAdd((void*)this);
}

Component::~Component()
{
}

void processAll()
{
    //process the next frame for each loaded component.
    for (int i = 0; i < componentPool.size(); i++)
    {
        Component* currComponent = componentPool[i];

        currComponent->process();
    }
}
