#include "GhostTerminal.hpp"
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <sys/ioctl.h>
#include <unistd.h>

namespace RuneLang {

GhostTerminal::GhostTerminal() : isRunning(false), isBinding(false) {
    monitor = std::make_unique<RuneMonitor>();
    registerRuneEffects();
}

void GhostTerminal::initialize() {
    registerDefaultCommands();
    registerRuneCommands();
    initializeRuneEnvironment();
    isRunning = true;
    currentDirectory = "/";
    clearScreen();
    displayRunicBorder();
    displayBanner();
}

void GhostTerminal::registerRuneEffects() {
    // Protection runes
    runeEffects["ᛉ"] = RuneEffect{"ᛉ", {
        "⟨ᛉ⟩", "《ᛉ》", "【ᛉ】", "「ᛉ」"
    }, 100, true, 0xFF00FF00};  // Green glow
    
    runeEffects["ᚹ"] = RuneEffect{"ᚹ", {
        "⟨ᚹ⟩", "《ᚹ》", "【ᚹ】", "「ᚹ」"
    }, 100, true, 0xFF0000FF};  // Blue glow
    
    // Advanced Protection Rituals
    runeEffects["ᚨ"] = RuneEffect{"ᚨ", {
        "⟨ᚨ⟩", "《ᚨ》", "【ᚨ】", "「ᚨ」"
    }, 100, true, 0xFF00FFFF};  // Cyan glow
    
    runeEffects["ᛋ"] = RuneEffect{"ᛋ", {
        "⟨ᛋ⟩", "《ᛋ》", "【ᛋ】", "「ᛋ」"
    }, 100, true, 0xFF00FFFF};  // Cyan glow
    
    runeEffects["ᚠ"] = RuneEffect{"ᚠ", {
        "⟨ᚠ⟩", "《ᚠ》", "【ᚠ】", "「ᚠ」"
    }, 100, true, 0xFF00FFFF};  // Cyan glow
    
    // Advanced Operation Runes
    runeEffects["ᛟ"] = RuneEffect{"ᛟ", {
        "⟨ᛟ⟩", "《ᛟ》", "【ᛟ】", "「ᛟ」"
    }, 100, true, 0xFFFF0000};  // Red glow for system ops
    
    runeEffects["ᛗ"] = RuneEffect{"ᛗ", {
        "⟨ᛗ⟩", "《ᛗ》", "【ᛗ】", "「ᛗ」"
    }, 100, true, 0xFF8A2BE2};  // Purple glow for memory ops
    
    runeEffects["ᚷ"] = RuneEffect{"ᚷ", {
        "⟨ᚷ⟩", "《ᚷ》", "【ᚷ】", "「ᚷ」"
    }, 100, true, 0xFFFFD700};  // Gold glow for gateway ops
    
    runeEffects["ᛏ"] = RuneEffect{"ᛏ", {
        "⟨ᛏ⟩", "《ᛏ》", "【ᛏ】", "「ᛏ」"
    }, 100, true, 0xFF4682B4};  // Steel blue for trace ops
}

void GhostTerminal::registerRuneCommands() {
    // Advanced Protection Rituals
    registerCommand("analyze", "ᚨ", "System analysis ritual", RuneCategory::PROTECTION,
        [this](const auto& args) {
            if (!validateRitual({"ᛉ", "ᚹ"})) {
                std::cout << "Ritual requires shield (ᛉ) and ward (ᚹ) runes for protection" << std::endl;
                return;
            }
            displayRuneEffect(runeEffects["ᚨ"]);
            monitor->startSystemAnalysis();
        }, true, {"ᛉ", "ᚹ"});

    registerCommand("scan", "ᛋ", "Network scanning ritual", RuneCategory::PROTECTION,
        [this](const auto& args) {
            if (!validateRitual({"ᛉ", "ᚹ", "ᚦ"})) {
                std::cout << "Ritual requires shield (ᛉ), ward (ᚹ), and power (ᚦ) runes" << std::endl;
                return;
            }
            displayRuneEffect(runeEffects["ᛋ"]);
            monitor->startNetworkScan();
        }, true, {"ᛉ", "ᚹ", "ᚦ"});

    registerCommand("fortify", "ᚠ", "System fortification ritual", RuneCategory::PROTECTION,
        [this](const auto& args) {
            if (!validateRitual({"ᛉ", "ᚹ", "ᚦ", "ᛈ"})) {
                std::cout << "Ritual requires shield, ward, power, and path runes" << std::endl;
                return;
            }
            displayRuneEffect(runeEffects["ᚠ"]);
            monitor->fortifySystem();
        }, true, {"ᛉ", "ᚹ", "ᚦ", "ᛈ"});

    // Protection/Security Runes
    registerCommand("shield", "ᛉ", "Activate system protection", RuneCategory::PROTECTION,
        [this](const auto&) {
            displayRuneEffect(runeEffects["ᛉ"]);
            std::cout << "Activating shield rune (ᛉ) - System protection enabled" << std::endl;
            monitor->startProtection();
        });

    registerCommand("ward", "ᚹ", "Create protective barrier", RuneCategory::PROTECTION,
        [this](const auto&) {
            displayRuneEffect(runeEffects["ᚹ"]);
            std::cout << "Activating ward rune (ᚹ) - Protective barrier established" << std::endl;
            monitor->enableBarrier();
        });

    // System Runes
    registerCommand("power", "ᚦ", "System power control", RuneCategory::SYSTEM,
        [this](const auto&) {
            displayRuneEffect(runeEffects["ᚦ"]);
            showRunePower();
        });

    // Navigation Runes
    registerCommand("path", "ᛈ", "Path finding and navigation", RuneCategory::NAVIGATION,
        [this](const auto& args) {
            displayRuneEffect(runeEffects["ᛈ"]);
            if (args.empty()) {
                visualizeRuneNetwork();
            } else {
                std::cout << "Navigating to: " << args[0] << std::endl;
            }
        });

    // Ritual Runes
    registerCommand("ritual", "ᚱ", "Start rune ritual", RuneCategory::RITUAL,
        [this](const auto& args) {
            if (args.size() < 2) {
                std::cout << "Usage: ritual <primary_rune> <supporting_runes...>" << std::endl;
                return;
            }
            std::vector<std::string> supportingRunes(args.begin() + 1, args.end());
            performRuneRitual(args[0], supportingRunes);
        }, true, {"ᛉ", "ᚹ", "ᚦ"});

    // Editor Runes
    registerCommand("write", "ᚱ", "Open Rune Editor", RuneCategory::EDITOR,
        [this](const auto& args) {
            displayRuneEffect(runeEffects["ᚱ"]);
            if (!args.empty()) {
                openRuneEditor(args[0]);
            } else {
                std::cout << "Usage: ᚱ <filename>" << std::endl;
            }
        });

    // System Operations
    registerCommand("process", "ᛟ", "Process manipulation ritual", RuneCategory::SYSTEM,
        [this](const auto& args) {
            if (!validateRitual({"ᛉ", "ᚹ", "ᚦ"})) {
                std::cout << "Ritual requires shield, ward, and power runes" << std::endl;
                return;
            }
            displayRuneEffect(runeEffects["ᛟ"]);
            monitor->processControl(args);
        }, true, {"ᛉ", "ᚹ", "ᚦ"});

    // Memory Operations
    registerCommand("memory", "ᛗ", "Memory analysis ritual", RuneCategory::SYSTEM,
        [this](const auto& args) {
            if (!validateRitual({"ᛉ", "ᚹ", "ᚦ"})) {
                std::cout << "Ritual requires shield, ward, and power runes" << std::endl;
                return;
            }
            displayRuneEffect(runeEffects["ᛗ"]);
            monitor->memoryAnalysis(args);
        }, true, {"ᛉ", "ᚹ", "ᚦ"});

    // Network Gateway
    registerCommand("gateway", "ᚷ", "Network gateway ritual", RuneCategory::NAVIGATION,
        [this](const auto& args) {
            if (!validateRitual({"ᛉ", "ᚹ", "ᚦ", "ᛈ"})) {
                std::cout << "Ritual requires shield, ward, power, and path runes" << std::endl;
                return;
            }
            displayRuneEffect(runeEffects["ᚷ"]);
            monitor->controlGateway(args);
        }, true, {"ᛉ", "ᚹ", "ᚦ", "ᛈ"});

    // Trace Operations
    registerCommand("trace", "ᛏ", "System trace ritual", RuneCategory::SYSTEM,
        [this](const auto& args) {
            if (!validateRitual({"ᛉ", "ᚹ", "ᚦ"})) {
                std::cout << "Ritual requires shield, ward, and power runes" << std::endl;
                return;
            }
            displayRuneEffect(runeEffects["ᛏ"]);
            monitor->startTrace(args);
        }, true, {"ᛉ", "ᚹ", "ᚦ"});

    // Combined Rituals
    registerCommand("bind", "ᛒ", "Bind multiple runes", RuneCategory::RITUAL,
        [this](const auto& args) {
            if (args.size() < 2) {
                std::cout << "Usage: bind <primary_rune> <supporting_runes...>" << std::endl;
                return;
            }
            performBindingRitual(args[0], std::vector<std::string>(args.begin() + 1, args.end()));
        });
}

void GhostTerminal::displayRuneEffect(const RuneEffect& effect) {
    // Save cursor position
    std::cout << "\033[s";
    
    // Get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    // Calculate center position
    int centerX = w.ws_col / 2;
    int centerY = w.ws_row / 2;
    
    // Display animation frames
    for (const auto& frame : effect.frames) {
        // Move to center
        std::cout << "\033[" << centerY << ";" << centerX - frame.length()/2 << "H";
        
        // Set color if glowing
        if (effect.glowing) {
            uint8_t r = (effect.color >> 16) & 0xFF;
            uint8_t g = (effect.color >> 8) & 0xFF;
            uint8_t b = effect.color & 0xFF;
            std::cout << "\033[38;2;" << (int)r << ";" << (int)g << ";" << (int)b << "m";
        }
        
        // Display frame
        std::cout << frame << std::flush;
        
        // Reset color
        std::cout << "\033[0m";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(effect.duration));
    }
    
    // Restore cursor position
    std::cout << "\033[u";
}

void GhostTerminal::performRuneRitual(const std::string& primaryRune, 
                        const std::vector<std::string>& supportingRunes) {
    if (!validateRitual(supportingRunes)) {
        std::cout << "Invalid rune combination for ritual" << std::endl;
        return;
    }

    // Display ritual circle with protective runes
    displayRuneCircle();
    
    std::cout << "\nBeginning protective ritual..." << std::endl;
    
    // Activate protection runes first
    displayRuneEffect(runeEffects["ᛉ"]); // Shield
    displayRuneEffect(runeEffects["ᚹ"]); // Ward
    
    // Activate supporting runes in sequence
    for (const auto& rune : supportingRunes) {
        displayRuneEffect(runeEffects[rune]);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    // Activate primary rune with enhanced effect
    RuneEffect enhancedEffect = runeEffects[primaryRune];
    enhancedEffect.duration *= 2;
    enhancedEffect.glowing = true;
    displayRuneEffect(enhancedEffect);
    
    // Execute ritual effect
    auto it = commands.find(primaryRune);
    if (it != commands.end() && it->second.requiresRitual) {
        std::cout << "\nRitual circle established. Executing " << it->second.name << " ritual...\n" << std::endl;
        it->second.handler(supportingRunes);
    }
}

void GhostTerminal::displayRuneCircle() {
    // Clear screen
    clearScreen();
    
    // Get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    // Calculate circle parameters
    int centerX = w.ws_col / 2;
    int centerY = w.ws_row / 2;
    int radius = std::min(w.ws_col, w.ws_row) / 4;
    
    // Draw circle with runes
    for (double angle = 0; angle < 2 * M_PI; angle += M_PI / 8) {
        int x = centerX + radius * cos(angle);
        int y = centerY + radius * sin(angle);
        
        // Move cursor to position
        std::cout << "\033[" << y << ";" << x << "H";
        
        // Draw rune
        int runeIndex = (int)(angle * 8 / M_PI) % activeRunes.size();
        std::cout << activeRunes[runeIndex];
    }
}

void GhostTerminal::visualizeRuneNetwork() {
    clearScreen();
    
    // Draw network connections between active runes
    std::cout << "Active Rune Network:" << std::endl;
    
    for (size_t i = 0; i < activeRunes.size(); ++i) {
        std::cout << activeRunes[i] << " ";
        for (size_t j = 0; j < activeRunes.size(); ++j) {
            if (i != j) {
                std::cout << "──";
            }
        }
        std::cout << std::endl;
    }
}

void GhostTerminal::showRunePower() {
    std::cout << "Rune Power Levels:" << std::endl;
    
    for (const auto& rune : activeRunes) {
        int power = rand() % 100;  // Simulate power level
        std::cout << rune << " [";
        for (int i = 0; i < 20; ++i) {
            if (i < power/5) {
                std::cout << "█";
            } else {
                std::cout << "░";
            }
        }
        std::cout << "] " << power << "%" << std::endl;
    }
}

void GhostTerminal::registerCommand(const std::string& name, const std::string& runeSymbol,
                      const std::string& description, RuneCategory category,
                      std::function<void(const std::vector<std::string>&)> handler,
                      bool requiresRitual, const std::vector<std::string>& ritualRunes) {
    RuneCommand cmd{runeSymbol, name, description, handler, requiresRitual, ritualRunes, category};
    commands[name] = cmd;
    if (!runeSymbol.empty()) {
        runeAliases[runeSymbol] = name;
    }
}

void GhostTerminal::registerCommand(const std::string& name, 
                                  std::function<void(const std::vector<std::string>&)> handler) {
    RuneCommand cmd;
    cmd.name = name;
    cmd.handler = handler;
    cmd.requiresRitual = false;
    cmd.category = RuneCategory::SYSTEM;
    commands[name] = cmd;
}

void GhostTerminal::initializeRuneEnvironment() {
    // Initialize environment variables and paths
    std::cout << "Initializing Rune environment..." << std::endl;
    
    // Set up initial rune configuration
    activeRunes.clear();
    
    // Initialize monitor
    if (monitor) {
        monitor->initialize();
    }
}

void GhostTerminal::openRuneEditor(const std::string& filename) {
    // TODO: Implement editor functionality
    std::cout << "Opening editor for file: " << filename << std::endl;
}

void GhostTerminal::displayRunicBorder() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    // Top border
    std::cout << "╔";
    for (int i = 0; i < w.ws_col - 2; ++i) {
        std::cout << ((i % 2 == 0) ? "ᚱ" : "═");
    }
    std::cout << "╗\n";
    
    // Side borders
    for (int i = 0; i < w.ws_row - 2; ++i) {
        std::cout << "║";
        std::cout << "\033[" << (i + 1) << ";" << (w.ws_col) << "H║\n";
    }
    
    // Bottom border
    std::cout << "╚";
    for (int i = 0; i < w.ws_col - 2; ++i) {
        std::cout << ((i % 2 == 0) ? "ᚱ" : "═");
    }
    std::cout << "╝\n";
}

void GhostTerminal::displayBanner() {
    std::cout << R"(
    ╔═══════════════════════════════════════╗
    ║           GhostC Rune Terminal        ║
    ║      Type 'help' or '?' for help      ║
    ╚═══════════════════════════════════════╝
    )" << std::endl;
}

void GhostTerminal::displayPrompt() {
    std::cout << "\033[32mghost@ghostc\033[0m:\033[34m" << currentDirectory << "\033[0m$ ";
}

void GhostTerminal::processCommand(const std::string& cmdLine) {
    if (cmdLine.empty()) return;

    // Check if it's a rune symbol
    if (isRuneCommand(cmdLine)) {
        std::string command = getRuneSymbol(cmdLine);
        if (!command.empty()) {
            auto args = parseCommand(cmdLine.substr(1)); // Skip the rune symbol
            commands[command].handler(args);
            return;
        }
    }

    // Regular command processing
    auto args = parseCommand(cmdLine);
    if (args.empty()) return;

    std::string cmd = args[0];
    args.erase(args.begin());

    auto it = commands.find(cmd);
    if (it != commands.end()) {
        it->second.handler(args);
    } else {
        std::cout << "ghost: command not found: " << cmd << std::endl;
        std::cout << "Try using a rune command or 'help' for available commands" << std::endl;
    }
}

std::vector<std::string> GhostTerminal::parseCommand(const std::string& cmdLine) {
    std::vector<std::string> args;
    std::stringstream ss(cmdLine);
    std::string arg;
    while (ss >> arg) {
        args.push_back(arg);
    }
    return args;
}

bool GhostTerminal::isRuneCommand(const std::string& input) {
    if (input.empty()) return false;
    return runeAliases.find(input.substr(0, 1)) != runeAliases.end();
}

std::string GhostTerminal::getRuneSymbol(const std::string& command) {
    if (command.empty()) return "";
    auto it = runeAliases.find(command.substr(0, 1));
    return it != runeAliases.end() ? it->second : "";
}

void GhostTerminal::registerDefaultCommands() {
    // Clear screen
    registerCommand("clear", [this](const auto&) { clearScreen(); });

    // Exit
    registerCommand("exit", [this](const auto&) { 
        std::cout << "Shutting down GhostC OS..." << std::endl;
        isRunning = false; 
    });

    // Help
    registerCommand("help", [](const auto&) {
        std::cout << "Available commands:\n"
                  << "  clear    - Clear the screen\n"
                  << "  exit     - Exit GhostC OS\n"
                  << "  help     - Display this help message\n"
                  << "  ls       - List directory contents\n"
                  << "  cd       - Change directory\n"
                  << "  pwd      - Print working directory\n";
    });

    // PWD
    registerCommand("pwd", [this](const auto&) {
        std::cout << currentDirectory << std::endl;
    });

    // CD
    registerCommand("cd", [this](const auto& args) {
        if (args.empty()) {
            currentDirectory = "/";
        } else {
            // Simple path handling - could be enhanced
            if (args[0] == "..") {
                size_t pos = currentDirectory.find_last_of('/');
                if (pos != 0) {
                    currentDirectory = currentDirectory.substr(0, pos);
                }
            } else if (args[0][0] == '/') {
                currentDirectory = args[0];
            } else {
                if (currentDirectory == "/") {
                    currentDirectory += args[0];
                } else {
                    currentDirectory += "/" + args[0];
                }
            }
        }
    });

    // LS
    registerCommand("ls", [](const auto&) {
        std::cout << "Directory listing not implemented yet." << std::endl;
    });
}

void GhostTerminal::shutdown() {
    clearScreen();
    std::cout << "\033[32mShutting down GhostC OS...\033[0m" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    clearScreen();
}

void GhostTerminal::startBootSequence() {
    clearScreen();
    std::cout << "\033[32m"; // Set color to green
    
    // Boot animation
    std::vector<std::string> bootSequence = {
        "Initializing GhostC Terminal...",
        "Loading Rune Protection Systems...",
        "Activating Runic Barriers...",
        "Establishing Secure Environment...",
        "Calibrating Ritual Circles...",
        "GhostC Terminal Ready."
    };

    for (const auto& message : bootSequence) {
        std::cout << message << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout << "\033[0m"; // Reset color
    std::cout << std::endl;
}

bool GhostTerminal::validateRitual(const std::vector<std::string>& runes) {
    // Check for required protection runes
    bool hasShield = false;
    bool hasWard = false;
    
    for (const auto& rune : runes) {
        if (rune == "ᛉ") hasShield = true;
        if (rune == "ᚹ") hasWard = true;
    }
    
    // All rituals require basic protection
    if (!hasShield || !hasWard) {
        std::cout << "Ritual requires basic protection runes (shield ᛉ and ward ᚹ)" << std::endl;
        return false;
    }
    
    return true;
}

void GhostTerminal::displayRitualStatus() {
    std::cout << "\nActive Protection Status:\n";
    std::cout << "Shield (ᛉ): " << (monitor->isShieldActive() ? "Active" : "Inactive") << "\n";
    std::cout << "Ward (ᚹ): " << (monitor->isWardActive() ? "Active" : "Inactive") << "\n";
    
    std::cout << "\nSystem Analysis:\n";
    auto vulnerabilities = monitor->getSystemVulnerabilities();
    for (const auto& vuln : vulnerabilities) {
        std::cout << "- " << vuln << "\n";
    }
    
    std::cout << "\nNetwork Status:\n";
    auto networkStatus = monitor->getNetworkStatus();
    for (const auto& status : networkStatus) {
        std::cout << "- " << status << "\n";
    }
}

void GhostTerminal::performBindingRitual(const std::string& primaryRune, 
                            const std::vector<std::string>& supportingRunes) {
    if (!validateRitual(supportingRunes)) {
        std::cout << "Invalid rune combination for binding ritual" << std::endl;
        return;
    }

    std::cout << "\nInitiating binding ritual..." << std::endl;
    
    // Create binding circle
    displayRuneCircle();
    
    // Activate each rune in sequence
    for (const auto& rune : supportingRunes) {
        displayRuneEffect(runeEffects[rune]);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    // Bind runes together
    std::cout << "\nBinding runes: ";
    for (const auto& rune : supportingRunes) {
        std::cout << rune << " ";
    }
    std::cout << "to " << primaryRune << std::endl;
    
    // Create combined effect
    RuneEffect bindingEffect = runeEffects[primaryRune];
    bindingEffect.duration *= 2;
    bindingEffect.glowing = true;
    displayRuneEffect(bindingEffect);
    
    // Register bound runes
    monitor->registerBoundRunes(primaryRune, supportingRunes);
}

} // namespace RuneLang
