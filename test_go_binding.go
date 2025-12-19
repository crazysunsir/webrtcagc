package main

/*
这是一个简单的测试文件，用于验证 Go 绑定是否正常工作
使用方法：
1. 确保已构建共享库: ./build_so.sh
2. 复制 go/agc 目录到当前目录
3. 运行: go run test_go_binding.go
*/

import (
	"fmt"
	"go/agc" // 根据实际路径修改
)

func main() {
	fmt.Println("测试 WebRTC AGC Go 绑定...")

	// 创建默认配置
	config := agc.DefaultConfig()
	fmt.Printf("默认配置: CompressionGaindB=%d, TargetLevelDbfs=%d, Mode=%d\n",
		config.CompressionGaindB, config.TargetLevelDbfs, config.Mode)

	// 创建 AGC 实例
	agcInstance, err := agc.NewAGC(16000, &config)
	if err != nil {
		fmt.Printf("创建 AGC 实例失败: %v\n", err)
		return
	}
	defer agcInstance.Close()

	fmt.Println("✓ AGC 实例创建成功")

	// 创建测试音频数据（静音）
	testSamples := make([]int16, 160) // 10ms @ 16kHz
	for i := range testSamples {
		testSamples[i] = 0
	}

	// 处理音频
	err = agcInstance.Process(testSamples)
	if err != nil {
		fmt.Printf("处理音频失败: %v\n", err)
		return
	}

	fmt.Println("✓ 音频处理成功")
	fmt.Println("测试完成！")
}

