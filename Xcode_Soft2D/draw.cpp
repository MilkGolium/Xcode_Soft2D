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

#include <iostream>
#include <SDL3/SDL.h>
#include "Soft2D.h"

// 画点函数
// 注意：Color类型已经在头文件里面定义，不在这个文件中。
void DrawPixel(int x, int y, Color c) {
    // 边界检查
    if (x < 0 || x >= window_width_sdl || y < 0 || y >= window_height_sdl) {
        return;
    }
    // 将 Color 结构体打包并写入，这种写法对于 0xFF0000FF 这种 RGBA 格式非常直接
    pixels[y * window_width_sdl + x] = ((uint32_t)c.r << 24) | ((uint32_t)c.g << 16) | ((uint32_t)c.b << 8) | (uint32_t)c.a;
}

// Vector2版 画点函数
// 注意：Vector2 类型已经在头文件中定义，不在此文件中。
void DrawPixelV(Vector2 pos, Color c) {
    // 边界检查
    if (pos.x < 0 || pos.x >= window_width_sdl || pos.y < 0 || pos.y >= window_height_sdl) {
        return;
    }
    // 将 Color 结构体打包并写入，这种写法对于 0xFF0000FF 这种 RGBA 格式非常直接
    pixels[pos.y * window_width_sdl + pos.x] = ((uint32_t)c.r << 24) | ((uint32_t)c.g << 16) | ((uint32_t)c.b << 8) | (uint32_t)c.a;
}

// 画水平线：因为内存连续，所以是效率之王。绘制实心填充图形使用这个。
void DrawHLine(int x1, int x2, int y, Color c) {
    if (y < 0 || y >= window_height_sdl) return;
    
    // 确保 x1 是左边的点
    if (x1 > x2) { int temp = x1; x1 = x2; x2 = temp; }
    
    // 裁剪左右边界
    if (x1 < 0) x1 = 0;
    if (x2 >= window_width_sdl) x2 = window_width_sdl - 1;

    uint32_t packedColor = ((uint32_t)c.r << 24) | ((uint32_t)c.g << 16) | ((uint32_t)c.b << 8) | c.a;
    
    // 使用 std::fill 填充这一行连续的像素内存
    std::fill(pixels.begin() + (y * window_width_sdl + x1),
              pixels.begin() + (y * window_width_sdl + x2 + 1),
              packedColor);
}

// 画垂直线：内存不连续，但索引计算非常简单
void DrawVLine(int x, int y1, int y2, Color c) {
    if (x < 0 || x >= window_width_sdl) return;
    if (y1 > y2) { int temp = y1; y1 = y2; y2 = temp; }
    
    if (y1 < 0) y1 = 0;
    if (y2 >= window_height_sdl) y2 = window_height_sdl - 1;

    uint32_t packedColor = ((uint32_t)c.r << 24) | ((uint32_t)c.g << 16) | ((uint32_t)c.b << 8) | c.a;
    
    for (int y = y1; y <= y2; y++) {
        pixels[y * window_width_sdl + x] = packedColor;
    }
}

// Bresenham画任意线
void DrawLine(int x1, int y1, int x2, int y2, Color c) {
    int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        DrawPixel(x1, y1, c);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

// Vector2 Bresenham画任意线
;

// 实心矩形
void DrawFilledRect(int x, int y, int w, int h, Color c) {
    for (int i = 0; i < h; i++) {
        DrawHLine(x, x + w - 1, y + i, c);
    }
}

// Vector2 实心矩形

// 实心圆
void DrawFilledCircle(int centerX, int centerY, int radius, Color c) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    auto drawSymLines = [&](int cx, int cy, int x, int y, Color c) {
        DrawHLine(cx - x, cx + x, cy + y, c); // 下部水平线
        DrawHLine(cx - x, cx + x, cy - y, c); // 上部水平线
        DrawHLine(cx - y, cx + y, cy + x, c); // 中下部水平线
        DrawHLine(cx - y, cx + y, cy - x, c); // 中上部水平线
    };

    while (y >= x) {
        drawSymLines(centerX, centerY, x, y, c);
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

// Vector2 实心圆
;

// 空心矩形
void DrawRect(int x, int y, int w, int h, Color c) {
    if (w <= 0 || h <= 0) return; // 宽度或高度为0或负数则不画

    // 绘制上下两条水平线
    DrawHLine(x, x + w - 1, y, c);           // 上边
    DrawHLine(x, x + w - 1, y + h - 1, c);   // 下边

    // 绘制左右两条垂直线 (注意：y的范围需要避开已经画过的角点，或者让DrawVLine处理重叠)
    // 为了简单且不重复画角点，我们让垂直线从y+1到y+h-2
    DrawVLine(x, y + 1, y + h - 2, c);       // 左边
    DrawVLine(x + w - 1, y + 1, y + h - 2, c); // 右边
}

// Vector2 空心矩形
;

// 空心圆
void DrawCircle(int centerX, int centerY, int radius, Color c) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius; // 初始决策参数

    // 辅助函数，在八个对称位置画点
    auto drawCirclePoints = [&](int cx, int cy, int p_x, int p_y, Color color) {
        DrawPixel(cx + p_x, cy + p_y, color);
        DrawPixel(cx - p_x, cy + p_y, color);
        DrawPixel(cx + p_x, cy - p_y, color);
        DrawPixel(cx - p_x, cy - p_y, color);
        DrawPixel(cx + p_y, cy + p_x, color);
        DrawPixel(cx - p_y, cy + p_x, color);
        DrawPixel(cx + p_y, cy - p_x, color);
        DrawPixel(cx - p_y, cy - p_x, color);
    };

    while (y >= x) {
        drawCirclePoints(centerX, centerY, x, y, c);
        
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        x++; // x 每次迭代都增加
    }
}

// Vector2 空心圆
;

// 彩色清屏
void FillScreen(Color c) {
    // 将 Color 结构体打包成一个 32 位整数
    uint32_t packedColor = ((uint32_t)c.r << 24) | ((uint32_t)c.g << 16) | ((uint32_t)c.b << 8) | (uint32_t)c.a;
    // 使用 std::fill 填充整个 vector
    std::fill(pixels.begin(), pixels.end(), packedColor);
}

// 速度最快的memset清屏，只能是黑色
#include <cstring> // 必须包含这个头文件

void ClearScreen(void) {
    // 0 表示所有位都是 0，即 RGBA(0, 0, 0, 0)
    // 注意：这里的长度是字节数，所以要乘以 sizeof(uint32_t)
    memset(pixels.data(), 0, pixels.size() * sizeof(uint32_t));
}
