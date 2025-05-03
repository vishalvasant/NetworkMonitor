// Add this method to the existing NetworkMonitor.cpp file

bool NetworkMonitor::validateBPFFilter(const std::string& filter, QString& errorMessage) const
{
    // Create a temporary pcap handler for validation
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t* pcap = pcap_open_dead(DLT_EN10MB, 65535);
    
    if (!pcap) {
        errorMessage = QString("Failed to initialize filter validator");
        return false;
    }
    
    bpf_program program;
    int result = pcap_compile(pcap, &program, filter.c_str(), 1, PCAP_NETMASK_UNKNOWN);
    
    if (result != 0) {
        errorMessage = QString(pcap_geterr(pcap));
        pcap_close(pcap);
        return false;
    }
    
    // Free resources
    pcap_freecode(&program);
    pcap_close(pcap);
    
    return true;
}

// Modify the existing setFilter method to include additional error handling
void NetworkMonitor::setFilter(const std::string& filter)
{
    if (!running_) {
        filter_ = filter;
        return;
    }

    // If the monitor is running, we need to apply the filter directly
    if (!filter.empty()) {
        struct bpf_program fp;
        if (pcap_compile(pcap_handle_, &fp, filter.c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1) {
            throw std::runtime_error("Failed to compile filter: " + std::string(pcap_geterr(pcap_handle_)));
        }
        
        if (pcap_setfilter(pcap_handle_, &fp) == -1) {
            pcap_freecode(&fp);
            throw std::runtime_error("Failed to set filter: " + std::string(pcap_geterr(pcap_handle_)));
        }
        
        pcap_freecode(&fp);
    }
    
    filter_ = filter;
    Logger::info("Applied filter: " + filter);
}