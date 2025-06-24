/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichpakov <ichpakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:18:38 by ichpakov          #+#    #+#             */
/*   Updated: 2025/06/20 11:59:26 by ichpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/server.hpp"

Server::Server(int port_) : port(port_)
{
}

void Server::handle_client(int clientSocket)
{
    const int bufferSize = 1024;
    char buffer[bufferSize];
    memset(buffer, 0, bufferSize);

    int bytesRead = read(clientSocket, buffer, bufferSize - 1);
    if (bytesRead < 0) {
        perror("read");
        return;
    }

    printf("Requête reçue :\n%s\n", buffer);
    const char* httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, world!";
    
    int bytesSent = write(clientSocket, httpResponse, strlen(httpResponse));
    if (bytesSent < 0) {
        perror("write");
        return;
    }
}

void Server::start()
{
    struct sockaddr_in serverAddr;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("socket");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(serverSocket, 5) < 0) {
        perror("listen");
        exit(1);
    }

    isRunning = true;
    while (isRunning) {
        int clientSocket;
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);

        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            perror("accept");
            continue;
        }

        handle_client(clientSocket);
        close(clientSocket);
    }

    close(serverSocket);
}
