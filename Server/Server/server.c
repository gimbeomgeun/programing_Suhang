#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in server, client;
    int clientAddrLen = sizeof(client);
    char message[1000];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d\n", WSAGetLastError());
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen to incoming connections
    listen(serverSocket, 3);
    printf("Server listening on port 8888...\n");

    // Accept and handle incoming connections
    clientSocket = accept(serverSocket, (struct sockaddr*)&client, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept failed: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    while (1)
    {
        int receivedLine;
        recv(clientSocket, (char*)&receivedLine, sizeof(int), 0);
        printf("%d\n", receivedLine);
    }


    //while (1) {
    //    // Receive data from client
    //    memset(message, 0, sizeof(message));
    //    if (recv(clientSocket, message, sizeof(message), 0) == SOCKET_ERROR) {
    //        printf("recv failed: %d\n", WSAGetLastError());
    //        closesocket(clientSocket);
    //        WSACleanup();
    //        return 1;
    //    }

    //    printf("Client: %s\n", message);

    //    // Send data back to client
    //    printf("Server: ");
    //    gets_s(message, sizeof(message));
    //    send(clientSocket, message, strlen(message), 0);
    //}


    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
