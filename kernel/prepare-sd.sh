#!/bin/bash

# Check if running as root
if [ "$EUID" -ne 0 ]; then 
    echo "Please run as root"
    exit 1
fi

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 /dev/sdX (replace X with your SD card device letter)"
    exit 1
fi

DEVICE=$1
BUILD_DIR="build"
MOUNT_POINT="/mnt/ghostc_boot"

# Verify device exists
if [ ! -b "$DEVICE" ]; then
    echo "Error: Device $DEVICE does not exist"
    exit 1
fi

echo "WARNING: This will erase all data on $DEVICE"
read -p "Are you sure you want to continue? (y/n) " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    exit 1
fi

# Create partitions
echo "Creating partitions..."
parted -s "$DEVICE" mklabel msdos
parted -s "$DEVICE" mkpart primary fat32 1MiB 100%
parted -s "$DEVICE" set 1 boot on

# Format boot partition
echo "Formatting boot partition..."
mkfs.vfat -F 32 "${DEVICE}1"

# Create mount point
mkdir -p "$MOUNT_POINT"

# Mount boot partition
mount "${DEVICE}1" "$MOUNT_POINT"

# Copy kernel and boot files
echo "Copying boot files..."
cp "$BUILD_DIR/kernel.img" "$MOUNT_POINT/"
cp config.txt "$MOUNT_POINT/"
cp boot.asm "$MOUNT_POINT/"

# Sync and unmount
sync
umount "$MOUNT_POINT"

echo "SD card preparation complete!"
echo "You can now insert the SD card into your Raspberry Pi and boot GhostC OS"
