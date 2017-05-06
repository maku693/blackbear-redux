#include <functional>
#include <memory>
#include <vector>

namespace BlackBear {

template <class ReducerT, class StateT>
struct Store final {
private:
    using ListenerT = std::function<void()>;

    ReducerT m_reducer;
    StateT m_state;

    std::vector<ListenerT> m_listeners;

public:
    Store() = default;

    Store(ReducerT&& reducer, StateT&& state) noexcept
        : m_reducer(std::move_if_noexcept(reducer))
        , m_state(std::move_if_noexcept(state))
    { }

    constexpr auto getState() const noexcept
    {
        return this->m_state;
    }

    template <class EventT>
    void dispatch(EventT&& event)
    {
        this->m_state = m_reducer(
            this->m_state,
            std::forward<EventT>(event)
        );

        for (auto listener : this->m_listeners) {
            listener();
        }
    }

    void subscribe(ListenerT listener) noexcept
    {
        m_listeners.push_back(listener);
    }
};

template <class ReducerT, class StateT>
auto makeStore(ReducerT&& reducer, StateT&& state)
{
    return Store<ReducerT, StateT> {
        std::forward<ReducerT>(reducer),
        std::forward<StateT>(state)
    };
}

}
