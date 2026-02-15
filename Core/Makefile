CC      ?= x86_64-elf-gcc
AS      ?= nasm
LD      ?= x86_64-elf-ld
CHAIN   ?= gcc

PROJ_ROOT ?= ..
BUILD_DIR ?= $(PROJ_ROOT)/.Build/
OBJ_DIR   := $(BUILD_DIR)/obj
BIN_DIR   := $(BUILD_DIR)/bin

TARGET    := $(BIN_DIR)/modularus

CFLAGS := \
    -std=c23 \
	-O2 \
	-g \
	-Wall \
	-Wextra \
	-Werror \
    -ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
    -fno-lto \
	-fno-pie \
	-fno-pic \
	-fno-builtin \
    -fno-omit-frame-pointer \
	-ftrivial-auto-var-init=pattern \
    -m64 \
	-march=x86-64 \
	-mcmodel=kernel \
	-mno-red-zone \
    -mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-80387 \
	-fno-strict-aliasing \
	-fstack-protector-all \
	-fno-common \
	-fwrapv \
    -IHeaders \
	-I../limine \
	-MMD \
	-MP

ifeq ($(CHAIN), clang)
    CFLAGS += -Wno-unknown-warning-option
endif

ASFLAGS := \
	-f elf64
LDFLAGS := \
	-T KrnLink.ld \
	-nostdlib \
	-static \
	-no-pie \
	-m elf_x86_64 \
    -z max-page-size=0x1000 \
	-z noexecstack \
	-z norelro

C_SRCS   := $(shell find . -name "*.c")
ASM_SRCS := $(shell find . -name "*.asm")

OBJS := $(C_SRCS:%.c=$(OBJ_DIR)/%.o)
OBJS += $(ASM_SRCS:%.asm=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: \
	all \
	clean

all: $(TARGET)

# Linking the Kernel
$(TARGET): $(OBJS)
	@echo -e "  LD      $(notdir $@)"
	@mkdir -p $(dir $@)
	@$(LD) $(LDFLAGS) $(OBJS) -o $@

# Compiling C sources
$(OBJ_DIR)/%.o: %.c
	@echo -e "  CC      $(notdir $<)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Assembling Sources
$(OBJ_DIR)/%.o: %.asm
	@echo -e "  AS      $(notdir $<)"
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) $< -o $@

clean:
	@echo -e "  CLEAN   Core"
	@rm -rf $(BUILD_DIR)

-include $(DEPS)