#include "MyEngine.h"

int main(void) {
    InitWindow(640, 480, "Hello");
    InitEngineTime(); // 初始化计数器
    
    while (GameShouldQuit()) {
        UpdateDeltaTime(); // 更新帧时间
        Rend();
    }
    CleanUp();
    return 0;
}
