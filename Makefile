NAME	:= lsd

# Replace these with the prefix for your compile tools.
TGT 	:= mipsel-unknown-linux-gnu
MKISO	:= mkpsxiso
CC	:= $(TGT)-gcc
OBJCOPY	:= $(TGT)-objcopy

INCDIRS	:=
LIBDIRS	:=
LIBS	:=

INCDIRS	+= -Ipsyq/include
LIBDIRS	+= -Lpsyq/libs
LIBS	+= -lc2 -lapi # etc

# Required flags
CCFLAGS	:= -Wall -fno-builtin -nostdinc -nostdlib -static -mips1 -march=r3000 -mno-abicalls -msoft-float -fno-pic
LDFLAGS := -T ps-exe.ld -Wl,--oformat=elf32-little #-nostartfiles

# User Flags
CCFLAGS	+= -g -O1 -flto -G0
LDFLAGS += -Wl,-Map=build/$(NAME).map

BUILD	:= build

# Build a single executable or a disc image.
all: $(BUILD) $(BUILD)/$(NAME).exe
.PHONY: all

build/$(NAME).elf: src/main.cc \
				   src/entity.cc \
				   src/pad.cc \
				   src/object.cc \
				   src/gpu.cc \
				   src/renderer.cc \
				   src/modelset.cc \
				   src/model.cc \
				   src/util.cc \
				   src/list.cc \
				   src/crt0.s
	$(CC) $(INCDIRS) $(CCFLAGS) $(LIBDIRS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD)/$(NAME).exe: $(BUILD)/$(NAME).elf
	$(OBJCOPY) -I elf32-little -O binary $^ $@

# In case you wanna embed any files into the binary.
$(BUILD)/%.elf: bin/%.*
	$(OBJCOPY) -I binary -O elf32-little $^ $@

$(BUILD):
	mkdir -p $@

clean:
	rm -r build/*
.PHONY: clean
