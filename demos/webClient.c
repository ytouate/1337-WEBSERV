#include "../inc.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void parse_url(char *url, char **hostname, char **port, char **path)
{
    printf("%s\n", url);
    char *p;
    p = strstr(url, "://");
    char *protocol = 0;
    if (p)
    {
        protocol = url;
        *p = 0;
        p += 3;
    }
    else
    {
        p = url;
    }
    if (protocol)
    {
        if (strcmp(protocol, "http"))
        {
            fprintf(stderr,
                    "Unknown protocol '%s'. Only 'http' is supported.\n",
                    protocol);
            exit(1);
        }
    }
    *hostname = p;
    while (*p && *p != ':' && *p != '/' && *p != '#')
        ++p;
    *port = "80";
    if (*p == ':')
    {
        *p++ = 0;
        *port = p;
    }
    while (*p && *p != '/' && *p != '#')
        ++p;
    *path = p;
    if (*p == '/')
    {
        *path = p + 1;
    }
    *p = 0;
    while (*p && *p != '#')
        ++p;
    if (*p == '#')
        *p = 0;
    printf("hostname: %s\n", *hostname);
    printf("port: %s\n", *port);
    printf("path: %s\n", *path);
}

void send_request(int s, char *hostname, char *port, char *path)
{
    char buffer[4096];
    sprintf(buffer, "GET /%s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "User-Agent: honpwc web_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");
    send(s, buffer, strlen(buffer), 0);
    printf("Sent Headers:\n%s", buffer);
}
int main(int ac, char **av)
{
    int a[10];

    memset(&a, -1, sizeof(int) * 10);
    for (int i = 0; i < 10; ++i)
        printf("%d\n", a[i]);
    return 0;
    char *path;
    char *port;
    char *host;
    parse_url(strdup("http://www.example.com:80/res/page1.php?user=bob#account"),
              &host, &port, &path);
}