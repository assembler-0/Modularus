#!/usr/bin/env bash

BUILD_DIR="./.Build"
DEPS_DIR="./Dependencies"

DISK_IMG="$BUILD_DIR/modularus.img"
ISO_IMG="$BUILD_DIR/modularus.iso"
LOG_DIR="$BUILD_DIR/logs"

TYPE=${1:-"iso"}
SMP=${2:-1}
MEM=${3:-512M}

INFO="\033[0;33m[INFO]\033[0m"
OK="\033[0;32m[OK]\033[0m"
ERR="\033[0;31m[ERROR]\033[0m"

usage() {
    echo "Usage: $0 [iso|img|build] [smp_cores] [memory]"
    echo "Examples:"
    echo "  $0 iso 4 2G"
    echo "  $0 build"
    exit 1
}

build_project() {
    echo -e "$INFO Triggering project build..."
    make all || { echo -e "$ERR Build failed!"; exit 1; }
}

find_ovmf() {
    local search_paths=(
        "$DEPS_DIR/edk2-ovmf/edk2-ovmf/ovmf-code-x86_64.fd"
        "$DEPS_DIR/edk2-ovmf/ovmf-code-x86_64.fd"
    )

    for p in "${search_paths[@]}"; do
        if [[ -f "$p" ]]; then
            echo "$p"
            return 0
        fi
    done
    return 1
}

# Handle "build"
if [ "$TYPE" == "build" ]; then
    build_project
    exit 0
fi

# Validation
if [[ "$TYPE" != "iso" && "$TYPE" != "img" ]]; then
    usage
fi

TARGET_FILE=$([ "$TYPE" == "iso" ] && echo "$ISO_IMG" || echo "$DISK_IMG")

# uncomment this below section if you arent actively testing the kernel

#if [ ! -f "$TARGET_FILE" ]; then
#    echo -e "$INFO Artifact $TARGET_FILE not found."
#    build_project
#fi

# Else

# Rebuild
make clean || exit 1
build_project

mkdir -p "$LOG_DIR"

OVMF_PATH=$(find_ovmf)
if [ $? -ne 0 ]; then
    echo -e "$ERR OVMF firmware not found. Run: make fetch-deps"
    exit 1
fi

echo -e "$OK Launching QEMU ($TYPE) with $SMP cores and $MEM memory..."

if [ "$TYPE" == "iso" ]; then
    qemu-system-x86_64 \
        -machine q35 \
        -m "$MEM" \
        -smp "$SMP" \
        -drive if=pflash,unit=0,format=raw,file="$OVMF_PATH",readonly=on \
        -cdrom "$ISO_IMG" \
        -gdb tcp::1234 \
        -serial file:"$LOG_DIR/serial.log" \
        -d guest_errors,cpu_reset,int \
        -D "$LOG_DIR/qemu.log" \
        -no-reboot \
        -no-shutdown \
        -monitor stdio
else
    qemu-system-x86_64 \
        -machine q35 \
        -m "$MEM" \
        -smp "$SMP" \
        -drive if=pflash,unit=0,format=raw,file="$OVMF_PATH",readonly=on \
        -drive file="$DISK_IMG",format=raw \
        -gdb tcp::1234 \
        -serial file:"$LOG_DIR/serial.log" \
        -d guest_errors,cpu_reset,int \
        -D "$LOG_DIR/qemu.log" \
        -no-reboot \
        -no-shutdown \
        -monitor stdio
fi
