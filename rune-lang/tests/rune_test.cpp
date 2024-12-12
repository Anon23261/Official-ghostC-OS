#include <cassert>
#include "RuneError.hpp"
#include "RuneMonitor.hpp"
#include "RuneLogger.hpp"
#include "GhostSystem.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using namespace RuneLang;

void testSystemInitialization() {
    GhostSystem& system = GhostSystem::getInstance();
    assert(system.initialize() == true);
    assert(system.isInitialized() == true);
    system.shutdown();
    assert(system.isInitialized() == false);
}

void testProcessManagement() {
    GhostSystem& system = GhostSystem::getInstance();
    system.initialize();

    GhostProcess process;
    process.setPriority(10);
    assert(process.getPriority() == 10);

    process.setAffinity(1);
    assert(process.getAffinity() == 1);

    system.shutdown();
}

void testMemoryManagement() {
    GhostSystem& system = GhostSystem::getInstance();
    system.initialize();

    size_t allocated = system.allocateMemory(1024);
    assert(allocated == 1024);
    // Check if free memory is at least the size we just freed
    assert(system.getFreeMemory() >= allocated);
    system.freeMemory(allocated);

    system.shutdown();
}

void testMonitoring() {
    RuneMonitor monitor;
    
    // Initialize the monitor
    monitor.initialize();
    
    // Test basic protection features
    monitor.startProtection();
    assert(monitor.isShieldActive() == true);
    
    monitor.enableBarrier();
    assert(monitor.isWardActive() == true);
    
    // Test system analysis
    monitor.startSystemAnalysis();
    assert(monitor.isAnalysisRunning() == true);
    
    // Wait a bit for analysis to run
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // Get status information
    auto vulnerabilities = monitor.getSystemVulnerabilities();
    auto networkStatus = monitor.getNetworkStatus();
    
    // Stop everything
    monitor.stopProtection();
    assert(monitor.isShieldActive() == false);
    assert(monitor.isWardActive() == false);
}

void testErrorHandling() {
    try {
        throw RuneError(RuneError::ErrorCode::SYSTEM_ERROR, "Test error");
        assert(false && "Should not reach here");
    } catch (const RuneError& e) {
        assert(e.getCode() == RuneError::ErrorCode::SYSTEM_ERROR);
    }
}

void testLogging() {
    LogLevelUtils::setLevel(LogLevel::DEBUG);
    LOG_DEBUG("Debug message");
    LOG_INFO("Info message");
    LOG_WARNING("Warning message");
    LOG_ERROR("Error message");
    LOG_CRITICAL("Critical message");
}

int main() {
    std::cout << "Running Rune tests..." << std::endl;

    // Initialize logging
    LogLevelUtils::setLevel(LogLevel::DEBUG);
    RuneLogger::getInstance().setLogFile("rune_test.log");

    try {
        testSystemInitialization();
        std::cout << "System initialization test passed" << std::endl;

        testProcessManagement();
        std::cout << "Process management test passed" << std::endl;

        testMemoryManagement();
        std::cout << "Memory management test passed" << std::endl;

        testMonitoring();
        std::cout << "Monitoring test passed" << std::endl;

        testErrorHandling();
        std::cout << "Error handling test passed" << std::endl;

        testLogging();
        std::cout << "Logging test passed" << std::endl;

        std::cout << "\nAll tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with error: " << e.what() << std::endl;
        return 1;
    }
}
