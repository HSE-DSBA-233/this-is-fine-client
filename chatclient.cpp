#include "chatclient.h"
#include <fstream>
#include <iostream>

std::string base_url = "";

void initializeChatClient(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string ip_address;
    std::getline(file, ip_address);
    file.close();

    if (ip_address.empty()) {
        std::cerr << "IP address is empty in file: " << filename << std::endl;
        return;
    }

    base_url = "http://" + ip_address + "/chat/";
    chatclient = ChatClient(base_url);
}

ChatClient chatclient(base_url);

struct ChatClientInitializer {
    ChatClientInitializer() {
        initializeChatClient("ip_address.txt");
    }
};

static ChatClientInitializer initializer;
