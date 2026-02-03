Compiler 	:= x86_64-elf-gcc
Assembler 	:= nasm
Linker 		:= x86_64-elf-ld

CFlags 		:= \
-std=c11 \
-ffreestanding \
-fno-stack-protector \
-fno-stack-check \
-fno-lto \
-fno-pie \
-fno-pic \
-fno-builtin \
-fno-omit-frame-pointer \
-mno-red-zone \
-mno-mmx \
-mno-sse \
-mno-sse2 \
-mno-sse3 \
-mno-ssse3 \
-mno-sse4.1 \
-mno-sse4.2 \
-mno-sse4 \
-mno-avx \
-mno-avx2 \
-mno-80387 \
-m64 \
-march=x86-64 \
-mcmodel=kernel \
-Wall \
-Wextra \
-Werror \
-Wno-unused-parameter \
-Wno-unused-variable \
-O2

AsmFlags 	:= \
-f elf64

LinkerFlags := \
-nostdlib \
-static \
-no-pie \
-m elf_x86_64 \
-z max-page-size=0x1000 \
-z noexecstack \
-z norelro \
-T KrnLink.ld

#	Includes
CFlags		+= -IHeaders
CFlags		+= -I../limine

TempBuild 	:= .Build
ObjectRoot 	:= $(TempBuild)/obj
BinaryRoot 	:= $(TempBuild)/bin

CSources 	:= $(shell find . -name "*.c" -type f)
AsmSources 	:= $(shell find . -name "*.asm" -type f)

CObjects 	:= $(patsubst ./%.c, $(ObjectRoot)/%.o, $(CSources))
AsmObjects 	:= $(patsubst ./%.asm, $(ObjectRoot)/%.o, $(AsmSources))
AllObjects 	:= $(CObjects) $(AsmObjects)

Target 		:= $(BinaryRoot)/corehotkrnl

.PHONY: \
all \
clean

all: $(Target)

$(Target): $(AllObjects) | $(BinaryRoot)

	$(Linker) $(AllObjects) $(LinkerFlags) -o $@

#	C
$(ObjectRoot)/%.o: %.c | $(ObjectRoot)

	@mkdir -p $(dir $@)
	$(Compiler) $(CFlags) -c $< -o $@

#	ASM
$(ObjectRoot)/%.o: %.asm | $(ObjectRoot)

	@mkdir -p $(dir $@)
	$(Assembler) $(AsmFlags) $< -o $@

$(ObjectRoot):

	mkdir -p $(ObjectRoot)

$(BinaryRoot):

	mkdir -p $(BinaryRoot)

clean:

	rm -rf $(TempBuild)
