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


/*
 这个方案确定可以吗？程序中每次获得的帧时间都是上一帧的帧时间啊，但是好像也只能这样，因为这一帧又没完成，你怎么知道这一帧会是多长时间呢
 我们永远无法预测“未来”这一帧需要跑多久（可能显卡突然卡了一下，或者后台有个程序抢了 CPU）。
 在游戏开发中，这被称为 “上一帧补偿法”。 我们假设：如果电脑上一帧跑了 16ms，那么下一帧大概率也会跑 16ms 左右。
 游戏界有两种方法：
 1. 固定时间步长 (Fixed Timestep) —— 最稳定
 2. 垂直同步 (VSync) —— 最省心
 
 // 在 UpdateDeltaTime 中加入平滑滤波
 static float smoothed_dt = 0.0f;
 float current_dt = (float)diff / 1000000000.0f;

 // 简单的低通滤波：90% 使用旧值，10% 使用新值，让时间变化不那么剧烈
 smoothed_dt = smoothed_dt * 0.9f + current_dt * 0.1f;

 // 或者简单的硬上限限制
 if (current_dt > 0.05f) current_dt = 0.05f; // 就算卡了，也按最高 20FPS 算，防止物体飞掉
*/

#include <SDL3/SDL.h>
#include "Soft2D.h"

// 静态全局变量，仅限本文件访问
static uint64_t last_now = 0;
static float delta_time = 0.0f;

// 在主循环开始前调用一次，初始化时间基准。注意，不要在主循环里面使用，否则delta time会接近0
void InitEngineTime(void) {
    last_now = SDL_GetTicksNS();
    delta_time = 0.0f;
}

// 每一帧的开始执行，以防刚进入主循环的时候delta time是0
void UpdateDeltaTime(void) {
    uint64_t now = SDL_GetTicksNS();
    // 计算纳秒差值，并转换为秒 (1秒 = 1,000,000,000 纳秒)
    uint64_t diff = now - last_now;
    
    // 转换为秒 (float)，方便后续物理计算
    delta_time = (float)diff / 1000000000.0f;
    
    // 更新上一帧时间点
    last_now = now;

    // 保护：防止 delta_time 过大（例如调试断点或窗口拖动时导致的跳跃）
    if (delta_time > 0.1f) delta_time = 0.1f;
}

// 供外部获取当前的 DeltaTime
float GetDeltaTime(void) {
    return delta_time;
}

void SetTargetFPS(int fps) {
    const uint64_t target_ns = 1000000000 / fps;
    uint64_t elapsed = SDL_GetTicksNS() - last_now;
    if (elapsed < target_ns) {
        SDL_DelayNS(target_ns - elapsed);
    }
}
