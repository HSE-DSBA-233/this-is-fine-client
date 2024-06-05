#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <string>
#include <chat-api.h>
#include <json.hpp>

extern ChatClient chatclient;

void initializeChatClient(const std::string& filename);

#endif // CHATCLIENT_H
