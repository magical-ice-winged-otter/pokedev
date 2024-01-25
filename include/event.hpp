#ifndef POKEDEV_EVENT_HPP
#define POKEDEV_EVENT_HPP

#include <vector>

namespace PokeDev {
    class Event {
    public:
        typedef void (*Listener)(void* userData);
        typedef int ListenerHandle;
        Event();
        ListenerHandle addListener(Listener listener, void* userData = nullptr);
        void removeListener(ListenerHandle handle);
        void invoke();
    private:
        struct ListenerData {
            Listener listener;
            void* userData;
        };
        std::vector<ListenerData> m_listeners;
    };
}

#endif // POKEDEV_EVENT_HPP