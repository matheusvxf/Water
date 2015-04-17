#pragma once

class Mouse
{
public:
    Mouse();
    ~Mouse();

    static void leftMotion(int x, int y);
    static void rightMotion(int x, int y);
    static void middleMotion(int x, int y);
    static void mouseCallback(int mouse, int state, int x, int y);
};

