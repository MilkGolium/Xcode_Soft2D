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

#include <iostream> // 用于显示报错信息
#include <SDL3/SDL.h>
#include "Soft2D.h"
// #include <MyEngine.h>
//#include <vector> // 为什么把它注释掉也能用Vector数组？

SDL_Event event; // SDL状态
int window_width_sdl, window_height_sdl;
std::vector<uint32_t> pixels; // 声明空数组，未来作为内存数组 (vector代替原始数组更安全)
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

// 初始化窗口，
/*
 使用到的变量：
 window_width_sdl,
 window_height_sdl,
 window,
 renderer,
 texture,
 pixels
*/
void InitWindow(int window_width, int window_height, const char *window_name){
    window_width_sdl = window_width;
    window_height_sdl = window_height;
    if (window_width <= 0 || window_height <= 0) {
        std::cout << "错误：窗口宽度或者高度不能小于等于0！" << std::endl;
        return;
    }
    SDL_Init(SDL_INIT_VIDEO); // 初始化 SDL
    // 创建窗口、创建渲染器
    window = SDL_CreateWindow(window_name, window_width, window_height, 0);
    renderer = SDL_CreateRenderer(window, NULL); // 声明指针指向渲染状态管理器

    // 创建一个纹理 (格式设为 RGBA8888)
    //texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height); // SDL_TEXTUREACCESS_STREAMING 表示这块内存会频繁更新
    // macOS 是小端序机器
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    
    pixels.resize(window_width * window_height, 0); // 把空数组改成内存数组，window_width * window_height 像素 * 4 字节/像素
    
    
}

// 渲染（一帧结束的时候使用）
void Rend(void){
    // 将内存数据上传到显卡的纹理中，Pitch 是每一行数据的字节数 (window_width_sdl * 4)
    SDL_UpdateTexture(texture, NULL, pixels.data(), window_width_sdl * sizeof(uint32_t));

    // 渲染到屏幕
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// 处理退出游戏主循环的条件
bool GameShouldQuit(void){
    // 处理退出请求
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) return false;
    }
    return true;
}

// 退出主循环以后释放资源
void CleanUp(void){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
