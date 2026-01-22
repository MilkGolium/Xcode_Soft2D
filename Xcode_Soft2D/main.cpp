#include "Soft2D.h"

int main(void) {
    InitWindow(640, 480, "Window name");
    InitEngineTime();
    Vector2 pos = {50, 100};
    while (GameShouldRunning()) {
        UpdateDeltaTime();
        ClearScreen();
        // Draw here
        DrawCircleV(pos, 30, {255, 255, 255, 255});
        RenderFrame();
    }
    CleanUp();
    return 0;
}
