/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichpakov <ichpakov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 11:17:59 by ichpakov          #+#    #+#             */
/*   Updated: 2025/06/20 11:53:37 by ichpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//CLASSE DE BASE DU SERVEUR

#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>

class Server {
    private:
        int port;
        int serverSocket;
        bool isRunning;
        void handle_client(int client_socket);
    public:
        Server(int port);
        void start();
};
