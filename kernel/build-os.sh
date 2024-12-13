#!/bin/bash
set -e

# Parse command line arguments
PLATFORM="zero-w"  # Default to Zero W
CROSS_COMPILE="arm-linux-gnueabihf-"
BUILD_DIR="/home/ghost/build"

# Add Zero W specific configurations
ZERO_W_CFLAGS="-mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard"
ZERO_W_DEFINES="-DRPI_ZERO_W -DGHOSTC_ENABLED"

while [[ $# -gt 0 ]]; do
    case $1 in
        --platform|-p)
            PLATFORM="$2"
            shift 2
            ;;
        --build-dir|-b)
            BUILD_DIR="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Set platform-specific flags
if [ "$PLATFORM" = "zero-w" ]; then
    CFLAGS="$ZERO_W_CFLAGS $ZERO_W_DEFINES"
fi

# Set platform-specific configurations
case $PLATFORM in
    "zero-w")
        echo "Building for Raspberry Pi Zero W..."
        DTB="bcm2835-rpi-zero-w.dtb"
        ;;
    "zero")
        echo "Building for Raspberry Pi Zero..."
        DTB="bcm2835-rpi-zero.dtb"
        ;;
    "3"|"3b"|"3b+")
        echo "Building for Raspberry Pi 3..."
        CFLAGS="-mcpu=cortex-a53 -mfpu=neon-fp-armv8 -mfloat-abi=hard -O2"
        DTB="bcm2837-rpi-3-b.dtb"
        ;;
    *)
        echo "Unsupported platform: $PLATFORM"
        exit 1
        ;;
esac

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Clone Raspberry Pi repositories if they don't exist
if [ ! -d "pi-gen" ]; then
    git clone https://github.com/RPi-Distro/pi-gen.git
fi

if [ ! -d "linux" ]; then
    git clone --depth=1 -b rpi-6.6.y https://github.com/raspberrypi/linux.git
fi

if [ ! -d "firmware" ]; then
    git clone --depth=1 https://github.com/raspberrypi/firmware.git
fi

if [ ! -d "userland" ]; then
    git clone --depth=1 https://github.com/raspberrypi/userland.git
fi

# Build kernel
cd linux
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcmrpi_defconfig
echo 'CONFIG_LOCALVERSION="-ghostc"' >> .config
echo 'CONFIG_CMDLINE="console=tty1 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline fsck.repair=yes rootwait"' >> .config

echo "Building kernel..."
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- olddefconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage -j$(nproc)

echo "Building device tree files..."
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- dtbs -j$(nproc)

echo "Building kernel modules..."
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules -j$(nproc)

# Create boot directory
mkdir -p ../boot/overlays

# Copy kernel and device tree files
cp arch/arm/boot/zImage ../boot/kernel.img
cp arch/arm/boot/dts/broadcom/bcm2835-rpi-zero*.dtb ../boot/ || true
cp arch/arm/boot/dts/broadcom/bcm2708-rpi-zero*.dtb ../boot/ || true
cp arch/arm/boot/dts/overlays/*.dtb* ../boot/overlays/ || true

# Install modules
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=../rootfs modules_install

# Build userland tools
cd ../userland

# Clean any previous build
rm -rf build
mkdir -p build/arm-linux/release
cd build/arm-linux/release

# Configure and build userland
cmake -DCMAKE_TOOLCHAIN_FILE=../../../makefiles/cmake/toolchains/arm-linux-gnueabihf.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -DARM64=OFF \
      -DARM32=ON \
      -DVMCS_INSTALL_PREFIX=/opt/vc \
      ../../..

make -j$(nproc)

# Copy userland binaries
mkdir -p ../../../boot/bin
find . -type f -executable -not -name "*.cmake" -not -name "*.o" -not -name "*.a" -exec cp {} ../../../boot/bin/ \;

echo "Building GhostC OS..."
cd ../../../pi-gen

# Configure pi-gen
cat > config << EOF
IMG_NAME=GhostC
RELEASE=bullseye
DEPLOY_ZIP=0
LOCALE_DEFAULT=en_US.UTF-8
TARGET_HOSTNAME=ghostc
KEYBOARD_KEYMAP=us
KEYBOARD_LAYOUT="English (US)"
TIMEZONE_DEFAULT=America/Los_Angeles
FIRST_USER_NAME=ghost
FIRST_USER_PASS=ghostc
ENABLE_SSH=1
EOF

# Build the image
./build.sh

# Create platform-specific config adjustments
if [ "$PLATFORM" != "zero-w" ] && [ "$PLATFORM" != "zero" ]; then
    # Enable high-performance settings for more powerful boards
    sed -i 's/^gpu_mem=32/gpu_mem=128/' config.txt
    sed -i 's/^hdmi_mode=4/hdmi_mode=16/' config.txt
    sed -i 's/^dtparam=audio=off/dtparam=audio=on/' config.txt
    sed -i 's/^max_usb_current=0/max_usb_current=1/' config.txt
    sed -i '/^dtoverlay=disable-bt/d' config.txt
    sed -i '/^dtoverlay=miniuart-bt/d' config.txt
fi

echo "Build complete! Output files in $BUILD_DIR:"
echo "- kernel.img (Main kernel image)"
echo "- config.txt (Platform-specific configuration)"
echo "- boot.asm (Boot code)"

echo -e "\nTo create a bootable SD card, run: sudo ./prepare-sd.sh /dev/sdX"
