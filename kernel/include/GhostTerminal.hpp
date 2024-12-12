#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <map>
#include <chrono>
#include <unordered_map>
#include "RuneLogger.hpp"
#include "RuneSystem.hpp"
#include "RuneParser.hpp"
#include "RuneMonitor.hpp"

namespace RuneLang {

// Rune Categories
enum class RuneCategory {
    PROTECTION,    // Protection and security
    SYSTEM,        // System operations
    NAVIGATION,    // File and directory navigation
    EDITOR,        // Text editing
    EXECUTION,     // Script and program execution
    MEMORY,        // Memory management
    NETWORK,       // Network operations
    RITUAL,        // Special combined operations
    BINDING,       // Resource binding
    VISION        // Display and graphics
};

// Rune command structure
struct RuneCommand {
    std::string runeSymbol;      // Unicode rune symbol
    std::string name;            // Command name
    std::string description;     // Command description
    std::function<void(const std::vector<std::string>&)> handler;
    bool requiresRitual;         // Whether this rune needs ritual activation
    std::vector<std::string> ritualRunes;  // Runes needed for ritual
    RuneCategory category;       // Command category
};

// Rune visual effects
struct RuneEffect {
    std::string rune;
    std::vector<std::string> frames;
    int duration;  // milliseconds
    bool glowing;
    uint32_t color;
};

class GhostTerminal {
public:
    GhostTerminal();
    void initialize();
    void processCommand(const std::string& cmdLine);
    void shutdown();
    bool isRunning;

private:
    void registerRuneEffects();
    void registerRuneCommands();
    void registerDefaultCommands();
    void displayRuneEffect(const RuneEffect& effect);
    void performRuneRitual(const std::string& primaryRune, const std::vector<std::string>& supportingRunes);
    void displayRuneCircle();
    void visualizeRuneNetwork();
    void showRunePower();
    void displayRunicBorder();
    void displayBanner();
    void displayPrompt();
    void displayRitualStatus();
    void performBindingRitual(const std::string& primaryRune, const std::vector<std::string>& supportingRunes);
    
    void registerCommand(const std::string& name, const std::string& runeSymbol = "",
                        const std::string& description = "", RuneCategory category = RuneCategory::SYSTEM,
                        std::function<void(const std::vector<std::string>&)> handler = nullptr,
                        bool requiresRitual = false, const std::vector<std::string>& ritualRunes = {});
    
    void registerCommand(const std::string& name, 
                        std::function<void(const std::vector<std::string>&)> handler);
    
    void openRuneEditor(const std::string& filename);
    void processCommand(const std::string& input);
    std::vector<std::string> parseCommand(const std::string& cmdLine);
    bool isRuneCommand(const std::string& input);
    std::string getRuneSymbol(const std::string& command);
    bool validateRitual(const std::vector<std::string>& runes);
    
    std::unordered_map<std::string, RuneCommand> commands;
    std::unordered_map<std::string, std::string> runeAliases;
    std::unordered_map<std::string, RuneEffect> runeEffects;
    std::vector<std::string> activeRunes;
    std::unique_ptr<RuneMonitor> monitor;
    bool isBinding;
    std::string currentDirectory;
};

} // namespace RuneLang
