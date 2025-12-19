#!/bin/bash

# WebRTC AGC 快速测试脚本

echo "=== WebRTC AGC 快速测试 ==="
echo ""

# 检查是否提供了音频文件参数
if [ $# -eq 0 ]; then
    echo "用法: $0 <音频文件.wav>"
    echo ""
    echo "示例:"
    echo "  $0 input.wav"
    echo ""
    echo "程序会在同一目录生成 output.wav 文件"
    exit 1
fi

INPUT_FILE="$1"

# 检查文件是否存在
if [ ! -f "$INPUT_FILE" ]; then
    echo "错误: 文件 '$INPUT_FILE' 不存在"
    exit 1
fi

# 检查是否已编译
if [ ! -f "build/agc" ]; then
    echo "错误: 可执行文件不存在，请先编译："
    echo "  mkdir -p build && cd build && cmake .. && make"
    exit 1
fi

echo "输入文件: $INPUT_FILE"
echo "开始处理..."
echo ""

# 运行程序
./build/agc "$INPUT_FILE"

echo ""
echo "=== 处理完成 ==="

