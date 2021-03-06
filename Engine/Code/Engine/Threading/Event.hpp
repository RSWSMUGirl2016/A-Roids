#pragma once

// Equivalent of a C# Delegate

#include "Engine/EngineCommon.hpp"
#include "Engine/Threading/Event.hpp"

#include "Engine/Threading/CriticalSection.hpp"

#include <vector>

// Infoknowledge Management System

//--------------------------------------------------------------------
// EVENT VERSION 0
// Basic Idea in place.  
// - Not Thread Safe
// - Only works with void* data
// - Only works with C style functions
//--------------------------------------------------------------------

// An Event/Delegate at its core is just a list of function callbacks
//
// Events can be registered with some identifying information [user_arg].
// And calling the event can take some event specific data [event_arg]
//typedef void(*basic_event_cb)(void *user_arg, void *event_arg);
//typedef void(*basic_event_two_cb)(void *user_arg1, void *user_arg2, void *event_arg);
//
//class EventV0
//{
//    // subscription - when subscribing this is the identifying 
//    // information (what to call, and what to call with)
//    struct event_sub_t
//    {
//        basic_event_cb cb;
//        void *user_arg;
//    };
//
//    struct event_sub_two_t
//    {
//        basic_event_two_cb cb;
//        void *user_arg1;
//        void *user_arg2;
//    };
//
//public:
//    EventV0()
//    {
//    }
//
//    ~EventV0()
//    {
//        subscriptions.clear();
//        subscriptionsTwo.clear();
//    }
//
//    void subscribe(void *user_arg, basic_event_cb cb)
//    {
//        // Good safeguard in debug to add is to make sure
//        // you're not double subscribing to an event
//        // with a similar pair. 
//        // TODO - ASSERT AGAINST ABOVE
//
//        // Add subscriptoin
//        event_sub_t sub;
//        sub.cb = cb;
//        sub.user_arg = user_arg;
//        subscriptions.push_back(sub);
//    }
//
//    void subscribeTwo(void *user_arg1, void *user_arg2, basic_event_two_cb cb)
//    {
//        // Good safeguard in debug to add is to make sure
//        // you're not double subscribing to an event
//        // with a similar pair. 
//        // TODO - ASSERT AGAINST ABOVE
//
//        // Add subscriptoin
//        event_sub_two_t sub;
//        sub.cb = cb;
//        sub.user_arg1 = user_arg1;
//        sub.user_arg2 = user_arg2;
//        subscriptionsTwo.push_back(sub);
//    }
//
//    void unsubscribe(void *user_arg, basic_event_cb cb)
//    {
//        for (int i = 0; i < (int)subscriptions.size(); ++i) {
//            event_sub_t &sub = subscriptions[i];
//            if ((sub.cb == cb) && (sub.user_arg == user_arg)) {
//                subscriptions.erase(subscriptions.begin() + i);
//                return; // should be unique, so return immeidately
//            }
//        }
//    }
//
//    void unsubscribeTwo(void *user_arg1, void *user_arg2, basic_event_two_cb cb)
//    {
//        for (int i = 0; i < (int)subscriptionsTwo.size(); ++i) {
//            event_sub_two_t &sub = subscriptionsTwo[i];
//            if ((sub.cb == cb) && (sub.user_arg1 == user_arg1) && (sub.user_arg2 == user_arg2)) {
//                subscriptionsTwo.erase(subscriptionsTwo.begin() + i);
//                return; // should be unique, so return immeidately
//            }
//        }
//    }
//
//    void trigger(void *event_arg)
//    {
//        for (int i = 0; i < (int)subscriptions.size(); ++i) {
//            event_sub_t &sub = subscriptions[i];
//            sub.cb(sub.user_arg, event_arg);
//        }
//    }
//
//    void triggerTwo(void *event_arg)
//    {
//        for (int i = 0; i < (int)subscriptionsTwo.size(); ++i) {
//            event_sub_two_t &sub = subscriptionsTwo[i];
//            sub.cb(sub.user_arg1, sub.user_arg2, event_arg);
//        }
//    }
//
//public:
//    std::vector<event_sub_t> subscriptions;
//    std::vector<event_sub_two_t> subscriptionsTwo;
//};

//--------------------------------------------------------------------
// EVENT VERSION 1
// - Thread Safety around 
// - Add support for callbacks without user data
// - Adds supports for methods
//--------------------------------------------------------------------
// TODO

//--------------------------------------------------------------------
// EVENT VERSION 2
// - Templated argument types
//--------------------------------------------------------------------
// TODO

//--------------------------------------------------------------------
// EVENT VERSION 3
// - Thread Safety [ReaderWriter Locks]
// - 
//--------------------------------------------------------------------
// Thread Safe

template <typename ...ARGS>
class Event
{
public:
    // TYPES:
    struct event_sub_t;
    typedef void(*cb_t)(event_sub_t*, ARGS...);
    typedef void(*cb_with_arg_t)(void*, ARGS...);
    // subscription - when subscribing this is the identifying 
    // information (what to call, and what to call with)

    struct event_sub_t
    {
        cb_t cb;
        void *secondary_cb;
        void *user_arg;
    };

    // STATIC FUNCTIONS AS GO BETWEENS BETWEEN
    // THE EXPECTED CALLBACK AND UNIVERSAL CALLBACK
    static void FunctionWithArgumentCallback(event_sub_t *sub, ARGS ...args);

    template <typename T, typename MCB>
    static void MethodCallback(event_sub_t *sub, ARGS ...args);

public:
    Event()
    {
    }

    ~Event()
    {
        subscriptions.clear();
    }

    // Subscribe a single function (
    void subscribe(void *user_arg, cb_with_arg_t cb)
    {
        // Good safeguard in debug to add is to make sure
        // you're not double subscribing to an event
        // with a similar pair. 
        // TODO - ASSERT AGAINST ABOVE

        // Add subscriptoin
        event_sub_t sub;
        sub.cb = FunctionWithArgumentCallback;
        sub.secondary_cb = cb;
        sub.user_arg = user_arg;
        subscriptions.push_back(sub);
    }


    // Unsubscribe a function (using user argument as well)
    void unsubscribe(void *user_arg, void* cb)
    {
        for (int i = 0; i < (int)subscriptions.size(); ++i) {
            event_sub_t &sub = subscriptions[i];
            if ((sub.secondary_cb == cb) && (sub.user_arg == user_arg)) {
                subscriptions.erase(subscriptions.begin() + i);
                return; // should be unique, so return immeidately
            }
        }
    }

    // remove all subscriptions using this user arg.
    void unsubscribe_by_argument(void *user_arg)
    {
        for (int i = 0; i < subscriptions.size(); ++i) {
            event_sub_t &sub = subscriptions[i];
            if (sub.user_arg == obj) {
                // don't return, just remove this object [could do a fast removal if order doesn't matter
                // by just setting last to this and popping back]
                subscriptions.erase(subscriptions.begin() + i);
                --i;
            }
        }
    }

    // Be able to subscribe a method;
    template <typename T>
    void subscribe_method(T *obj, void (T::*mcb)(ARGS...))
    {
        event_sub_t sub;
        sub.cb = MethodCallback<T, decltype(mcb)>;
        sub.secondary_cb = *(void**)(&mcb);
        sub.user_arg = obj;
        subscriptions.push_back(sub);
    }

    // unsubscribe - just forwards to normal unsubscribe
    template <typename T>
    void unsubscribe_method(T *obj, void (T::*mcb)(ARGS...))
    {
        unsubscribe(obj, *(void**)&mcb);
    }

    // unsubscribe all allocations for this object;
    template <typename T>
    void unsubscribe_object(T *obj)
    {
        unsubscribe_by_argument(obj);
    }

    // Triggers the call - calls all registered callbacks;
    void trigger(ARGS ...args)
    {
        for (int i = 0; i < (int)subscriptions.size(); ++i) {
            event_sub_t &sub = subscriptions[i];
            sub.cb(&sub, args...);
        }
    }

public:
    std::vector<event_sub_t> subscriptions;
};


//--------------------------------------------------------------------
template <typename ...ARGS>
void Event<ARGS...>::FunctionWithArgumentCallback(event_sub_t *sub, ARGS ...args)
{
    cb_with_arg_t cb = (cb_with_arg_t)(sub->secondary_cb);
    cb(sub->user_arg, args...);
}

//--------------------------------------------------------------------
template <typename ...ARGS>
template <typename T, typename MCB>
void Event<ARGS...>::MethodCallback(event_sub_t *sub, ARGS ...args)
{
    MCB mcb = *(MCB*)&(sub->secondary_cb);
    T *obj = (T*)(sub->user_arg);
    (obj->*mcb)(args...);
}

void EventTest();