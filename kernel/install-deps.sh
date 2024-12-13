#!/bin/bash

# Update package lists
sudo apt-get update

# Install essential build tools
sudo apt-get install -y \
    git \
    build-essential \
    cmake \
    make \
    gcc \
    g++ \
    gdb \
    binutils

# Install cross-compilation tools
sudo apt-get install -y \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    libnewlib-arm-none-eabi \
    gcc-arm-linux-gnueabihf \
    g++-arm-linux-gnueabihf

# Install Raspberry Pi specific tools
sudo apt-get install -y \
    raspberrypi-kernel-headers \
    libraspberrypi-dev \
    libraspberrypi-bin

# Install disk and filesystem tools
sudo apt-get install -y \
    parted \
    dosfstools \
    zip \
    unzip \
    fdisk \
    mount

# Install development libraries
sudo apt-get install -y \
    libncurses5-dev \
    libssl-dev \
    libelf-dev \
    bison \
    flex

# Install QEMU for testing
sudo apt-get install -y \
    qemu-system-arm \
    qemu-user-static \
    binfmt-support

# Create build directory if it doesn't exist
mkdir -p build

echo "All dependencies installed successfully!"
