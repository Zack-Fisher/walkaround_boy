#pragma once

#include "../Component.h"
#include "../Entity.h"

class Timer : public Component {
    private:
        float initialTime;
        float timeRemaining;
        bool repeat;
    
    public:
        Timer(Entity entity, float startingTime);
        //inheriting the destructor here.

        void process();

        void Tick();
        void End();
};