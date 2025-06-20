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

class Server {
    private:
        int port_;
        void handle_client(int client_socket);
    public:
        Server(int port);
        void start();
};
