# Compiler and tools
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OCPY = $(CROSS_COMPILE)objcopy
ODUMP = $(CROSS_COMPILE)objdump
GDB = $(CROSS_COMPILE)gdb
MKDIR = mkdir
# QEMU = qemu-system-arm

# Project name and directories
TARGET ?= $(target-app)
BUILD_DIR ?= $(build-dir-app)
OBJ_DIR = $(BUILD_DIR)/objs
REPO_ROOT := $(shell pwd)
SUBDIR := ${sort ${dir ${wildcard ./*/ ./*/*/ ./*/*/*/}}}

# Source files and includes
# Source files
SRCS = $(wildcard $(addsuffix *.[csS],$(SUBDIR)))

# Include directories
INCLUDES = $(SUBDIR)
LD_FILE = board/linker/m1.ld

#Application files and includes
SRCS += $(addprefix $(repo-dir-app)/,$(srcs-app))
INCLUDES +=  $(addprefix $(repo-dir-app)/,$(inc-app))

# Flags and options
CFLAGS += -Wall -Wextra -g -Wno-format -mthumb -march=armv6-m -mcpu=cortex-m1	-Wall -std=c11 -specs=nano.specs -O3 -fdebug-prefix-map=$(REPO_ROOT)= -g -ffreestanding -ffunction-sections -fdata-sections $(foreach i,$(INCLUDES),-I$(i)) $(foreach d,$(DEFINES),-D$(d))
LDFLAGS = -mthumb -march=armv6-m -mcpu=cortex-m1 -Wl,--print-memory-usage -Wl,-Map=$(BUILD_DIR)/$(TARGET).map -T $(LD_FILE) -Wl,--gc-sections
# QEMU_FLAGS = -cpu cortex-m3 -machine lm3s6965evb -nographic -semihosting-config enable=on,target=native -gdb "tcp::50000" -S
# CFLAGS += -DSYSTICK_ENABLE
 CFLAGS += -DDEBUG_MODE #-DEVAL_MODE

# Object files
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
# Make targets
.PHONY: all
all:
	@echo "Building project..."
	@$(MAKE) --no-print-directory $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).lst

$(BUILD_DIR):
	@echo "Creating build directory..."
	@$(MKDIR) -p $(BUILD_DIR)

$(OBJ_DIR):
	@echo "Creating object directory..."
	@$(MKDIR) -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(MKDIR) -p $(dir $@)
	@$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@echo "Creating binary file..."
	@$(OCPY) $< $@ -O binary

$(BUILD_DIR)/$(TARGET).lst: $(BUILD_DIR)/$(TARGET).elf
	@echo "Creating listing file..."
	@$(ODUMP)  -d -M reg-names-raw -S -s $< > $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@echo "Linking object files..."
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	@echo "Deleting build directory..."
	@rm -rf $(BUILD_DIR)
