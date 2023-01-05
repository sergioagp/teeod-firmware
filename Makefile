REPO_ROOT := $(shell pwd)
TARGET ?= aes-ecb-test
BUILD_DIR ?= build

SRCS = main.c aes-ecb.c syscalls.c uart.c startup.s system.c
INCLUDES += .

CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OCPY = $(CROSS_COMPILE)objcopy
GDB = $(CROSS_COMPILE)gdb

QEMU = qemu-system-arm
MKDIR = mkdir

CFLAGS += \
	-mthumb \
	-march=armv6-m \
	-mcpu=cortex-m1 \
	-Wall \
	-std=c11 \
	-specs=nano.specs \
	-O0 \
	-fdebug-prefix-map=$(REPO_ROOT)= \
	-g \
	-ffreestanding \
	-ffunction-sections \
	-fdata-sections

LDFLAGS += \
	-mthumb \
	-march=armv6-m \
	-mcpu=cortex-m1 \
	-Wl,--print-memory-usage \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map \
	-T m1.ld \
	-Wl,--gc-sections \

CFLAGS += $(foreach i,$(INCLUDES),-I$(i))

QEMU_FLAGS = -cpu cortex-m3 -machine lm3s6965evb -nographic -semihosting-config enable=on,target=native -gdb "tcp::50000" -S

OBJ_DIR = $(BUILD_DIR)/objs
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all
all: $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR):
	$(MKDIR) -p $(BUILD_DIR)

$(OBJ_DIR):
	$(MKDIR) -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	$(MKDIR) -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OCPY) $< $@ -O binary

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: qemu
qemu: $(BUILD_DIR)/$(TARGET).elf
	$(QEMU) $(QEMU_FLAGS) -kernel $<
