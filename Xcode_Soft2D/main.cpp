#include "Soft2D.h"

int main(void) {
    InitWindow(640, 480, "Window name");
    InitEngineTime();
    
    while (GameShouldQuit()) {
        UpdateDeltaTime();
        // Draw here
        ClearScreen();
        Rend();
    }
    CleanUp();
    return 0;
}
