/*
 * AGC Wrapper for Go CGO
 * 提供简单的 C 接口供 Go 语言调用
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "agc.h"

#ifndef MIN
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#endif

// AGC 配置结构体（简化版，供 Go 使用）
typedef struct {
    int16_t compressionGaindB;  // 压缩增益 (dB)
    int16_t targetLevelDbfs;    // 目标电平 (0-31)
    uint8_t limiterEnable;      // 限幅器开关 (0/1)
    int16_t agcMode;            // 增益模式
} AgcConfig;

// 创建 AGC 实例
// 返回: AGC 实例指针，失败返回 NULL
void* AgcCreate() {
    return WebRtcAgc_Create();
}

// 初始化 AGC
// agcInst: AGC 实例
// minLevel: 最小电平 (通常 0)
// maxLevel: 最大电平 (通常 255)
// agcMode: 增益模式 (0-3)
// sampleRate: 采样率 (8000, 16000, 32000, 48000)
// 返回: 0 成功, -1 失败
int AgcInit(void* agcInst, int32_t minLevel, int32_t maxLevel, 
            int16_t agcMode, uint32_t sampleRate) {
    if (agcInst == NULL) return -1;
    return WebRtcAgc_Init(agcInst, minLevel, maxLevel, agcMode, sampleRate);
}

// 设置 AGC 配置
// agcInst: AGC 实例
// config: AGC 配置
// 返回: 0 成功, -1 失败
int AgcSetConfig(void* agcInst, AgcConfig* config) {
    if (agcInst == NULL || config == NULL) return -1;
    
    WebRtcAgcConfig agcConfig;
    agcConfig.compressionGaindB = config->compressionGaindB;
    agcConfig.targetLevelDbfs = config->targetLevelDbfs;
    agcConfig.limiterEnable = config->limiterEnable;
    
    return WebRtcAgc_set_config(agcInst, agcConfig);
}

// 处理音频数据
// agcInst: AGC 实例
// samples: 输入/输出音频数据 (16-bit PCM)
// numSamples: 采样点数
// sampleRate: 采样率
// 返回: 0 成功, -1 失败
int AgcProcess(void* agcInst, int16_t* samples, size_t numSamples, uint32_t sampleRate) {
    if (agcInst == NULL || samples == NULL || numSamples == 0) return -1;
    
    size_t samplesPerFrame = MIN(160, sampleRate / 100);
    if (samplesPerFrame == 0) return -1;
    
    const int maxSamples = 320;
    int16_t* input = samples;
    size_t nTotal = numSamples / samplesPerFrame;
    size_t num_bands = 1;
    int inMicLevel = 0;
    int outMicLevel = -1;
    int16_t out_buffer[maxSamples];
    int16_t* out16 = out_buffer;
    uint8_t saturationWarning = 1;
    int16_t echo = 0;
    
    // 处理完整帧
    for (size_t i = 0; i < nTotal; i++) {
        inMicLevel = 0;
        int nAgcRet = WebRtcAgc_Process(agcInst, 
                                        (const int16_t *const *) &input, 
                                        num_bands, 
                                        samplesPerFrame,
                                        (int16_t *const *) &out16, 
                                        inMicLevel, 
                                        &outMicLevel, 
                                        echo,
                                        &saturationWarning);
        
        if (nAgcRet != 0) {
            return -1;
        }
        
        memcpy(input, out_buffer, samplesPerFrame * sizeof(int16_t));
        input += samplesPerFrame;
    }
    
    // 处理剩余采样点
    size_t remainedSamples = numSamples - nTotal * samplesPerFrame;
    if (remainedSamples > 0) {
        if (nTotal > 0) {
            input = input - samplesPerFrame + remainedSamples;
        }
        
        inMicLevel = 0;
        int nAgcRet = WebRtcAgc_Process(agcInst,
                                        (const int16_t *const *) &input,
                                        num_bands,
                                        samplesPerFrame,
                                        (int16_t *const *) &out16,
                                        inMicLevel,
                                        &outMicLevel,
                                        echo,
                                        &saturationWarning);
        
        if (nAgcRet != 0) {
            return -1;
        }
        
        memcpy(&input[samplesPerFrame - remainedSamples], 
               &out_buffer[samplesPerFrame - remainedSamples], 
               remainedSamples * sizeof(int16_t));
    }
    
    return 0;
}

// 释放 AGC 实例
void AgcFree(void* agcInst) {
    if (agcInst != NULL) {
        WebRtcAgc_Free(agcInst);
    }
}

// 获取默认配置
// 返回: 默认配置结构体
AgcConfig AgcGetDefaultConfig() {
    AgcConfig config;
    config.compressionGaindB = 9;      // 默认 9 dB
    config.targetLevelDbfs = 3;        // 默认 3 (-3 dBOv)
    config.limiterEnable = 1;          // 默认开启
    config.agcMode = kAgcModeAdaptiveDigital;  // 默认自适应数字模式
    return config;
}

