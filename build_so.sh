#!/bin/bash

# 构建共享库脚本

set -e

echo "=== 构建 WebRTC AGC 共享库 ==="
echo ""

# 创建构建目录
mkdir -p build
cd build

# 运行 CMake
echo "运行 CMake..."
cmake ..

# 编译
echo "编译共享库..."
make agc_go

# 检查生成的文件
if [ -f "libagc.so" ] || [ -f "libagc.dylib" ] || [ -f "libagc.1.dylib" ]; then
    echo ""
    echo "✓ 共享库构建成功！"
    echo ""
    
    # 显示生成的文件
    if [ -f "libagc.so" ]; then
        echo "Linux 共享库: build/libagc.so"
        ls -lh libagc.so
    fi
    
    if [ -f "libagc.dylib" ]; then
        echo "macOS 共享库: build/libagc.dylib"
        ls -lh libagc.dylib
    fi
    
    if [ -f "libagc.1.dylib" ]; then
        echo "macOS 共享库: build/libagc.1.dylib"
        ls -lh libagc.1.dylib
        # 创建符号链接
        if [ ! -f "libagc.dylib" ]; then
            ln -sf libagc.1.dylib libagc.dylib
            echo "已创建符号链接: libagc.dylib -> libagc.1.dylib"
        fi
    fi
    
    echo ""
    echo "共享库已准备好，可以在 Go 代码中使用！"
else
    echo "✗ 共享库构建失败"
    exit 1
fi

