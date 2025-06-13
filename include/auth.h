#ifndef AUTH_H
#define AUTH_H

bool authenticate_user(int client_socket, std::string &username);
bool register_user(int client_socket);

#endif
