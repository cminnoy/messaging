# File tree

<pre><font color="#3465A4"><b>.</b></font>
├── <font color="#4E9A06"><b>build.sh</b></font>
├── CMakeLists.txt
├── <font color="#4E9A06"><b>configure.sh</b></font>
├── <font color="#3465A4"><b>examples</b></font>
│   ├── <font color="#3465A4"><b>atm</b></font>
│   │   ├── atm_machine.hpp
│   │   ├── atm_messages.hpp
│   │   ├── bank_machine.hpp
│   │   ├── CMakeLists.txt
│   │   ├── interface_machine.hpp
│   │   ├── logger.hpp
│   │   └── main.cpp
│   ├── atm_machine.hpp
│   ├── CMakeLists.txt
│   └── <font color="#3465A4"><b>task</b></font>
│       ├── CMakeLists.txt
│       └── main.cpp
├── <font color="#3465A4"><b>include</b></font>
│   ├── <font color="#3465A4"><b>messaging</b></font>
│   │   ├── async.hpp
│   │   ├── broadcasting_process.hpp
│   │   ├── close_queue.hpp
│   │   ├── <font color="#3465A4"><b>coroutine</b></font>
│   │   │   ├── all.hpp
│   │   │   ├── message_queue.hpp
│   │   │   └── types.hpp
│   │   ├── dispatcher_base.hpp
│   │   ├── dispatcher.hpp
│   │   ├── interfaces.hpp
│   │   ├── <font color="#3465A4"><b>multithread</b></font>
│   │   │   ├── all.hpp
│   │   │   ├── message_queue.hpp
│   │   │   └── types.hpp
│   │   ├── naming.hpp
│   │   ├── process.hpp
│   │   ├── process_manager.hpp
│   │   ├── pump.hpp
│   │   ├── queue_base.hpp
│   │   ├── receiver.hpp
│   │   ├── sender.hpp
│   │   ├── stop_source.hpp
│   │   ├── stop_token.hpp
│   │   └── template_dispatcher.hpp
│   └── <font color="#3465A4"><b>util</b></font>
│       ├── algorithm.hpp
│       ├── allocator.hpp
│       ├── attributes.hpp
│       ├── bundle.hpp
│       ├── chrono_io.hpp
│       ├── debug.hpp
│       ├── inspect.hpp
│       ├── lock_free_queue.hpp
│       ├── math.hpp
│       ├── pmte.hpp
│       ├── pointer_cast.hpp
│       ├── shared_library.hpp
│       ├── simulated_clock.hpp
│       ├── traits.hpp
│       └── type_info.hpp
├── Jenkinsfile
├── LICENSE.txt
├── README.md
├── <font color="#4E9A06"><b>reformat.sh</b></font>
├── <font color="#3465A4"><b>src</b></font>
│   ├── util__allocator.cpp
│   ├── util__debug.cpp
│   ├── util__pmte.cpp
│   ├── util__simulated_clock.cpp
│   └── util__type_info.cpp
└── <font color="#3465A4"><b>tests</b></font>
    ├── CMakeLists.txt
    ├── test_bundle.cpp
    ├── test_handler.cpp
    ├── test_messaging.cpp
    ├── test_messaging_perf.cpp
    ├── test_pmte.cpp
    ├── test_pmte_traits.cpp
    ├── test_simulated_clock.cpp
    ├── test_type_info.cpp
    └── test_visitor.cpp
</pre>

## Messaging
## Multithreaded and coroutine messaging framework for C++14 and above.

Original idea from Antony Williams

### Intro

The framework provides functionality to safely send messages as instances of **struct**/**class** or basic types between processes in a fully multi-threaded environment.
A process is a concept of a long duration task that reacts on incoming messages for event based programming.
A process can use preemptive multithreading using the C++11 *std::thread* class or it can use cooperative multithreading using the BOOST coroutine2 library.
The framework is open enough to allow other cooperative and/or multithreaded frameworks to be used in a later stage; for example GLib.

The class *process* is used as the base class for both coroutine and multithreaded processes. The process manager expects that every process inherits from this class.
By providing the *types* template the developer can select at compile time if the process will be run multithreaded or cooperatively without changes to the process implementation from the users point of view.

Each *process* has a *receiver* which holds a lock-free queue to store the messages it receives until they can be processed.
The *sender* class simply holds a reference to the *receiver* and allows anyone having a copy of the *sender* to send messages to the *receiver*.
Note that the *sender* class does not check if the receiver still exists; this is the users reponsibility to manage.

A *process* is started by calling the **run** function with the *yield* argument. Normally this **run** function is called once by the **process_manager**.
The *run* function holds a loop which calls the virtual *dispatch* function.
Derivatives of class *process* may have to override the **dispatch** function, which has as its argument a *dispatcher* object.
It is also possible to attach statically handlers as template arguments as well as add/remove extra handlers during runtime dynamically.
The static and dynamic handlers which are added to the process are often more performant during dispatch than those attached in the **dispatch** method.
This is because the internal **visitor** uses hash caching and binary search to improve lookup to match the right handler with the received type.
If your process has a lot of 'states' where it can receive different kind of messages in each state you may be better off overriding the **dispatch** method.
See the examples for means of doing this.

The *dispatcher* instance can be used to build up a message processing chain on the stack of the current thread, that will sit idle until a message is received in the queue and the thread regains control.
The stack based chain will be destructed during the search for an apropriate handler.
If an appropriate handler is found that can accept the message type it will be called and executed; thereafter the chain will be further destructed until no more handlers remain.
If no handlers remains the execution continues in the *dispatch* function whereafter the control returns back to the *run* function which holds the loop.
The infinite event loop will be broken when the message *close_queue* has been received and handled.

To make the usage of processes more conveniant the *pump* classes and the class *process_manager* are introduced.

A *pump* acts like a controller and container for a specific type of processes; one pump for coroutine based processes and one for multithreaded processes. The developer can add at any given time a new process to a pump; the pump will then start the process for execution. The pump allows to query for a specific process, send messages to all processes it contains, query for a sender and query if, and how many, processes are still active. For convenience there are also broadcast pumps, that take process with
attached observers.

The *pump* class also allows the developer to add *functions* to the container, which will be executed just as other processes.
These functions get a stop token and need to accept a *yield* argument which must be used by the developer to return control to the pump (in cooperative functions; in multithreaded functions yielding is not needed but allowed; care must be taken not to overload the CPU within multithreaded loops).
Functions do not have *receiver* objects so you can't send a message to a function but you can send messages from a *function* to a *process*.
The instance of *Process* can be asked to stop by calling the *done* function; functions can be asked to stop by using the *stop_source* object.
For convenience of query each process and function will carry a name.
Multi-threaded processes may repond to messages to sleep, change thread name and/or have a heartbeat.
Broadcast processes are processes that keep a list of interested listeners and they MAY broadcast messages to those listeners.

The *process_manager* can hold one or more pumps; selectable by the developer at compîle time.
This allows you to write and use processes in a uniform way, regardless if the process should run as a coroutine or preemptive thread.
The developer is free to change his/her mind later in the process.

Note that all coroutines run on the same thread as the manager_process.
If you need to have coroutines on multiple threads, you need to create a different managers or pumps on that thread.
This could be done inside a process, with for example a heartbeat.

Messages are always send by copy of moved to the receivers queue; they are stored in the receivers queue.
Make sure that for complex message structures you implement the move constructor and/or move assignment operator well to avoid performance hits, or use *std::unique_ptr* or *std::shared_ptr* if you can't move or copy your object.
*std::unique_ptr* should not be used for broadcasting as this will not give the result you expect; use a plain object, a plain pointer or a *std::shared_ptr* instead.

To avoid programming mistakes sending by reference is not allowed; you will get a compiler error if you try to do so.
Similarly volatile types are also not allowed.
Also note that a handler that can only accept a non-const type can not process a const message and will silently be ignored.
Sending a non-const message to a const handler is perfectly possible.
If a type can be processed by more than one handler, the system of detection may vary depending if one uses the chained-dispatcher method or the dynamicaly visitor system.
In the chained-dispatcher method the first handler that is capable of handling the type in the unwinding chain will be selected.
This may or may not be what you intended to happen.
For handlers registered with the visitor the most specific handler will be selected. If the match is exact that handler will be selected, otherwise it will query the inheritance chain as described by the user using the *type_list*. Then the best match will be selected.
Note that if the send type is non-const and there are two possible handlers that can process the type, one being const and one being non-const, the non-const handler has precedence.
Be aware that the PMTE system can not detect automatically the inheritance tree from a type; there is no such mechanism in C++ available.
During construction of a PMTE instance you will need to provide the inheritance tree explicitly using *type_list* if you which to use polymorphism.

### Example

As an example an ATM cash machine is implemented.

The class *atm* is the representation of the cash vending machine. It can be in different states which are represented by functions inside the class. The internal helper class *state* holds the actual state and helps in triggering events
on state change. The function *bind* is a conveniance function and wraps the *std::bind* function for improved readability.

The class *bank_machine* represents the server at the main office of the bank, which responds on requests from the *atm*.
Interestingly this shows how to pass *sender* alongside messages to state the return address for a response.

The class *interface_machine* represents the screen the user sees at the ATM.

The numeric keypad at the ATM is implemented as a function in main.cpp.

The developer is free in his choice to make a process/function coroutine based or preemptive. Changing the bank from

    manager.add<multithread_types>("bank", factory::create_bank_machine<multithread_types>());

towards

    manager.add<coroutine_types>("bank", factory::create_bank_machine<coroutine_types>());

will result in the same behaviour towards the user.

The example *task* shows how to send asynchronous tasks to a worker process and waits for the result. Also the usage of an *awaitable* query is shown querying the process name of the worker.
### Testing

Tested on:
- g++ v7  (c++14, c++17)
- g++ v8  (c++14, c++17, c++20)
- g++ v9  (c++14, c++17, c++20)
- g++ v10 (c++14, c++17, c++20)
- g++ v11 (c++14, c++17, c++20)
- clang++ v10 (c++14, c++17, c++20)
- clang++ v12 (c++14, c++17, c++20) 

# Util

## Generic utilities to aid debugging, simulation, DLL exposure, memory allocation, math and pointer managment.
### attributes

Check standard:
- CPP98_STANDARD
- CPP11_STANDARD
- CPP14_STANDARD
- CPP17_STANDARD
- CPP20_STANDARD
- CPP23_STANDARD

Attributes:
- NO_RETURN     [[noreturn]] c++11 attribute in the form of a macro
- DEPRECATED    [[deprecated]] c++14 attribute in the form of a macro
- MAYBE_UNUSED  [[maybe_unused]] c++17 attribute in the form of a macro
- NO_DISCARD    [[nodiscard]] c++17 attribute in the form of a macro
- FALL_THROUGH  [[falltrough]] c++17 attribute in the form of a macro
- LIKELY	    [[likely]] c++20 attribute in the form a a macro
- UNLIKELY	    [[unlikely]] c++20 attribute in the form of a macro
- FORCE_INLINE  Enforce function to be inlined and don't let the compiler decide
- NO_INLINE     Don't let the function be inlined
- FLATTEN       Inline all the called functions in the body of the callee
- GTEST_NON_VIRTUAL / GTEST_VIRTUAL helper macros that can make a function virtual under testing conditions

### shared library

Helper macros for shared library identifier exposure. Usefull if you need to keep all but some functions/classes private when creating your own library.

### traits and naming

Helper functions to get the name of a type in textual form and its traits.
*type_name* is different than in BOOST as this one is constexpr while in BOOST it is std::string.
(see https://bitwizeshift.github.io/posts/2021/03/09/getting-an-unmangled-type-name-at-compile-time/)

Usage:

    std::cout << type_name<void*>() << std::endl;
    std::cout << type_primary_category<int>() << std::endl;

Note that type names are not consistent accros compilers; const/volatile and pointer/reference locations in the string may differ.
If that is important to you, you'll need to implement a small parser for the string type.

The *signature* structs are usefull for analysing the return type and argument types of functions.

### allocator (C++17 and above)

Static and PMR allocator helper classes that take statistics of allocations/deallocations.
If you have the Tracy profiler installed you may enhance your local memory allocator with Tracy profiling support using **memory_resource_trace** (see: https://github.com/wolfpld/tracy)

Tested on:
- g++ v9  (c++17, c++20)
- g++ v10 (c++17, c++20)
- g++ v11 (c++17, c++20)
- clang++ v10 (c++17, c++20)
- clang++ v12 (c++17, c++20)

### math

Some basic compare functions for floating point types to take into account rounding errors and epsilon.

### debug

Adds support for output channels. The following channels are implemented:

    - debug
    - trace
    - info
    - warning
    - error
    - stats
    - null
    - test

You can print a debug message on the screen by:

    util::logger::debug << "Hey, I'm able to print specific debug messages!" << std::endl;

Each channel can be enabled or disabled:

    debug.enable(false);

You can select if you want ANSI escape sequences (colour/italic/bold) to pass through the channel or not. If you want to send your output to a file it might be wise to disable ANSI while during development your may want to have it turned on to enhance readability.

Other features are:

- Indicate which thread is printing text.
- Show incrementing line numbers; have tracing messages refer to line numbers when exiting for enhance readability.
- Indent tracing arguments
- Change the stream object (default std::cout)
- Clear the buffer

Cpp files can be turned into logical modules for printing. For example you have some domain logic in one cpp file. You can turn printing on/off for each module independantly.
During tracing the module that did the printing will be show on the schreen.

By default GLB is the global scope. If you don't define TRACER_MODULE_NAME in your .cpp file you will use GLB scope.
#### TRACER_FUNC

Prints tracing of annotated functions. Add **TRACER_FUNC()** as the first line in your function.
At entrance of the function during runtime the name of the function will be send to the screen. When leaving the function an exit message will be printed.

From C++17 onwards you can add the function arguments to be printed.

    TRACER_FUNC(arg1, arg2, arg3);

Tracer func supports up to 16 arguments. Make sure that each argument can be send to an **ostream**, so for classes you need to add and implement the overload:

    std::ostream & operator<<(std::ostream&, myclass const& o) {
        return out << o.field1;
    }

If your function returns a value replace basic returns like

    return myvalue;

with

    TRACER_RETURN(myvalue);

This will print also the return value on the screen. You can also do **TRACER_RETURN()** without arguments for a plain **return**.
A TRACER_RETURN prints the filename and line number of the exit point.

You can also look towards an alternative logger with many features: https://github.com/gabime/spdlog

### simulated clock

A *std::chrono* compatible clock that advances only on request.
The clock has nanosecond accuracy which should suffice for most simulations. The startpoint is UNIX epoch.

### bundle

Helper class around tuple to allows to attach two or more shared pointers together, each pointer pointing to it's own hierarchical class tree, allowing static and dynamic casting on all pointers at the same time. This allows independent development of those class trees with loose binding.

Each base class of a tree hierarchy must have a virtual destructor and inherit itself from **std::enable_shared_from_this** to make casting work. Those constraints are checked by static_asserts.

example:

    struct Tree1 : public std::enable_shared_from_this<Tree1> {
        virtual ~Tree1() noexcept {}
        void foo() { std::cout << "foo\n"; }
    };

    struct Tree2 : public std::enable_shared_from_this<Tree2> {
        virtual ~Tree2() noexcept {}
    };

    struct Tree3 : public std::enable_shared_from_this<Tree3> {
        virtual ~Tree3() noexcept {}
        virtual void hello_world() { std::cout << "Hello World!\n"; }
    };

    struct Child1 : Tree1 {
        using Tree1::Tree1;
    };

    struct Child2 : Tree2 {
        using Tree2::Tree2;
        void bar() { std::cout << "bar\n"; }
    };

    struct Child3 : Tree3 {
        using Tree3::Tree3;
        void foobar() { std::cout << "foobar\n"; }
        void hello_world() override { std::cout << "HELLO WORLD!\n"; }
    };

    util::bundle<Child1,Child2,Child3> source_bundle {
        std::make_shared<Child1>(),
        std::make_shared<Child2>(),
        std::make_shared<Child3>()
    };

    util::bundle<Tree1,Tree2,Tree3> base_bundle = source_bundle; // automatic static cast
    if (base_bundle) {
        base_bundle.get<0>()->foo();
        base_bundle.get<2>()->hello_world();
    }

    util::bundle<Tree1,Child2,Tree3> mixed_bundle = base_bundle; // automatic dynamic cast
    mixed_bundle()
    .then([](auto &b) {
        std::cout << "Yeah, I'm a winner!\n";
        b.template get<1>()->bar();
    }).or_else([](auto &) {
        std::cout << "Not for me :-(\n";
    });

# Tools

Thanks to Alan de Freitas for:
https://github.com/alandefreitas/clang-unformat
