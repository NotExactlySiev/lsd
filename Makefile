NAME	:= lsd

# Replace these with the prefix for your compile tools.
# TGT 	:= mipsel-unknown-none-elf
TGT :=
MKISO	:= mkpsxiso
CC	:= $(TGT)g++
OBJCOPY	:= $(TGT)-objcopy

INCDIRS	:=
LIBDIRS	:=
LIBS	:=

INCDIRS	+= -Ipsyq/include
#LIBDIRS	+= -Lpsyq/lib
#LIBS		+= -lc -lapi # etc

# Required flags
CCFLAGS	:= -Wall -fno-builtin #-static -march=r3000 -mno-abicalls -msoft-float
#LDFLAGS := -T ps-exe.ld -Wl,--oformat=elf32-littlemips #-nostartfiles

# User Flags
# CCFLAGS	+= -g -O1 -flto -G0
LDFLAGS += -Wl,-Map=build/$(NAME).map

BUILD	:= build

# Build a single executable or a disc image.
all: $(BUILD) $(BUILD)/$(NAME)
.PHONY: all

build/$(NAME): src/main.cc src/entity.cc src/list.cc src/pad.cc src/object.cc src/gpu.cc src/renderer.cc
	$(CC) $(INCDIRS) $(CCFLAGS) $(LIBDIRS) $(LDFLAGS) -o $@ $^ $(LIBS)

#$(BUILD)/%.o: src/%.c
#	$(CC) -c $(INCDIRS) $(CCFLAGS) -o $@ $^

#$(BUILD)/$(NAME).exe: $(BUILD)/$(NAME).elf
#	$(OBJCOPY) -O binary $^ $@

# In case you wanna embed any files into the binary.
$(BUILD)/%.elf: bin/%.*
	$(OBJCOPY) -I binary -O elf32-littlemips $^ $@

$(BUILD):
	mkdir -p $@

clean:
	rm -r build/*
.PHONY: clean
