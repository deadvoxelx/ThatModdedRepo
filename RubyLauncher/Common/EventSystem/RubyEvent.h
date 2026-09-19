#pragma once

#include <string>

struct RubyEvent {
    std::string eventName;
};

struct CancellableRubyEvent : public RubyEvent {
    bool cancelled = false;

    void setCancelled(bool value) { cancelled = value; }
    bool isCancelled() const {return cancelled;}
};