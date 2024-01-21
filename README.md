## Trying to make some use of this new MCU, while avoiding bloated and proprietary tools

I have a CH32X035F8U6-R0-1v0 evaluation board with bootloader v2.6.0, YMMV.

### Preparations:

```
# Go to a directory with your projects and clone this repo:
git clone https://github.com/Nable80/ch32x035_experiments

# Clone a repo with simple firmware flashing tools too:
git clone https://github.com/wagiminator/MCU-Flash-Tools

# Install libc and compiler packages for bare-metal RISC-V targets:
sudo apt update -y
sudo apt install picolibc-riscv64-unknown-elf gcc-riscv64-unknown-elf

# Install udev rules to avoid unnecessary privileges:
cd ch32x035_experiments
sudo install -v -m 0644 99-wch-isp.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
```

### Building and uploading firmware:

``chprog.py`` is the only tool that works for me right now:
```
make build && ../MCU-Flash-Tools/chprog.py main.bin
```

### Pin reminders:

USB and bootloader:
* Connect ``PC17`` to ``VCC`` with a resistor (4.7k or so) to enter USB bootloader on power-on
* ``PC17`` - ``D+``
* ``PC16`` - ``D-``

Serial Debug Interface (SDI, seems to be incompatible with SWD):
* ``PC18`` - ``DIO`` aka ``SWDIO``
* ``PC19`` - ``DCK`` aka ``SWCLK``

USART without remapping:
* ``PA2`` - ``TX2`` (``USART2_TX``)
* ``PA3`` - ``RX2`` (``USART2_RX``)
* ``PB3`` - ``TX3`` (note: ``RX3`` and ``RX4`` require remaps)
* ``PB0`` - ``TX4``
* ``PB11`` - ``RX1`` (note: F8U6 package doesn't provide ``TX1`` at all)

SPI:
* ``PA4`` - ``CS``
* ``PA5`` - ``SCK``
* ``PA6`` - ``MISO``
* ``PA7`` - ``MOSI``

### NOTE:

NOTE: original WCH's toolchain uses ``-march=rv32imacxw`` to enable useful proprietary extentions (non-standard compressed instructions, fast interrupt handling, etc) and all of this isn't available in a regular toolchain (did they publish any patches?).

I changed this to ``-march=rv32imac_zicsr`` to be able to compile the code at least somehow but I don't know much about RISC-V, feel free to correct me.
