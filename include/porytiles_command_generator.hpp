#ifndef PORYTILES_GUI_COMMAND_GENERATOR_HPP
#define PORYTILES_GUI_COMMAND_GENERATOR_HPP

#include <string>
#include "porytiles_context.hpp"

namespace PorytilesCommandGenerator
{
    void init();
    void renderImGui();
    void shutdown();

    std::string generateCompilePrimaryCommand(PorytilesContext& context);
    std::string generateCompileSecondaryCommand(PorytilesContext& context);
    std::string generateDecompilePrimaryCommand(PorytilesContext& context);
    std::string generateDecompileSecondaryCommand(PorytilesContext& context);

} // namespace PorytilesCommandGenerator

#endif // PORYTILES_GUI_COMMAND_GENERATOR_HPP