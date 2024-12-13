# 🔮 Rune Editor

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/std/the-standard)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Security](https://img.shields.io/badge/Security-Enhanced-purple.svg)]()

> 🚀 A modern, secure programming environment for the Rune language, integrated with GhostC OS.

## ✨ Features

- 🎨 **Rune Language Support**: Full support for the Rune programming language with syntax highlighting and code completion
- 🔒 **GhostC OS Integration**: Seamless integration with GhostC OS security features and system calls
- 💾 **Memory Management**: Secure memory allocation and deallocation with built-in safety checks
- ⚡ **Process Management**: Advanced process control and monitoring capabilities
- 📁 **File System Operations**: Secure file operations with encryption support
- 📊 **System Monitoring**: Real-time system metrics and performance monitoring
- ⚠️ **Error Handling**: Comprehensive error handling and logging system

## 🛠️ Building from Source

### Prerequisites

| Requirement | Version |
|------------|---------|
| 💻 C++ Compiler | C++17 compatible |
| 🏗️ CMake | >= 3.18 |
| 👻 GhostC OS | Latest |

### 🚀 Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/rune-editor.git

# Navigate to the build directory
cd rune-lang
mkdir build && cd build

# Build the project
cmake ..
make
```

### 🧪 Running Tests

```bash
cd build
./rune_test
```

## 📂 Project Structure

```
📦 rune-editor
 ┣ 📂 docs            # 📚 Documentation
 ┣ 📂 examples        # 💡 Example programs
 ┣ 📂 rune-lang       # 🔧 Core implementation
 ┃ ┣ 📂 include      # 📑 Header files
 ┃ ┣ 📂 src          # 💻 Source files
 ┃ ┗ 📂 tests        # 🧪 Test suite
 ┗ 📂 build          # 🏗️ Build artifacts
```

## 💡 Examples

Check out our example programs in `/examples`:

| File | Description |
|------|-------------|
| 🖥️ `system_info.rune` | System metrics and info |
| ⚡ `process_management.rune` | Process control |
| 💾 `memory_management.rune` | Memory safety |
| 📁 `file_operations.rune` | File operations |

## 🤝 Contributing

Contributions are welcome! Please feel free to submit pull requests.

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🔐 Security

The Rune Editor is built with security at its core:
- 🛡️ Secure memory management
- 🔒 Process isolation
- 🔑 Encrypted file operations
- 📝 Comprehensive logging

## SD Card Preparation and Booting

### Prerequisites
- A micro SD card (minimum 4GB recommended)
- SD card reader
- Raspberry Pi (tested on Raspberry Pi 3B+ and 4)
- Root/sudo access on your Linux system

### Building the OS
1. First, build the kernel:
```bash
cd kernel
./build-os.sh
```

### Preparing the SD Card
1. Insert your SD card into your computer
2. Identify your SD card device (you can use `lsblk` command)
3. Run the preparation script as root:
```bash
sudo ./prepare-sd.sh /dev/sdX  # Replace X with your SD card device letter
```

⚠️ **WARNING**: Be very careful with the device name. Using the wrong device could erase your system drive!

### Booting GhostC OS
1. Insert the prepared SD card into your Raspberry Pi
2. Connect a monitor via HDMI
3. Connect a USB keyboard
4. Power on the Raspberry Pi

The GhostC OS will boot automatically and you'll see the GhostTerminal interface.

### Default Configuration
The default `config.txt` settings are optimized for:
- HDMI output at 1080p
- USB keyboard support
- Basic GPIO functionality

You can modify these settings by editing the `config.txt` file before running the preparation script.

### Troubleshooting
If you encounter boot issues:
1. Ensure the SD card is properly formatted
2. Verify all files were copied correctly
3. Check your power supply is adequate (recommended: 5V 2.5A)
4. Ensure your HDMI cable is connected before powering on

## 💬 Support

Need help? Open an issue in our issue tracker!

---
<div align="center">
Made with ❤️ by the GhostC OS team

Special thanks to:
- [Raspberry Pi Foundation](https://github.com/raspberrypi) for their incredible work on bare metal programming resources and documentation
- The open source community for their continuous support and contributions
</div>
