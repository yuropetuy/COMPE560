#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    char *ip;
    int port;
    int error;

    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        // WSAStartup failed
        return 1;
    }

    // Take in arguments ip address and port and assign them
    if (argc < 2)
    {
        printf("No arguments passed...\n");
        return 0;
    }
    else if (argc < 3)
    {
        printf("Not enough arguments passed...\n");
        return 0;
    }
    else
    {
        printf("Arguments passed!\n");
        ip = argv[1];         // Localhost ip address
        port = atoi(argv[2]); // port number server is listening through this port

        printf("ip: %s\nport: %d\n", ip, port);
    }

    int server_sock, client_sock;
    int val_read;
    char str[100];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[4096]; // 4096 Byte Buffer

    // Create the socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket generation error\n");
        exit(1);
    }
    printf("Socket created\n");

    // memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    printf("Server IP: %s, Port: %d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
    // Binding socket to designated port
    if ((bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0)
    {
        error = WSAGetLastError();
        printf("Binding failed. Error: %d", error);
        exit(1);
    }

    printf("Bound to port number: %d\n", port);

    // Listen through binded port
    if (listen(server_sock, 3) < 0)
    {
        perror("Listening error...\n");
        exit(1);
    }
    printf("Listening...\n");

    // Accepting a connection request with client
    addr_size = sizeof(client_addr);
    if (client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size) < 0)
    {
        perror("Accepting error...\n");
        exit(1);
    }
    printf("Client Connected\n");

    // Reading data sent from the client
    val_read = recv(client_sock, buffer, sizeof(buffer), 0);

    printf("String received from client: %s\n", str);

    int i, j, temp;
    int l = strlen(str);

    // Loop for reversing the string
    for (i = 0, j = l - 1; i < j; i++, j--)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }

    // Send the reversed string back to the client
    send(client_sock, str, sizeof(str), 0);
    printf("Reversed string sent back to client\n");

    close(server_sock);

    WSACleanup();

    return 0;
}
