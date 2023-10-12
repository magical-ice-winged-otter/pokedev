#ifndef POKEDEV_APPLICATION_HPP
#define POKEDEV_APPLICATION_HPP

#include "porytiles_gui.hpp"
#include "serializer.hpp"

/// The logic that drives the GUI and all it's supporting features.
namespace Application
{
    void init();
    void shutdown();
    void render();
};

#endif // POKEDEV_APPLICATION_HPP