#ifndef NGINE_APP_KEYBOARD_HPP
#define NGINE_APP_KEYBOARD_HPP

#include <ng/core/hash.hpp>

#include <cstdint>
#include <string>
#include <string_view>
#include <cassert>
#include <algorithm>

namespace ng
{

/**
 * Represents a keyboard key
 * @note Name of key is platform specific, it must be taken into consideration if you need to represent a platform agnostic
 *       key. Can safely be used to compare keys inside a single platform. This limitation will need to be considered
 *       when we want to support key remapping especially when we set the default key mapping for the game.
 *       My initial idea is to use scancode for the initial keymapping and allow user to rebind them using local key names
 */
class keyboard_key
{
public:
    static constexpr std::size_t max_name_length = 24;
private:
    char name_[max_name_length];
    uint64_t hash_;

public:


    /**
     * Construct an empty key
     */
    constexpr keyboard_key() noexcept
    : name_{0}
    , hash_{0}
    {

    }

    /**
     * Construct a key from it's name
     * @param name The name of the key
     */
    constexpr explicit keyboard_key(std::string_view name) noexcept
    : keyboard_key{}
    {
        assert(name.size() < max_name_length);
        if(name.size() < max_name_length)
        {
            std::copy(name.begin(), name.end(), name_);
            // Make sure to store 0 at the end of the name
            name_[name.size()] = 0;
        }
        // Because keyboard handling will still work when the key name is too long for internal storage,
        // except being wrongly displayed if we access the name of the key, 24 bytes to identify the key is enough
        // so we gracefuly handle the case where an unknown key name is bigger than what we expected
        else
        {
            std::copy(name.begin(), std::next(name.begin(), max_name_length - 1), name_);
            // Make sure to store 0 at the end of the name
            name_[max_name_length - 1] = 0;
        }

        hash_ = ng::hash(name_);
    }

    /**
     * Check if this key is identical to another key
     * @param other The other key to compare
     * @return true when both keys are identical
     */
    constexpr bool operator==(const keyboard_key& other) const noexcept
    {
        return hash_ == other.hash_
            && std::strcmp(name_, other.name_) == 0;
    }

    /**
     * Check if this key is different from another key
     * @param other The other key
     * @return true when both keys are different or false when they are identical
     */
    constexpr bool operator!=(const keyboard_key& other) const noexcept
    {
        return hash_ != other.hash_;
    }

    /**
     * Returns the name of the key
     * @return The name of the key
     */
    [[nodiscard]] std::string name() const noexcept;

    /**
     * Returns a string view of the name of the key
     * @return The name of the key as a string view
     */
    [[nodiscard]] constexpr std::string_view keyboard_key::name_view() const noexcept
    {
        return name_;
    }

    /**
     * Returns a pointer to the raw name storage
     * @return The pointer to the raw name storage
     */
    [[nodiscard]] constexpr const char* keyboard_key::data() const noexcept
    {
        return name_;
    }

    // Arrow keys
    static const keyboard_key left;
    static const keyboard_key right;
    static const keyboard_key up;
    static const keyboard_key down;

    // Alpha/num keys
    static const keyboard_key a;
    static const keyboard_key b;
    static const keyboard_key c;
    static const keyboard_key d;
    static const keyboard_key e;
    static const keyboard_key f;
    static const keyboard_key g;
    static const keyboard_key h;
    static const keyboard_key i;
    static const keyboard_key j;
    static const keyboard_key k;
    static const keyboard_key l;
    static const keyboard_key m;
    static const keyboard_key n;
    static const keyboard_key o;
    static const keyboard_key p;
    static const keyboard_key q;
    static const keyboard_key r;
    static const keyboard_key s;
    static const keyboard_key t;
    static const keyboard_key u;
    static const keyboard_key v;
    static const keyboard_key w;
    static const keyboard_key x;
    static const keyboard_key y;
    static const keyboard_key z;
    static const keyboard_key space;
    static const keyboard_key enter;
    static const keyboard_key backspace;
    static const keyboard_key tab;
    static const keyboard_key num1;
    static const keyboard_key num2;
    static const keyboard_key num3;
    static const keyboard_key num4;
    static const keyboard_key num5;
    static const keyboard_key num6;
    static const keyboard_key num7;
    static const keyboard_key num8;
    static const keyboard_key num9;
    static const keyboard_key num0;
    static const keyboard_key hash;
    static const keyboard_key minus;
    static const keyboard_key equal;

    // Function keys
    static const keyboard_key f1;
    static const keyboard_key f2;
    static const keyboard_key f3;
    static const keyboard_key f4;
    static const keyboard_key f5;
    static const keyboard_key f6;
    static const keyboard_key f7;
    static const keyboard_key f8;
    static const keyboard_key f9;
    static const keyboard_key f10;
    static const keyboard_key f11;
    static const keyboard_key f12;
    static const keyboard_key escape;
    static const keyboard_key caps_lock;

    // Modifier keys
    static const keyboard_key shift_left;
    static const keyboard_key shift_right;
    static const keyboard_key alt_left;
    static const keyboard_key alt_right;
    static const keyboard_key ctrl_left;
    static const keyboard_key ctrl_right;
    static const keyboard_key system_left;
    static const keyboard_key system_right;
    static const keyboard_key menu;
};

/**
 * Enumerates all supported key modifiers
 */
enum class keyboard_modifiers : uint16_t
{
    none = 0,
    shift_left = 1,
    shift_right = 2,
    ctrl_left = 4,
    ctrl_right = 8,
    alt_left = 16,
    alt_right = 32,
    gui_left = 64,
    gui_right = 128,
    num = 256,
    caps = 512,
    mode = 1024,
    ctrl = ctrl_left | ctrl_right,
    alt = alt_left | alt_right,
    gui = gui_left | gui_right,
    shift = shift_left | shift_right
};



}

#endif
