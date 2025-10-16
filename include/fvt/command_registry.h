#ifndef FVT_COMMAND_REGISTRY_H
#define FVT_COMMAND_REGISTRY_H


#include "fvt/command_parser.h"

namespace fvt {

    /**
     * Registers all available commands with the provided CommandParser instance.
     * @param parser The CommandParser instance to register commands with.
     */
    void register_commands(CommandParser& parser);
}

#endif // FVT_COMMAND_REGISTRY_H