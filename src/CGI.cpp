/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njeanbou <njeanbou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 17:53:14 by ichpakov          #+#    #+#             */
/*   Updated: 2025/10/01 17:53:14 by njeanbou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGI.hpp"

// CONSTRUCTORS

CGI::CGI(const Request& request, const std::string root) : request(request)
{
    setup_env_var();
	script_filename = root + script_name;
	std::cout << "var env set\n";
	std::cout << query_string << " " << script_filename << " " << script_name << std::endl;
	execute();
}

// CGI::CGI()
// {

// }

CGI::~CGI()
{

}

// PRIVATE

int ft_stoi(std::string s)
{
    int n = 0;
    for (size_t i = 0; i < s.length(); ++i)
    {
        if (isdigit(s[i]))
            n = n * 10 + (s[i] - '0');
        else 
            return 0;
    }
    return n;
}

void CGI::buildEnv()
{
    envStrings.push_back("REQUEST_METHOD=" + request.get_method()); 
    envStrings.push_back("QUERY_STRING=" + query_string); // infos apres le ?
    envStrings.push_back("SCRIPT_FILENAME=" + script_filename); //chemin absolue du fichier cgi - en gros le fichier php - faut le composer
    envStrings.push_back("SCRIPT_NAME=" + script_name); //chemin du script dans l'url
    envStrings.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envStrings.push_back("GATEWAY_INTERFACE=CGI/1.1");

	std::cout << "env cgi :\n";
	for (int i = 0; i < 6; ++i)
		std::cout << envStrings[i] << std::endl;
    if (request.get_method() == "POST")
	{
		std::ostringstream oss;
		oss << request.get_body().size();
        envStrings.push_back("CONTENT_LENGTH=" + oss.str());
        envStrings.push_back("CONTENT_TYPE=" + content_type);
    }

    envStrings.push_back("REDIRECT_STATUS=200");

    for (size_t i = 0; i < envStrings.size(); ++i)
        envp.push_back(const_cast<char*>(envStrings[i].c_str()));
    envp.push_back(NULL);
}

void CGI::setupAndRun()
{
    int stdout_pipe[2];
    int stdin_pipe[2];
    if (pipe(stdout_pipe) < 0 || pipe(stdin_pipe) < 0)
        return;

    pid_t pid = fork();
    if (pid == 0)
    {
        // Rediriger stdout vers le pipe d'écriture
        dup2(stdout_pipe[1], STDOUT_FILENO);
        // Rediriger stdin vers le pipe de lecture
        dup2(stdin_pipe[0], STDIN_FILENO);

        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        close(stdin_pipe[0]);
        close(stdin_pipe[1]);

        chdir("/www/cgi-bin");

        char *argv[] = { const_cast<char*>("/usr/bin/php-cgi"), NULL};
        execve("/usr/bin/php-cgi", argv, envp.data());
        exit(1);
    }
    else
    {
        close(stdout_pipe[1]);
        close(stdin_pipe[0]);

        std::string body = request.get_body();
        if (!body.empty())
            write(stdin_pipe[1], body.c_str(), body.size());
        close(stdin_pipe[1]);

        const int TIMEOUT_MS = 5000;
        const int POLL_STEP_MS = 100;
        const int MAX_ITER = TIMEOUT_MS / POLL_STEP_MS;

        char buffer[4096];
        struct pollfd pfd;
        pfd.fd = stdout_pipe[0];
        pfd.events = POLLIN | POLLHUP;

        bool timed_out = false;

        for (int i = 0; i < MAX_ITER; ++i)
        {
            int ret = poll(&pfd, 1, POLL_STEP_MS);
            if (ret > 0)
            {
                if (pfd.revents & POLLIN)
                {
                    ssize_t bytes = read(stdout_pipe[0], buffer, sizeof(buffer));
                    if (bytes > 0)
                        cgiOutput.append(buffer, bytes);
                    else if (bytes == 0)
                        break;
                }
                else if (pfd.revents & POLLHUP)
                {
                    break;
                }
            }
            else if (ret == 0)
            {
                int status;
                pid_t result = waitpid(pid, &status, WNOHANG);
                if (result == pid)
                {
                    break;
                }
            }
            else if (ret < 0 && errno != EINTR)
            {
                break;
            }
        }

        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);
        if (result == 0)
        {
            kill(pid, SIGKILL);
            waitpid(pid, &status, 0);
            timed_out = true;
        }
        close(stdout_pipe[0]);
        if (timed_out)
            cgiOutput = "Status: 504 Gateway Timeout\r\nContent-Type: text/plain\r\n\r\nCGI script timed out.";
    }
}

// PUBLIC METHODES



int CGI::execute()
{
    buildEnv();
    setupAndRun();
    return 0;
}

std::string CGI::getOutput() const
{
    return cgiOutput;
}

int CGI::getError() const
{
    std::string output(getOutput());
    size_t pos = output.find('\n');
    std::string line = output.substr(0, pos);

    if (line.find("Status:") != std::string::npos)
    {
        std::stringstream ss(line);
        std::string word;
        while (ss >> word)
        {
            if (isdigit(word[0]))
            {
                int nbr = ft_stoi(word);
                return nbr;
            }
        }
    }
    return 0;
}

void CGI::setup_env_var()
{
	std::cout << "setup_env_var\n";
	std::string raw = request.get_raw_request();
	std::cout << "RAW : " << raw << std::endl << std::endl;

	size_t pos_method = raw.find(request.get_method());
	size_t pos_http = raw.find(" HTTP/");
	size_t start_path = pos_method + request.get_method().length() + 1;
	size_t path_length = pos_http - start_path;

	std::string full_path = raw.substr(start_path, path_length);
	std::cout << "Full path: " << full_path << std::endl;

	size_t pos_qs = full_path.find("?");
	if (pos_qs != std::string::npos)
	{
		script_name = full_path.substr(0, pos_qs);
		query_string = full_path.substr(pos_qs + 1);
	}
	else
	{
		script_name = full_path;
		query_string = "";
	}

	std::cout << "Script Name : " << script_name << std::endl;
	std::cout << "Query String : " << query_string << std::endl;

	size_t pos_ct = raw.find("Content-Type: ");
	if (pos_ct != std::string::npos)
		content_type = raw.substr(pos_ct + 14, raw.find("\r\n", pos_ct) - (pos_ct + 14));
	else
		content_type = "";

	std::cout << "Content Type : " << content_type << std::endl;
}


/*
GET /cgi-bin/hello.py?name=Nathan HTTP/1.1
Host: localhost:8080
User-Agent: curl/8.0
Accept:

POST /cgi-bin/form_handler.py HTTP/1.1
Host: localhost:8080
Content-Type: application/x-www-form-urlencoded
Content-Length: 17

username=Nathan42
*/

// GET /cgi-bin/index.php?name=Alice HTTP/1.1

/* dans la request il me faut :
*   method :            on l'a
*   query_string : information apres le ? (ex: name=Alice)
*   script_name : chemin indiquer dans le get ou post (ex: /cgi-bin/index.php)
*   script_filename : chemin des executables du conf + script_name (ex: www/cgi-bin/index.php)
*   body :              on l'a
*   Content_type : variable dans la requete, faut juste la ligne brut comme elle est dans la requete.
*/

/* infos :
*   erreur 500 a gerer apres l'exec du cgi :
*/
