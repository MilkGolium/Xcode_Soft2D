/*
 * Copyright (C) 2026  [Voosk]
 * * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
//  MyEngine.h
//  Xcode_MyEngine
//
//  Created by Lunar on 2026/1/20.
//

#ifndef MyEngine_h
#define MyEngine_h


#endif /* MyEngine_h */

#include <iostream>

// Platform ==========================================================================
extern int window_width_sdl, window_height_sdl;
extern std::vector<uint32_t> pixels;
void InitWindow(int window_width, int window_height, const char *window_name);
void Rend(void);
bool GameShouldQuit(void);
void CleanUp(void);

// Draw ==============================================================================

// 定义Vector2类型，存储坐标
typedef struct {
    int x, y;
} Vector2;

// 定义Color类型，存储颜色
typedef struct {
    uint8_t r, g, b, a;
} Color;

// 画点函数
void DrawPixel(int x, int y, Color c);
// Bresenham画线函数
void DrawLine(int x1, int y1, int x2, int y2, Color c);
// 实心矩形
void DrawFilledRect(int x, int y, int w, int h, Color c);
// 实心圆
void DrawFilledCircle(int centerX, int centerY, int radius, Color c);
// 空心矩形
void DrawRect(int x, int y, int w, int h, Color c);
// 空心圆
void DrawCircle(int centerX, int centerY, int radius, Color c);

// Time ==============================================================================
void InitEngineTime(void); // 在主循环开始前调用一次，初始化时间基准
void UpdateDeltaTime(void); // 计算出当前帧用了多久
float GetDeltaTime(void); // 获取上一帧的时间
void SetTargetFPS(int fps); // 设置帧率
// 你也可以开垂直同步

// 彩色清屏（准确说叫颜色填充）
void FillScreen(Color c);
// 速度更快的memset清屏，只支持黑色，数组全填充成0
void ClearScreen(void);
