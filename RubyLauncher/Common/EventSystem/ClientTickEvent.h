#pragma once
#include <string>
#include "Common/EventSystem/RubyEvent.h"
struct ClientTickEvent : public RubyEvent {
    ClientTickEvent(int tickCount)
        : tickCount(tickCount)
    {
        eventName = "ClientTickEvent";
    }

    int tickCount;
};
