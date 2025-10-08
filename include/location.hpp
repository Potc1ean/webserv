/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njeanbou <njeanbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:54:59 by njeanbou          #+#    #+#             */
/*   Updated: 2025/10/01 15:06:01 by njeanbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>
#include <map>

typedef struct s_location
{
    std::string loc;
    std::string allow_methods;
    int redirCode;
    std::string redirHTTP; // status_code + taget_url
    std::string root;
    std::string upload_store;
    bool directory_listing;
    bool upload_enable;
    bool cgi_extension;
    std::map<int, std::string> error_page;
    std::vector<std::string> index;
}   t_location;
