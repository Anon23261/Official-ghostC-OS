#include "GhostTerminal.hpp"
#include <iostream>
#include <string>
#include <csignal>
#include <cstdlib>

namespace {
    RuneLang::GhostTerminal* terminal = nullptr;

    void signalHandler(int signum) {
        if (terminal) {
            std::cout << "\nReceived signal " << signum << ". Shutting down safely..." << std::endl;
            terminal->shutdown();
        }
        exit(signum);
    }
}

void setupSignalHandlers() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
}

void printUsage() {
    std::cout << "\nGhostC Terminal Usage:\n"
              << "------------------------\n"
              << "Basic Commands:\n"
              << "  help       - Show this help message\n"
              << "  clear      - Clear the screen\n"
              << "  exit       - Exit the terminal\n\n"
              << "Protection Runes:\n"
              << "  ᛉ (shield) - Activate system protection\n"
              << "  ᚹ (ward)   - Create protective barrier\n\n"
              << "System Runes:\n"
              << "  ᛟ (process) - Process control\n"
              << "  ᛗ (memory)  - Memory analysis\n"
              << "  ᚷ (gateway) - Network gateway\n"
              << "  ᛏ (trace)   - System tracing\n\n"
              << "Ritual Commands:\n"
              << "  ritual <rune> <supporting_runes...> - Perform rune ritual\n"
              << "  bind <primary_rune> <runes...>      - Bind runes together\n\n"
              << "Example Rituals:\n"
              << "  ritual process ᛉ ᚹ ᚦ list           - List processes\n"
              << "  ritual memory ᛉ ᚹ ᚦ scan            - Scan memory\n"
              << "  ritual gateway ᛉ ᚹ ᚦ ᛈ status       - Check gateway status\n"
              << "  ritual trace ᛉ ᚹ ᚦ syscalls         - Trace system calls\n"
              << std::endl;
}

int main(int argc, char* argv[]) {
    setupSignalHandlers();
    
    terminal = new RuneLang::GhostTerminal();
    
    // Process command line arguments
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            printUsage();
            delete terminal;
            return 0;
        }
    }
    
    // Start boot sequence
    terminal->startBootSequence();
    
    // Initialize terminal
    terminal->initialize();
    
    // Print initial help
    printUsage();
    
    std::string command;
    while (true) {
        terminal->displayPrompt();
        
        // Handle input
        if (!std::getline(std::cin, command) || command == "exit") {
            break;
        }
        
        if (command == "help") {
            printUsage();
            continue;
        }
        
        try {
            terminal->processCommand(command);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
    terminal->shutdown();
    delete terminal;
    return 0;
}
