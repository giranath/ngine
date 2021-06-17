#include "keyboard.hpp"

namespace ng
{

std::string keyboard_key::name() const noexcept
{
    return name_;
}

const keyboard_key keyboard_key::a{"A"};
const keyboard_key keyboard_key::b{"B"};
const keyboard_key keyboard_key::c{"C"};
const keyboard_key keyboard_key::d{"D"};
const keyboard_key keyboard_key::e{"E"};
const keyboard_key keyboard_key::f{"F"};
const keyboard_key keyboard_key::g{"G"};
const keyboard_key keyboard_key::h{"H"};
const keyboard_key keyboard_key::i{"I"};
const keyboard_key keyboard_key::j{"J"};
const keyboard_key keyboard_key::k{"K"};
const keyboard_key keyboard_key::l{"L"};
const keyboard_key keyboard_key::m{"M"};
const keyboard_key keyboard_key::n{"N"};
const keyboard_key keyboard_key::o{"O"};
const keyboard_key keyboard_key::p{"P"};
const keyboard_key keyboard_key::q{"Q"};
const keyboard_key keyboard_key::r{"R"};
const keyboard_key keyboard_key::s{"S"};
const keyboard_key keyboard_key::t{"T"};
const keyboard_key keyboard_key::u{"U"};
const keyboard_key keyboard_key::v{"V"};
const keyboard_key keyboard_key::w{"W"};
const keyboard_key keyboard_key::x{"X"};
const keyboard_key keyboard_key::y{"Y"};
const keyboard_key keyboard_key::z{"Z"};
const keyboard_key keyboard_key::space{"Space"};
const keyboard_key keyboard_key::enter{"Return"};
const keyboard_key keyboard_key::backspace{"Backspace"};
const keyboard_key keyboard_key::tab{"Tab"};
const keyboard_key keyboard_key::num1{"1"};
const keyboard_key keyboard_key::num2{"2"};
const keyboard_key keyboard_key::num3{"3"};
const keyboard_key keyboard_key::num4{"4"};
const keyboard_key keyboard_key::num5{"5"};
const keyboard_key keyboard_key::num6{"6"};
const keyboard_key keyboard_key::num7{"7"};
const keyboard_key keyboard_key::num8{"8"};
const keyboard_key keyboard_key::num9{"9"};
const keyboard_key keyboard_key::num0{"0"};
const keyboard_key keyboard_key::hash{"#"};
const keyboard_key keyboard_key::minus{"-"};
const keyboard_key keyboard_key::equal{"="};

const keyboard_key keyboard_key::f1{"F1"};
const keyboard_key keyboard_key::f2{"F2"};
const keyboard_key keyboard_key::f3{"F3"};
const keyboard_key keyboard_key::f4{"F4"};
const keyboard_key keyboard_key::f5{"F5"};
const keyboard_key keyboard_key::f6{"F6"};
const keyboard_key keyboard_key::f7{"F7"};
const keyboard_key keyboard_key::f8{"F8"};
const keyboard_key keyboard_key::f9{"F9"};
const keyboard_key keyboard_key::f10{"F10"};
const keyboard_key keyboard_key::f11{"F11"};
const keyboard_key keyboard_key::f12{"F12"};
const keyboard_key keyboard_key::escape{"Escape"};
const keyboard_key keyboard_key::caps_lock{"CapsLock"};

const keyboard_key keyboard_key::shift_left{"Left Shift"};
const keyboard_key keyboard_key::shift_right{"Right Shift"};
const keyboard_key keyboard_key::alt_left{"Left Alt"};
const keyboard_key keyboard_key::alt_right{"Right Alt"};
const keyboard_key keyboard_key::ctrl_left{"Left Ctrl"};
const keyboard_key keyboard_key::ctrl_right{"Right Ctrl"};
const keyboard_key keyboard_key::system_left{"Left Windows"};
const keyboard_key keyboard_key::system_right{"Right Windows"};
const keyboard_key keyboard_key::menu{"Menu"};

}