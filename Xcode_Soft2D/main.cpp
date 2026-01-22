#include "Soft2D.h"

int main(void) {
    InitWindow(640, 480, "Window name");
    InitEngineTime();
    Vector2 pos = {50, 100};
    while (GameShouldQuit()) {
        UpdateDeltaTime();
        // Draw here
        Draw
        ClearScreen();
        Rend();
    }
    CleanUp();
    return 0;
}
