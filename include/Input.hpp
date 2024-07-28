enum KeyState {
    KS_UP,
    KS_DOWN,
    KS_RELEASED,
    KS_PRESSED,
};

class Input {
    public:
        Input();

        void keyCallback(int key, int mods, int state);
        void update();

        bool isKeyDown(int key) const;
        bool isKeyUp(int key) const;
        bool isKeyPressed(int key) const;
        bool isKeyReleased(int key) const;

    private:
        KeyState m_Keys[256];
};
