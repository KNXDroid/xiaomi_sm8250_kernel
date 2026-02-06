#!/bin/bash
make CC=$(pwd)/clang/bin/clang \
LD=$(pwd)/clang/bin/ld.lld \
NM=$(pwd)/clang/bin/llvm-nm \
AR=$(pwd)/clang/bin/llvm-ar \
OBJCOPY=$(pwd)/clang/bin/llvm-objcopy \
OBJDUMP=$(pwd)/clang/bin/llvm-objdump \
STRIP=$(pwd)/clang/bin/llvm-strip \
CROSS_COMPILE=$(pwd)/gcc-compiler/bin/aarch64-none-linux-gnu- \
O=out ARCH=arm64 -j$(($(nproc)+1)) $@
