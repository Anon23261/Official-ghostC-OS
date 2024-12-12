#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <map>

namespace RuneLang {

class RuneMonitor {
public:
    RuneMonitor();
    virtual ~RuneMonitor();

    // Basic Protection
    void startProtection();
    void stopProtection();
    void enableBarrier();
    void disableBarrier();
    
    // Advanced Security Operations
    void startSystemAnalysis();
    void startNetworkScan();
    void fortifySystem();
    void processControl(const std::vector<std::string>& args);
    void memoryAnalysis(const std::vector<std::string>& args);
    void controlGateway(const std::vector<std::string>& args);
    void startTrace(const std::vector<std::string>& args);
    void registerBoundRunes(const std::string& primary, const std::vector<std::string>& supporting);
    
    // Status Checks
    bool isShieldActive() const { return shieldActive; }
    bool isWardActive() const { return wardActive; }
    bool isAnalysisRunning() const { return analysisRunning; }
    
    std::vector<std::string> getSystemVulnerabilities() const;
    std::vector<std::string> getNetworkStatus() const;
    
private:
    // Protection Status
    std::atomic<bool> shieldActive;
    std::atomic<bool> wardActive;
    std::atomic<bool> analysisRunning;
    
    // Security Monitoring
    std::vector<std::string> vulnerabilities;
    std::vector<std::string> networkStatus;
    mutable std::mutex statusMutex;
    
    // Bound Runes
    std::map<std::string, std::vector<std::string>> boundRunes;
    
    // Monitoring Threads
    std::unique_ptr<std::thread> monitorThread;
    std::unique_ptr<std::thread> analysisThread;
    
    void runSystemAnalysis();
    void scanNetwork();
    void monitorSystem();
    void updateSecurityStatus();
    
    // Protection Methods
    void enforceAccessControl();
    void monitorSystemCalls();
    void checkFileIntegrity();
    void analyzeNetworkTraffic();
    
    // Process Operations
    void listProcesses();
    void inspectProcess(int pid);
    void terminateProcess(int pid);
    
    // Memory Operations
    void scanMemory();
    void mapMemory(uintptr_t address);
    void protectMemory(uintptr_t address);
    
    // Gateway Operations
    void showGatewayStatus();
    void openGateway(int port);
    void closeGateway(int port);
    
    // Trace Operations
    void traceSyscalls(const std::string& filter);
    void traceNetwork(const std::string& filter);
    void traceFileAccess(const std::string& filter);
};

} // namespace RuneLang
