#include "client.h"
#include "platform.h"
#include <iostream>
#include <thread>
#include <string>
#include <cstring>

void receive_messages(int sock) {
    char buffer[1024];
    while (true) {
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << ">> " << buffer << std::endl;
        }
    }
}

void start_client() {
    SOCKET_INIT();

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
#ifdef _WIN32
    InetPton(AF_INET, L"127.0.0.1", &server_addr.sin_addr);
#else
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
#endif

    connect(sock, (sockaddr*)&server_addr, sizeof(server_addr));

    std::thread(receive_messages, sock).detach();

    std::string msg;
    while (true) {
        std::getline(std::cin, msg);
        send(sock, msg.c_str(), msg.length(), 0);
    }

    SOCKET_CLEANUP();
}
