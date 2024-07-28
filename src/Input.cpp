#include "Input.hpp"

#include <SDL_events.h>

Input::Input() {
    for (int i = 0; i < 256; i++) {
        m_Keys[i] = KS_UP;
    }
}

void Input::keyCallback(int key, int mods, int state) {
    if (state == SDL_KEYUP) {
        if (m_Keys[key] == KS_DOWN || m_Keys[key] == KS_PRESSED) {
            m_Keys[key] = KS_RELEASED;
        } else if (m_Keys[key] == KS_RELEASED) {
            m_Keys[key] = KS_UP;
        }
    } else if (state == SDL_KEYDOWN) {
        if (m_Keys[key] == KS_UP || m_Keys[key] == KS_RELEASED) {
            m_Keys[key] = KS_PRESSED;
        } else if (m_Keys[key] == KS_PRESSED) {
            m_Keys[key] = KS_DOWN;
        }
    }
}

void Input::update() {
    for (int i = 0; i < 256; i++) {
        if (m_Keys[i] == KS_PRESSED) {
            m_Keys[i] = KS_DOWN;
        } else if (m_Keys[i] == KS_RELEASED) {
            m_Keys[i] = KS_UP;
        }
    }
}

bool Input::isKeyDown(int key) const {
    return m_Keys[key] == KS_DOWN;
}

bool Input::isKeyUp(int key) const {
    return m_Keys[key] == KS_UP;
}

bool Input::isKeyPressed(int key) const {
    return m_Keys[key] == KS_PRESSED;
}

bool Input::isKeyReleased(int key) const {
    return m_Keys[key] == KS_RELEASED;
}
