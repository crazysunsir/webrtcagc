/*
 * AGC Wrapper Header for Go CGO
 */

#ifndef AGC_WRAPPER_H
#define AGC_WRAPPER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// AGC 配置结构体
typedef struct {
    int16_t compressionGaindB;  // 压缩增益 (dB)
    int16_t targetLevelDbfs;    // 目标电平 (0-31)
    uint8_t limiterEnable;      // 限幅器开关 (0/1)
    int16_t agcMode;            // 增益模式
} AgcConfig;

// 增益模式常量
#define AGC_MODE_UNCHANGED        0
#define AGC_MODE_ADAPTIVE_ANALOG  1
#define AGC_MODE_ADAPTIVE_DIGITAL 2
#define AGC_MODE_FIXED_DIGITAL    3

// 创建 AGC 实例
void* AgcCreate(void);

// 初始化 AGC
int AgcInit(void* agcInst, int32_t minLevel, int32_t maxLevel, 
            int16_t agcMode, uint32_t sampleRate);

// 设置 AGC 配置
int AgcSetConfig(void* agcInst, AgcConfig* config);

// 处理音频数据
int AgcProcess(void* agcInst, int16_t* samples, size_t numSamples, uint32_t sampleRate);

// 释放 AGC 实例
void AgcFree(void* agcInst);

// 获取默认配置
AgcConfig AgcGetDefaultConfig(void);

#ifdef __cplusplus
}
#endif

#endif // AGC_WRAPPER_H

