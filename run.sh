#!/usr/bin/env bash

BUILD_DIR="./.build"
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
    echo "  $0 iso 4 2G    # Run ISO with 4 cores, 2GB RAM"
    echo "  $0 build       # Just build the project"
    exit 1
}

build_project() {
    echo -e "$INFO Triggering project build..."
    make all || { echo -e "$ERR Build failed!"; exit 1; }
}

find_ovmf() {
    local search_paths=(
        "./edk2-ovmf/edk2-ovmf/ovmf-code-x86_64.fd"
    )
    
    for p in "${search_paths[@]}"; do
        if [[ -f "$p" ]]; then
            echo "$p"
            return 0
        fi
    done
    return 1
}

# Handle "build" command specifically
if [ "$TYPE" == "build" ]; then
    build_project
    exit 0
fi

# Validation
if [[ "$TYPE" != "iso" && "$TYPE" != "img" ]]; then
    usage
fi

# Auto-Build Check: If the file doesn't exist, build it now
TARGET_FILE=$([ "$TYPE" == "iso" ] && echo "$ISO_IMG" || echo "$DISK_IMG")
if [ ! -f "$TARGET_FILE" ]; then
    echo -e "$INFO Artifact $TARGET_FILE not found."
    build_project
fi

# Setup Logging
mkdir -p "$LOG_DIR"

# Detect OVMF
OVMF_PATH=$(find_ovmf)
if [ $? -ne 0 ]; then
    echo -e "$ERR OVMF firmware not found. Please install ovmf/edk2 or run 'make fetch-deps'."
    exit 1
fi

# Launch QEMU
echo -e "$OK Launching QEMU ($TYPE) with $SMP cores and $MEM memory..."

qemu-system-x86_64 \
    -machine q35 \
    -m "$MEM" \
    -smp "$SMP" \
    -bios "$OVMF_PATH" \
    -drive file="$TARGET_FILE",format=raw \
    -gdb tcp::1234 \
    -serial file:"$LOG_DIR/serial.log" \
    -d guest_errors,cpu_reset,int \
    -D "$LOG_DIR/qemu.log" \
    -no-reboot \
    -no-shutdown \
    -monitor stdio