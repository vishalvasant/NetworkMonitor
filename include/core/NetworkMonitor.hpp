#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <pcap.h>
#include <QString>

class NetworkMonitor {
public:
    NetworkMonitor(const std::string& interface);
    ~NetworkMonitor();

    void start();
    void stop();
    void setFilter(const std::string& filter);

    // New method for BPF filter validation
    bool validateBPFFilter(const std::string& filter, QString& errorMessage) const;

private:
    std::string interface_;
    std::string filter_;
    pcap_t* pcap_handle_;
    
    std::thread capture_thread_;
    std::thread process_thread_;
    std::thread analyze_thread_;
    std::thread store_thread_;
    
    std::atomic<bool> running_{false};

    void captureThread();
    void processThread();
    void analyzeThread();
    void storeThread();
};