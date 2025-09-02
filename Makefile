NAME	:= minimal

# Replace these with the prefix for your compile tools.
TGT 	:= mipsel-unknown-none-elf
MKISO	:= mkpsxiso
CC	:= $(TGT)-gcc
OBJCOPY	:= $(TGT)-objcopy

INCDIRS	:=
LIBDIRS	:=
LIBS	:=

# Do you want the spicyjpeg headers?
#INCDIRS	+= -Ispicy

# Do you want Psy-Q?
INCDIRS	+= -Ipsyq/include
LIBDIRS	+= -Lpsyq/libs
LIBS	+= -lgs -lgpu -lgte -letc -lc2 -lapi # etc

# Required flags
CCFLAGS	:= -Wall -ffreestanding -mips1 -march=r3000 -static -msoft-float
LDFLAGS := -T ps-exe.ld -Wl,--oformat=elf32-littlemips -nostartfiles

# User Flags
CCFLAGS	+= -g -O1 -flto -G0
LDFLAGS += -Wl,-Map=build/$(NAME).map

BUILD	:= build

# Build a single executable or a disc image.
all: $(BUILD) $(BUILD)/$(NAME).bin
.PHONY: all

build/$(NAME).elf: src/main.cc src/crt0.s
	$(CC) $(INCDIRS) $(CCFLAGS) $(LIBDIRS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD)/$(NAME).exe: $(BUILD)/$(NAME).elf
	$(OBJCOPY) -O binary $^ $@

# This rule can be removed if you don't need a disc image.
$(BUILD)/$(NAME).bin: disc.xml $(BUILD)/$(NAME).exe $(FILES)
	$(MKISO) -y --quiet --cuefile build/$(NAME).cue --output $@ $<

# In case you wanna embed any files into the binary.
$(BUILD)/%.elf: bin/%.*
	$(OBJCOPY) -I binary -O elf32-littlemips $^ $@

$(BUILD):
	mkdir -p $@

clean:
	rm -r build/*
.PHONY: clean
