#ifndef NGINE_APP_MOUSE_HPP
#define NGINE_APP_MOUSE_HPP

#include <cstdint>

namespace ng
{

/**
 * Enumerates all supported mouse buttons
 */
enum class mouse_buttons : uint8_t
{
    left,
    right,
    middle,
    extra_1,
    extra_2
};

}

#endif
