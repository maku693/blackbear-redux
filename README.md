# BlackBear Redux

BlackBear Redux is a [Redux](https://github.com/reactjs/redux)-like header-only library for C++.

## Example

```cpp
#include<BlackBear/Redux.hpp>
#include<iostream>

// Here we define a class for state, but any types includes fundamental
// types are valid for state.
struct State final {
private:
    int m_counter;

public:
    constexpr int getCounter() const
    {
        return m_counter;
    }

    constexpr void setCounter(int counter)
    {
        this->m_counter = counter;
    }
};


// Actions are `tag classes` to change behavior of a reducer.
// We don't define any members for actions here, but ofcource you can use
// action's member in the reducer, because not only it's type but also an
// instance is passed to the reducer.
namespace Action {
    struct Increment final {};
    struct Decrement final {};
}

// You can use any type of overloaded function object (i.e. A class that
// implements `operator()`, overloaded lambdas, and so on) for a reducer.
struct Reducer final {

    // Get the state and actions by const reference is good for performance
    // when objects are relatively large, but not neccesary.
    constexpr auto operator()(
        const State& prevState,
        const Action::Increment&
    ) const
    {
        auto state = State(prevState);
        state.setCounter(prevState.getCounter() + 1);
        return state;
    }

    constexpr auto operator()(
        const State& prevState,
        const Action::Decrement&
    ) const
    {
        auto state = State(prevState);
        state.setCounter(prevState.getCounter() - 1);
        return state;
    }

};

int main()
{
    // Create a store with a reducer instance and initial state.
    // `makeStore()` is just a helper function, so you can create a in-
    // stance of `Store` class by hand, but we recommend to use
    // `makeStore()` because sometimes their declaration is complicated.
    auto store = BlackBear::makeStore(Reducer{}, State{});

    // Subscribe to store to get notifications of a state update.
    store.subscribe(
        [&store]()
        {
            std::cout << store.getState().getCounter() << std::endl;
        }
    );

    // Change the state by dispatching actions to the store.
    // This will prints "1 2 1 2".
    store.dispatch(Action::Increment{});
    store.dispatch(Action::Increment{});
    store.dispatch(Action::Decrement{});
    store.dispatch(Action::Increment{});
}
```
