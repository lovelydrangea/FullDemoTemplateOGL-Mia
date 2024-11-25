#pragma once
#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include "../Base/Utilities.h"

// Define the Input struct with key codes
struct Input {
    enum Keys : unsigned int {  // Usar unsigned int como tipo subyacente
        Backspace = 0x08, Tab,
        Clear = 0x0C, Enter,
        Shift = 0x10, Control, Alt,
        Escape = 0x1B,
        Space = 0x20, PageUp, PageDown, End, Home, Left, Up, Right, Down,
        Zero = 0x30, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
        A = 0x41, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        NumPad0 = 0x60, NumPad1, NumPad2, NumPad3, NumPad4, NumPad5, NumPad6, NumPad7, NumPad8, NumPad9,
        F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
    };
};


// Define key mappings for convenience
#define KEYB_CAMERA input.V
#define KEYB_HMOVEMENT input.Shift

// MouseInput class definition
class MouseInput {
private:
    glm::vec2 prevP;       // Previous position of the mouse
    glm::vec2 currP;       // Current position of the mouse
    glm::vec2 delta;       // Mouse movement (delta)
    char mouseWheel;       // Mouse wheel movement
    bool lbtn;             // Left mouse button state
    bool rbtn;             // Right mouse button state

public:
    MouseInput();

    // Getters and setters
    float getDY() const;    // Get Y movement (delta)
    float getDX() const;    // Get X movement (delta)
    glm::vec2 getDelta() const;  // Get delta of mouse movement

    glm::vec2 setPosition(double x, double y, bool isDelta = false);  // Set mouse position
    glm::vec2 setPosition(glm::vec2 d, bool isDelta = false);  // Set mouse position from glm::vec2

    bool getLbtn() const;      // Get state of the left mouse button
    bool getRbtn() const;      // Get state of the right mouse button
    void setLbtn(bool v);      // Set state of the left mouse button
    void setRbtn(bool v);      // Set state of the right mouse button

    char getMouseWheel() const; // Get mouse wheel state
    void setMouseWheel(char x); // Set mouse wheel state
};

// External variables and function declarations
extern struct Input input;
extern std::array<bool, 256> KEYS;  // Using std::array for type safety
extern bool KeysEvents(GameActions* obj);  // Key event handling function
extern void Init();  // Initialization function for keys
extern MouseInput cDelta;  // Mouse input instance

#endif // KEYBOARD_INPUT_H
