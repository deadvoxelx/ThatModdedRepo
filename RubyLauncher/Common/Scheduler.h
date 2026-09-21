#pragma once
#include <cstdint>
#include <cstdio>
#include <map>
#include <memory>
#include "sol/sol.hpp"
#include "Common/EventSystem/EventBus.h"
class Scheduler {
public:
    int schedule(double delayTicks, sol::protected_function fn);
    int scheduleEvery(double intervalTicks, sol::protected_function fn);
    void cancel(int id);
    void clear();
    void tick();

private:
    struct Task {
        int id;
        uint64_t dueTick;
        uint64_t interval;
        sol::protected_function fn;
    };

    static uint64_t ticksFrom(double ticks);
    uint64_t m_curTick = 0;
    int m_nextId = 1;
    std::map<int, std::unique_ptr<Task>> m_tasks;
    std::multimap<uint64_t, int> m_due;
};

inline uint64_t Scheduler::ticksFrom(double ticks)
{
    if (ticks <= 0.0) return 0;
    if (ticks >= 9.0e15) return static_cast<uint64_t>(9.0e15 - 1.0);
    return static_cast<uint64_t>(ticks + 0.5);
}

inline int Scheduler::schedule(double delayTicks, sol::protected_function fn)
{
    if (!fn.valid()) return -1;
    int id = m_nextId++;
    uint64_t due = m_curTick + ticksFrom(delayTicks);
    auto task = std::make_unique<Task>(Task{ id, due, 0, std::move(fn) });
    m_tasks[id] = std::move(task);
    m_due.emplace(due, id);
    return id;
}

inline int Scheduler::scheduleEvery(double intervalTicks, sol::protected_function fn)
{
    if (!fn.valid()) return -1;
    int id = m_nextId++;
    uint64_t interval = ticksFrom(intervalTicks);
    uint64_t due = m_curTick + interval;
    auto task = std::make_unique<Task>(Task{ id, due, interval, std::move(fn) });
    m_tasks[id] = std::move(task);
    m_due.emplace(due, id);
    return id;
}

inline void Scheduler::cancel(int id)
{
    m_tasks.erase(id);
}

inline void Scheduler::clear()
{
    m_tasks.clear();
    m_due.clear();
}

inline void Scheduler::tick()
{
    ++m_curTick;
    while (!m_due.empty() && m_due.begin()->first <= m_curTick)
    {
        auto dueIt = m_due.begin();
        int id = dueIt->second;
        m_due.erase(dueIt);
        auto taskIt = m_tasks.find(id);
        if (taskIt == m_tasks.end()) continue;
        auto task = std::move(taskIt->second);
        m_tasks.erase(taskIt);
        auto result = task->fn();
        if (!result.valid())
        {
            fprintf(stderr, "[Lua] Error in scheduled task %d: %s\n", id, safeLuaErrorText(result).c_str());
        }

        if (task->interval > 0)
        {
            task->dueTick = m_curTick + task->interval;
            m_tasks[id] = std::move(task);
            m_due.emplace(m_tasks[id]->dueTick, id);
        }
    }
}
