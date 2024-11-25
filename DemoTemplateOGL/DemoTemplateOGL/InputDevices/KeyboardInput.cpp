#include "KeyboardInput.h"
using namespace std;
using namespace glm;

// Constructor for MouseInput class
MouseInput::MouseInput() {
    prevP.x = 0;
    prevP.y = 0;
    currP.x = 0;
    currP.y = 0;
    delta.x = 0;
    delta.y = 0;
    lbtn = false;
    rbtn = false;
    mouseWheel = 0;
}

// Getters for mouse delta and wheel state
float MouseInput::getDY() const { return delta.y; }
float MouseInput::getDX() const { return delta.x; }
char MouseInput::getMouseWheel() const { return mouseWheel; }

// Setters for mouse buttons and wheel state
void MouseInput::setMouseWheel(char x) { mouseWheel = x; }
bool MouseInput::getLbtn() const { return lbtn; }
bool MouseInput::getRbtn() const { return rbtn; }
void MouseInput::setLbtn(bool v) { lbtn = v; }
void MouseInput::setRbtn(bool v) { rbtn = v; }

// Get mouse movement delta
vec2 MouseInput::getDelta() const { return delta; }

// Set the current mouse position and calculate delta if needed
vec2 MouseInput::setPosition(double x, double y, bool isDelta) {
    prevP = currP;
    currP.x = x;
    currP.y = y;

    // Reset delta if not using delta-based movement
    if (!isDelta) {
        prevP = currP;
        delta.x = 0;
        delta.y = 0;
    }
    else {
        delta.x = prevP.x - currP.x;
        delta.y = prevP.y - currP.y;
    }

    return delta;
}

vec2 MouseInput::setPosition(glm::vec2 d, bool isDelta) {
    return setPosition(d.x, d.y, isDelta);
}

// External variables
extern struct Input input;
extern array<bool, 256> KEYS;  // Using std::array for better type safety
extern MouseInput cDelta;           // Instance of MouseInput class

// Initializes the keys array
void Init() {
    KEYS.fill(false);  // Resets the key states to false
}

// Handle key events
bool KeysEvents(GameActions* actions) {
    bool checkCollition = false;

    // Handle key press actions with proper casting
    if (KEYS[static_cast<size_t>(input.P)]) {
        actions->firstPerson = true;
        KEYS[static_cast<size_t>(input.P)] = false;
    }

    if (KEYS[static_cast<size_t>(input.D)]) {
        if (KEYS[KEYB_HMOVEMENT])
            actions->hAdvance = -1;
        else
            actions->sideAdvance = -1;
        KEYS[static_cast<size_t>(input.D)] = false;
        checkCollition = true;
    }

    if (KEYS[static_cast<size_t>(input.A)]) {
        if (KEYS[KEYB_HMOVEMENT])
            actions->hAdvance = 1;
        else
            actions->sideAdvance = 1;
        KEYS[static_cast<size_t>(input.A)] = false;
        checkCollition = true;
    }

    if (KEYS[static_cast<size_t>(input.W)]) {
        actions->advance = 1;
        KEYS[static_cast<size_t>(input.W)] = false;
        checkCollition = true;
    }

    if (KEYS[static_cast<size_t>(input.S)]) {
        actions->advance = -1;
        KEYS[static_cast<size_t>(input.S)] = false;
        checkCollition = true;
    }

    // Mouse button and movement handling
    if (cDelta.getLbtn() && cDelta.getDX() != 0) {
        actions->setAngle(cDelta.getDX() > 0 ? 1 : -1);
    }

    if (cDelta.getRbtn() && cDelta.getDY() != 0) { //KEYS[KEYB_CAMERA]
        actions->setPitch(cDelta.getDY() > 0 ? 1 : -1);
    }

    if ((!KEYS[KEYB_CAMERA]) && cDelta.getMouseWheel() != 0) {
        actions->setPlayerZoom(cDelta.getMouseWheel() > 0 ? 1 : -1);
    }

    if (KEYS[KEYB_CAMERA] && cDelta.getMouseWheel() != 0) {
        actions->setZoom(cDelta.getMouseWheel() > 0 ? 1 : -1);
    }

    cDelta.setMouseWheel(0);
    return checkCollition;
}


