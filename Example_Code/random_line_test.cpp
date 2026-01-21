//
//  random_line_test.cpp
//  Xcode_MyEngine
//
//  Created by Lunar on 2026/1/20.
//

#include "Soft2D.h"
#include <cstdlib>
#include <ctime>

int main(void) {
    InitWindow(640, 480, "Window name");
    InitEngineTime();
    srand((unsigned int)time(NULL)); // 初始化随机种子
    
    while (GameShouldQuit()) {
        UpdateDeltaTime();
        float dt = GetDeltaTime();
        if (dt > 0) {
            std::cout << "DT: " << dt << " s | FPS: " << (int)(1.0f / dt) << std::endl;
        }
        ClearScreen();
        for (int i = 0; i < 100; i++) {
            // 生成随机坐标
            int x1 = rand() % 640;
            int y1 = rand() % 480;
            int x2 = rand() % 640;
            int y2 = rand() % 480;
            
            // 生成随机颜色
            Color randomColor = {
                (uint8_t)(rand() % 256), // R
                (uint8_t)(rand() % 256), // G
                (uint8_t)(rand() % 256), // B
                255                      // A (不透明)
            };
            DrawLine(x1, y1, x2, y2, randomColor);
        }
        Rend();
    }
    
    CleanUp();
    return 0;
}
