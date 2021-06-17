#ifndef NGINE_CORE_TIME_HPP
#define NGINE_CORE_TIME_HPP

#include <chrono>

namespace ng
{

/**
 * Clock used to measure duration of a frame
 */
using frame_clock = std::chrono::steady_clock;

/**
 * Store the duration of a single frame as seconds
 */
using frame_duration = std::chrono::duration<double>;

}

#endif