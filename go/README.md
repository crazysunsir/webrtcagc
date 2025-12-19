# Go AGC 绑定

这是 WebRTC AGC 的 Go 语言绑定，允许在 Go 项目中使用 AGC 自动增益控制功能。

## 快速开始

### 1. 构建共享库

在项目根目录执行：

```bash
./build_so.sh
```

### 2. 在 Go 项目中使用

```go
package main

import (
    "fmt"
    "your-module/go/agc"  // 根据实际路径修改
)

func main() {
    // 创建配置
    config := &agc.Config{
        CompressionGaindB: 12,
        TargetLevelDbfs:   1,
        LimiterEnable:     true,
        Mode:              agc.ModeAdaptiveDigital,
    }
    
    // 创建 AGC 实例
    agcInstance, err := agc.NewAGC(16000, config)
    if err != nil {
        panic(err)
    }
    defer agcInstance.Close()
    
    // 处理音频数据（16-bit PCM）
    samples := []int16{...}  // 你的音频数据
    err = agcInstance.Process(samples)
    if err != nil {
        panic(err)
    }
    
    fmt.Println("处理完成！")
}
```

## 文件说明

- `agc.go` - Go 绑定代码
- `agc_wrapper.h` - C 头文件（需要与 agc.go 在同一目录）
- `example/main.go` - 使用示例

## 注意事项

1. 确保共享库 `libagc.dylib` (macOS) 或 `libagc.so` (Linux) 在 `build/` 目录中
2. 头文件 `agc_wrapper.h` 需要与 `agc.go` 在同一目录
3. 如果共享库不在系统路径中，代码中已设置 `-Wl,-rpath` 自动查找

## 更多信息

查看 `../GO_USAGE.md` 获取详细使用说明。

