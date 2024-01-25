#include "event.hpp"

using namespace PokeDev;

Event::ListenerHandle Event::addListener(Event::Listener listener, void* userData) {
    ListenerData data {
        .listener = listener,
        .userData = userData,
    };
    m_listeners.push_back(data);
    return static_cast<int>(m_listeners.size() - 1);
}

void Event::removeListener(Event::ListenerHandle handle) {
    m_listeners.erase(m_listeners.begin() + handle);
}

void Event::invoke() {
    for (int i = 0; i < m_listeners.size(); ++i) {
        m_listeners[i].listener(m_listeners[i].userData);
    }
}

Event::Event()
        : m_listeners {}
{}
