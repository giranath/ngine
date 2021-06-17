#include "application.hpp"
#include <ng/gpu/gpu_context.hpp>

#include "keyboard.hpp"

#include <iostream>

namespace ng
{

void application::prepare_for_application(int argc, char* argv[])
{
    gpu_context::load_driver();
}

application::application()
: application(0, nullptr)
{

}

application::application(int argc, char* argv[])
: running_(true)
{

}

void application::tick(frame_duration dt)
{

}

void application::render()
{

}

bool application::running() const noexcept
{
    return running_;
}

void application::on_quit()
{
    running_ = false;
}

void application::on_mouse_pressed(mouse_buttons button, int count, int x, int y)
{

}

void application::on_mouse_released(mouse_buttons button, int x, int y)
{

}

void application::on_mouse_cursor_moved(int x, int y)
{

}

void application::on_mouse_moved(int x, int y)
{

}

void application::on_mouse_scrolled(int delta_x, int delta_y, bool flipped)
{

}

void application::on_key_pressed(const keyboard_key& key, bool repeat)
{

}

void application::on_key_released(const keyboard_key& key)
{

}

void application::clear_keymods() noexcept
{
    key_modifiers_ = 0;
}

void application::enable_keymod(keyboard_modifiers modifier) noexcept
{
    key_modifiers_ |= static_cast<uint16_t>(modifier);
}

bool application::keymod(keyboard_modifiers modifiers) const noexcept
{
    return (key_modifiers_ & static_cast<uint16_t>(modifiers)) != 0;
}

}