#include "auth.h"
#include "dbconnect.h"
#include <mysql/mysql.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

bool authenticate_user(int client_socket, std::string &username) {
    char buffer[1024];
    send(client_socket, "Username: ", 10, 0);
    int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes] = '\0';
    username = buffer;

    send(client_socket, "Password: ", 10, 0);
    bytes = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes] = '\0';
    std::string password = buffer;

    MYSQL* conn = connect_db();
    std::string query = "SELECT * FROM users WHERE username='" + username + "' AND password='" + password + "'";
    mysql_query(conn, query.c_str());
    MYSQL_RES* res = mysql_store_result(conn);
    bool valid = mysql_num_rows(res) > 0;

    if (valid) {
        std::string online = "UPDATE users SET online=1 WHERE username='" + username + "'";
        mysql_query(conn, online.c_str());
    }

    mysql_free_result(res);
    mysql_close(conn);
    return valid;
}
