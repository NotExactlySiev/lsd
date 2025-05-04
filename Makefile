NAME	:= lsd

# Replace these with the prefix for your compile tools.
TGT 	:= mipsel-unknown-none-elf
MKISO	:= mkpsxiso
CC	:= $(TGT)-gcc
OBJCOPY	:= $(TGT)-objcopy

INCDIRS	:=
LIBDIRS	:=
LIBS	:=

INCDIRS	+= -Ipsyq/include
#LIBDIRS	+= -Lpsyq/lib
#LIBS		+= -lc -lapi # etc

# Required flags
CCFLAGS	:= -Wall -march=r3000 -mno-abicalls -static -msoft-float -fno-builtin
LDFLAGS := -T ps-exe.ld -Wl,--oformat=elf32-littlemips #-nostartfiles

# User Flags
CCFLAGS	+= -g -O1 -flto -G0
LDFLAGS += -Wl,-Map=build/$(NAME).map

BUILD	:= build

# Build a single executable or a disc image.
all: $(BUILD) $(BUILD)/$(NAME).exe
.PHONY: all

build/$(NAME).elf: src/main.cc src/entity.cc src/list.cc src/pad.cc src/biggrand.cc
	$(CC) $(INCDIRS) $(CCFLAGS) $(LIBDIRS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD)/$(NAME).exe: $(BUILD)/$(NAME).elf
	$(OBJCOPY) -O binary $^ $@

# In case you wanna embed any files into the binary.
$(BUILD)/%.elf: bin/%.*
	$(OBJCOPY) -I binary -O elf32-littlemips $^ $@

$(BUILD):
	mkdir -p $@

clean:
	rm -r build/*
.PHONY: clean
