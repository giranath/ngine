#ifndef NGINE_GAMEPLAY_GAME_INSTANCE_HPP
#define NGINE_GAMEPLAY_GAME_INSTANCE_HPP

namespace ng
{

/**
 * Contains game specific definitions
 * This is where a game will configure all it's game specific rules and content
 */
struct game_instance
{
    virtual ~game_instance() = default;

    /// Add various hooking methods to configure the engine
};

}

#endif
