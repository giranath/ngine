#include <ng/gpu/gpu_context.hpp>
#include <ng/app/application.hpp>
#include <ng/app/mouse.hpp>
#include <ng/app/keyboard.hpp>
#include <SDL.h>

#include <iostream>
#include <thread>

int launch_sdl_application(int argc, char* argv[]);

/**
 * Entry point for the game
 * @param argc The number of arguments that were passed to the game
 * @param argv The arguments vector passed to the game
 * @return the exit code of the game
 */
int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "failed to initialize SDL" << std::endl;
        return 1;
    }

    const int res = launch_sdl_application(argc, argv);

    SDL_Quit();

    return res;
}

int launch_window_application(int argc, char* argv[], SDL_Window* window, SDL_GLContext opengl);

// At this point, SDL is correctly initialized
int launch_sdl_application(int argc, char* argv[])
{
    // Setup OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* main_window = SDL_CreateWindow("game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 760, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(!main_window)
    {
        std::cerr << "failed to create game window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(main_window);
    if(!gl_context)
    {
        SDL_DestroyWindow(main_window);

        std::cerr << "failed to create opengl context: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Make sure launch_window_application can instanciate an application
    ng::application::prepare_for_application(argc, argv);

    const int res = launch_window_application(argc, argv, main_window, gl_context);

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(main_window);

    return res;
}

void handle_sdl_event(ng::application& app, const SDL_Event& event);
void handle_sdl_keymods(ng::application& app, SDL_Keymod key_mod);

// At this point, the function has a valid window and OpenGL context
int launch_window_application(int argc, char* argv[], SDL_Window* window, SDL_GLContext opengl)
{
    ng::application app(argc, argv);

    // We render at 60 fps
    const ng::frame_duration tick_delay{1.0 / 60.0};

    ng::frame_duration previous_duration = tick_delay;

    ng::frame_duration tick_accumulator{};

    ng::frame_clock::time_point start_of_frame_tp = ng::frame_clock::now();

    // Main loop
    while(app.running())
    {
        tick_accumulator += previous_duration;

        // Event loop

        // Handle key modifiers
        handle_sdl_keymods(app, SDL_GetModState());

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            handle_sdl_event(app, event);
        }

        // Update current internal state at 60 FPS
        while(tick_accumulator >= tick_delay)
        {
            app.tick(tick_delay);
            tick_accumulator -= tick_delay;
        }

        // Render current frame
        // TODO: Add interpolation between frames at some point
        app.render();
        SDL_GL_SwapWindow(window);

        // Give back CPU to OS the goal is to minimize interruption in the middle of a frame
        // Find a way to sleep the program to reduce power consumption
        std::this_thread::yield();

        // Measure how much time this frame took
        const ng::frame_clock::time_point end_of_frame_tp = ng::frame_clock::now();
        previous_duration = end_of_frame_tp - start_of_frame_tp;
        start_of_frame_tp = end_of_frame_tp;
    }

    return 0;
}

constexpr ng::mouse_buttons sdl_to_ng_mouse_button(int sdl_button)
{
    switch(sdl_button)
    {
    case SDL_BUTTON_LEFT:
        return ng::mouse_buttons::left;
    case SDL_BUTTON_RIGHT:
        return ng::mouse_buttons::right;
    case SDL_BUTTON_MIDDLE:
        return ng::mouse_buttons::middle;
    case SDL_BUTTON_X1:
        return ng::mouse_buttons::extra_1;
    case SDL_BUTTON_X2:
        return ng::mouse_buttons::extra_2;
    default:
        throw std::runtime_error{"unknown mouse button"};
    }
}

void handle_sdl_event(ng::application& app, const SDL_Event& event)
{
    switch(event.type)
    {
    case SDL_QUIT:
        app.on_quit();
        break;
    case SDL_MOUSEBUTTONDOWN:
        app.on_mouse_pressed(sdl_to_ng_mouse_button(event.button.button), event.button.clicks, event.button.x, event.button.y);
        break;
    case SDL_MOUSEBUTTONUP:
        app.on_mouse_released(sdl_to_ng_mouse_button(event.button.button), event.button.x, event.button.y);
        break;
    case SDL_MOUSEWHEEL:
        app.on_mouse_scrolled(event.wheel.x, event.wheel.y, event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED);
        break;
    case SDL_MOUSEMOTION:
        app.on_mouse_cursor_moved(event.motion.x, event.motion.y);
        app.on_mouse_moved(event.motion.xrel, event.motion.yrel);
        break;
    case SDL_KEYDOWN:
        app.on_key_pressed(ng::keyboard_key{SDL_GetKeyName(event.key.keysym.sym)}, event.key.repeat);
        break;
    case SDL_KEYUP:
        app.on_key_released(ng::keyboard_key{SDL_GetKeyName(event.key.keysym.sym)});
        break;
    // TODO: Handle every events
    default:
        break;
    }
}

void handle_sdl_keymods(ng::application& app, SDL_Keymod key_mod)
{
    app.clear_keymods();

    if((key_mod & KMOD_LALT) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::alt_left);
    }

    if((key_mod & KMOD_RALT) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::alt_right);
    }

    if((key_mod & KMOD_LSHIFT) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::shift_left);
    }

    if((key_mod & KMOD_RSHIFT) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::shift_right);
    }

    if((key_mod & KMOD_LCTRL) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::ctrl_left);
    }

    if((key_mod & KMOD_RCTRL) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::ctrl_right);
    }

    if((key_mod & KMOD_LGUI) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::gui_left);
    }

    if((key_mod & KMOD_RGUI) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::gui_right);
    }

    if((key_mod & KMOD_NUM) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::num);
    }

    if((key_mod & KMOD_CAPS) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::caps);
    }

    if((key_mod & KMOD_MODE) != 0)
    {
        app.enable_keymod(ng::keyboard_modifiers::mode);
    }
}