#include "server.h"
#include "auth.h"
#include "loadMsg.h"
#include "dbconnect.h"
#include "platform.h"
#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <mutex>
#include <cstring>

std::map<std::string, int> active_users;
std::mutex mtx;

void handle_client(int client_socket) {
    std::string username;
    if (!authenticate_user(client_socket, username)) {
        CLOSESOCKET(client_socket);
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        active_users[username] = client_socket;
    }

    load_offline_messages(username, client_socket);

    char buffer[1024];
    while (true) {
        int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';

        std::string msg(buffer);
        std::string target = msg.substr(0, msg.find(':'));
        std::string content = msg.substr(msg.find(':') + 1);

        int target_socket = -1;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (active_users.count(target)) {
                target_socket = active_users[target];
            }
        }

        MYSQL* conn = connect_db();
        std::string query = "INSERT INTO messages(sender, receiver, message, delivered) VALUES('" +
                            username + "', '" + target + "', '" + content + "', ";

        if (target_socket != -1) {
            send(target_socket, content.c_str(), content.length(), 0);
            query += "1)";
        } else {
            query += "0)";
        }
        mysql_query(conn, query.c_str());
        mysql_close(conn);
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        active_users.erase(username);
    }

    CLOSESOCKET(client_socket);
}

void start_server() {
    SOCKET_INIT();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);
    std::cout << "Server listening on port 8080...\n";

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        std::thread(handle_client, client_fd).detach();
    }

    SOCKET_CLEANUP();
}
