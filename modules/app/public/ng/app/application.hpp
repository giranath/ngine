#ifndef NGINE_APP_APPLICATION_HPP
#define NGINE_APP_APPLICATION_HPP

#include <ng/core/time.hpp>
#include <cstdint>

namespace ng
{

enum class mouse_buttons : uint8_t;
enum class keyboard_modifiers : uint16_t;
class keyboard_key;

/**
 * Class responsible to hold ngine's application together in a platform agnostic way
 */
class application
{
    uint16_t key_modifiers_;
    bool running_;
public:
    /**
     * Should be called once before an application is instanced
     * @param argc The number of arguments that were passed to the program
     * @param argv The list of arguments that were passed to the program
     * @note This is used to initialize every static variables required for the application to work
     *       In a better world, this would not be required, but at the moment it is
     */
    static void prepare_for_application(int argc, char* argv[]);

    application();
    application(int argc, char* argv[]);

    /**
     * Called every frame by host to give application a chance to periodically update itself
     * @param dt The duration of the previous frame in seconds
     */
    void tick(frame_duration dt);

    /**
     * Called every frame by host to give application the signal to render it's current state onto the screen
     */
    void render();

    /**
     * Check if the application is still running
     * @return true when it is still running or false otherwise
     * @note A host can call this as the condition of the main loop
     */
    [[nodiscard]] bool running() const noexcept;

    /**
     * Called by host when the user is requesting the game to quit
     */
    void on_quit();

    /**
     * Called when the user has clicked with its mouse
     * @param button The button that was clicked
     * @param count The number of time the button was clicked (single click or double clicks)
     * @param x The horizontal location of the cursor
     * @param y The vertical location of the cursor
     */
     void on_mouse_pressed(mouse_buttons button, int count, int x, int y);

     /**
      * Called when the user has released a button on its mouse
      * @param button The button that was released
      * @param x The location of the cursor on X
      * @param y The location of the cursor on Y
      */
     void on_mouse_released(mouse_buttons button, int x, int y);

     /**
      * Called when the mouse cursor has moved
      * @param x The location of the cursor on X
      * @param y The location of the cursor on Y
      * @note Cursor's location is absolute
      */
     void on_mouse_cursor_moved(int x, int y);

     /**
      * Called when the mouse cursor is moved
      * @param x The movement of the mouse on X
      * @param y The movement of the mouse on Y
      * @note The movement is relative
      */
     void on_mouse_moved(int x, int y);

     /**
      * Called when the mouse scroll wheel changes
      * @param delta_x The movement on x
      * @param delta_y The movement on y
      * @param flipped Is the scroll wheel inversed on this platform
      */
     void on_mouse_scrolled(int delta_x, int delta_y, bool flipped);

     /**
      * Called when a key is pressed
      * @param key The key being pressed
      * @param repeat Is this press event from a key repeat
      */
     void on_key_pressed(const keyboard_key& key, bool repeat);

     /**
      * Called when a key is released
      * @param key The key being released
      */
     void on_key_released(const keyboard_key& key);

     /**
      * Clear all key modifiers
      */
     void clear_keymods() noexcept;

     /**
      * Enable a specific key modifier
      * @param modifier The modifier to enable
      */
     void enable_keymod(keyboard_modifiers modifier) noexcept;

     /**
      * Check the state of a key modifier
      * @param modifiers The modifier to check
      * @return true if modifier is enabled
      */
     [[nodiscard]] bool keymod(keyboard_modifiers modifiers) const noexcept;
};

}

#endif
