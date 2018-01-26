#include "Engine/Threading/Event.hpp"

class MethodExample
{
public:
    int some_method(int a, int b)
    {
        printf("Hi: %i, %i\n", a, b);
        return a + b;
    }

    void some_other_method(int a, int b)
    {
        printf("oooo, magic.... %i\n", a + b);
    }

    typedef int (MethodExample::*method_cb)(int, int);
    method_cb cb;
};

int SomeFunction(MethodExample*, int a, int b)
{
    printf("Hi: %i, %i\n", a, b);
    return a * b;
}

typedef int(__cdecl *func_cb)(MethodExample*, int, int);


void EventTest()
{
    int v = 0;

    MethodExample example;
    v = example.some_method(1, 2);
    v = SomeFunction(&example, 3, 4);

    Event<int, int> sample_event;
    sample_event.subscribe_method(&example, &MethodExample::some_other_method);
    sample_event.unsubscribe_method(&example, &MethodExample::some_other_method);
    sample_event.trigger(5, 12);


    MethodExample::method_cb cb = &MethodExample::some_method;
    example.cb = &MethodExample::some_method;
    (example.*cb)(5, 7);

    func_cb fa = SomeFunction;
    auto test = &(MethodExample::some_method);
    func_cb fb = *(func_cb*)(void*)&test;

    v = fa(&example, 1, 2);
    v = fb(&example, 3, 4);
}