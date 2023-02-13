#pragma once

#include "Entity.h"
#include <string>

class Component {
    private:
        int assocEntityID;
    public:
        Component(Entity entity);
        ~Component();

        //use for comparison and searching, components are stored as pointers in the entity to the Component class, not the subclass.
        virtual std::string ReturnTypeName();

        //run the Component for a frame.
        virtual void process();
        

        bool active;
};

//not related to the stuff, just keep track of components.
void PoolAdd(void* object_ptr);

void processAll();