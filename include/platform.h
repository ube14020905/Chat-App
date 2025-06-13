#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define CLOSESOCKET closesocket
    #define SOCKET_INIT() \
        WSADATA wsaData; \
        WSAStartup(MAKEWORD(2, 2), &wsaData);
    #define SOCKET_CLEANUP() WSACleanup()
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #define CLOSESOCKET close
    #define SOCKET_INIT()
    #define SOCKET_CLEANUP()
    #define INVALID_SOCKET -1
#endif

#endif
