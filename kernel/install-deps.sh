#!/bin/bash

# Install required packages for building
sudo apt-get update
sudo apt-get install -y \
    git \
    build-essential \
    cmake \
    gcc-arm-linux-gnueabihf \
    g++-arm-linux-gnueabihf \
    debootstrap \
    qemu-user-static \
    binfmt-support \
    parted \
    dosfstools \
    zip \
    unzip \
    libncurses5-dev \
    libssl-dev \
    bc \
    bison \
    flex \
    python3 \
    python3-pip \
    rsync \
    wget \
    cpio \
    file \
    kpartx
