#!/bin/bash

set -e

KERNEL_IMG="$1"
SD_IMG="$2"
BOOT_SIZE=64  # Size in MB
ROOT_SIZE=256 # Size in MB

if [ -z "$KERNEL_IMG" ] || [ -z "$SD_IMG" ]; then
    echo "Usage: $0 <kernel.img> <sd.img>"
    exit 1
fi

echo "Creating SD card image for Raspberry Pi Zero W..."

# Calculate total size
TOTAL_SIZE=$((BOOT_SIZE + ROOT_SIZE))

# Create empty image
dd if=/dev/zero of="$SD_IMG" bs=1M count=$TOTAL_SIZE

# Set up loop device
LOOP_DEV=$(sudo losetup -f)
sudo losetup "$LOOP_DEV" "$SD_IMG"

# Create partition table
sudo parted -s "$LOOP_DEV" mklabel msdos
sudo parted -s "$LOOP_DEV" mkpart primary fat32 1MiB ${BOOT_SIZE}MiB
sudo parted -s "$LOOP_DEV" mkpart primary ext4 ${BOOT_SIZE}MiB 100%
sudo parted -s "$LOOP_DEV" set 1 boot on

# Set up partitions
sudo kpartx -av "$LOOP_DEV"
LOOP_NAME=$(basename "$LOOP_DEV")
BOOT_PART="/dev/mapper/${LOOP_NAME}p1"
ROOT_PART="/dev/mapper/${LOOP_NAME}p2"

# Format partitions
sudo mkfs.vfat -F 32 -n "BOOT" "$BOOT_PART"
sudo mkfs.ext4 -L "ROOT" "$ROOT_PART"

# Mount partitions
MOUNT_DIR=$(mktemp -d)
sudo mount "$BOOT_PART" "$MOUNT_DIR"

# Copy boot files
echo "Copying boot files..."
sudo cp "$KERNEL_IMG" "$MOUNT_DIR/kernel.img"
sudo cp config.txt "$MOUNT_DIR/"
sudo cp cmdline.txt "$MOUNT_DIR/" 2>/dev/null || echo "cmdline.txt not found, skipping..."

# Unmount and clean up
sudo umount "$MOUNT_DIR"
sudo rmdir "$MOUNT_DIR"
sudo kpartx -d "$LOOP_DEV"
sudo losetup -d "$LOOP_DEV"

echo "SD card image created successfully: $SD_IMG"
echo "Write this image to your SD card using 'dd' or your preferred imaging tool"
