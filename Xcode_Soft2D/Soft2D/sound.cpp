//
//  sound.cpp
//  Xcode_Soft2D
//
//  Created by Lunar on 2026/1/22.
//

#include <SDL3/SDL.h>
#include <stdlib.h>

static SDL_AudioStream* bgm_stream = NULL;

// 初始化音频系统
bool InitSound() {
    // SDL3 只要初始化了 Video，Audio 也可以一起工作
    // 或者显式调用：SDL_Init(SDL_INIT_AUDIO);
    
    // 创建一个音频流：通常使用常见的 44100Hz, 立体声, 16位格式
    SDL_AudioSpec spec = { SDL_AUDIO_S16, 2, 44100 };
    bgm_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_ID, &spec, NULL, NULL);
    
    if (!bgm_stream) {
        SDL_Log("音频初始化失败: %s", SDL_GetError());
        return false;
    }

    // 开启播放
    SDL_ResumeAudioStreamDevice(bgm_stream);
    return true;
}

// 播放一个 WAV 文件
void PlayWAV(const char* filename) {
    SDL_AudioSpec spec;
    uint8_t *buffer;
    uint32_t len;

    // SDL 内置了加载 WAV 的函数
    if (SDL_LoadWAV(filename, &spec, &buffer, &len)) {
        // 将 WAV 数据直接推入流中
        SDL_PutAudioStreamData(bgm_stream, buffer, len);
        // 记得释放 buffer
        SDL_free(buffer);
    }
}

void PlayBeep(float frequency, float duration_sec) {
    int sample_rate = 44100;
    int total_samples = (int)(sample_rate * duration_sec);
    int16_t* buffer = (int16_t*)malloc(total_samples * sizeof(int16_t));

    for (int i = 0; i < total_samples; i++) {
        // 生成正弦波: sin(2 * PI * f * t)
        float time = (float)i / sample_rate;
        buffer[i] = (int16_t)(32767 * sinf(2.0f * M_PI * frequency * time));
    }

    SDL_PutAudioStreamData(bgm_stream, buffer, total_samples * sizeof(int16_t));
    free(buffer);
}
