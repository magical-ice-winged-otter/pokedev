#ifndef POKEDEV_TOOL_HPP
#define POKEDEV_TOOL_HPP

class PokeDevTool
{
public:
    virtual ~PokeDevTool() {};
    virtual void renderWindow() = 0;
    const char* name;
    bool isActive;
};

#endif // POKEDEV_TOOL_HPP