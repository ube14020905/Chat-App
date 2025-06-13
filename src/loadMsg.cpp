#include "loadMsg.h"
#include "dbconnect.h"
#include <mysql/mysql.h>
#include <unistd.h>
#include <iostream>

void load_offline_messages(const std::string &username, int client_socket) {
    MYSQL* conn = connect_db();
    std::string query = "SELECT sender, message, timestamp FROM messages WHERE receiver='" + username + "' AND delivered=0";
    mysql_query(conn, query.c_str());
    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        std::string msg = "[From " + std::string(row[0]) + "] " + std::string(row[1]) + " (" + std::string(row[2]) + ")\n";
        send(client_socket, msg.c_str(), msg.length(), 0);
    }

    std::string update = "UPDATE messages SET delivered=1 WHERE receiver='" + username + "' AND delivered=0";
    mysql_query(conn, update.c_str());
    mysql_free_result(res);
    mysql_close(conn);
}
