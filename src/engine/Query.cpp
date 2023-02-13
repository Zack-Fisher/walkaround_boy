#include "Query.h"
#include <stdexcept>
#include <iostream>

#include "Component.h"

template <typename T>
std::vector<T *> Query()
{
    std::vector<T*> queryReturn;

    if (std::is_base_of<Component, T>::value)
    {
        std::cout << "T is not a Component type, returning blank query object." << std::endl;
        return queryReturn;
    }

    for (int i = 0; i < componentPool.size(); i++)
    {
        Component* currComponent = componentPool[i]
        try {
            T* typeCastPointer = (T*)(currComponent);
            queryReturn.push_back(typeCastPointer);
        } catch (const std::bad_cast& e) {
            std::cerr << "Failed to cast pointer: " << e.what() << std::endl;
        }
    }

    return queryReturn;
}
