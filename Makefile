TARGET := main
PREFIX ?= riscv64-unknown-elf

CFLAGS += \
	-g -ggdb -Os -flto \
	-static-libgcc \
	-march=rv32imac_zicsr -mabi=ilp32 -msmall-data-limit=8 \
	-I /usr/include/newlib \
	-I lib/Core \
	-I lib/Debug \
	-I lib/Peripheral/inc \
	-I . \
	-ffunction-sections -fdata-sections -fno-common \
	-nostdlib -ffreestanding \
	-D DEBUG=DEBUG_UART2 \
	-Wall -Wextra

LDFLAGS += -T lib/Ld/Link.ld -Wl,--gc-sections -lgcc

SOURCES := \
	lib/Startup/startup_ch32x035.S \
	lib/Debug/debug.c \
	system_ch32x035.c \
	ch32x035_it.c \
	lib/Peripheral/src/ch32x035_gpio.c \
	lib/Peripheral/src/ch32x035_rcc.c \
	lib/Peripheral/src/ch32x035_usart.c \
	main.c

$(TARGET).elf: $(SOURCES)
	$(PREFIX)-gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(TARGET).bin: $(TARGET).elf
	$(PREFIX)-size $^
	$(PREFIX)-objdump -S $^ > $(TARGET).lst
	$(PREFIX)-objdump -t $^ > $(TARGET).map
	$(PREFIX)-objcopy -O binary $< $(TARGET).bin
	$(PREFIX)-objcopy -O ihex $< $(TARGET).hex

clean:
	rm -f $(TARGET).elf $(TARGET).bin $(TARGET).hex $(TARGET).lst $(TARGET).map $(TARGET).hex

build: $(TARGET).bin
