#include "dbconnect.h"
#include <iostream>

MYSQL* connect_db() {
    MYSQL* conn = mysql_init(nullptr);
    if (!mysql_real_connect(conn, "localhost", "root", "your_password", "chatdb", 3306, nullptr, 0)) {
        std::cerr << "MySQL connection failed: " << mysql_error(conn) << "\n";
        exit(1);
    }
    return conn;
}
