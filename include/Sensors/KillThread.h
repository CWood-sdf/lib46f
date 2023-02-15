#include "vex.h"
#ifndef KILL_THREAD_H
#define KILL_THREAD_H
struct KillThread
{
    static inline vector<thread*> instances;
    thread* t;

    KillThread(void (*callback)(void)) : t(new thread(callback))
    {
        instances.push_back(t);
    }
    KillThread(thread&& t) : t(&t)
    {
        instances.push_back(this->t);
    }
    KillThread& operator=(thread&& t)
    {
        this->t = &t;
        instances.push_back(&t);
        return *this;
    }
    static void killAll()
    {
        cout << "KAT " << instances.size() << endl;
        for (auto i : instances)
        {
            // i->join();
            i->interrupt();
            // i->detach();
        }
        instances.clear();
    }
};
#endif
