#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>
#include <ws2tcpip.h>

int main(int argc, char *argv[])
{
    char *ip;
    int port;
    int error;
    int receivedVal;

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

    int client_sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[4096]; // 4096 Byte Buffer
    int n;

    // Create the socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
    {
        perror("Socket generation error\n");
        exit(1);
    }

    printf("Socket created\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    // Requesting connection with server socket
    if (connect(client_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        error = WSAGetLastError();
        printf("Connection failed. Error: %d", error);
        WSACleanup();
        exit(1);
    }

    // Input string to send
    printf("Please type in the message you would like to reverse: ");
    scanf("%s", buffer);

    // Send string
    send(client_sock, buffer, sizeof(buffer), 0);

    // Receive the string sent from the server
    int l = strlen(buffer);
    receivedVal = read(client_sock, buffer, l);

    printf("String received back from server: %s\n", receivedVal);

    // Close socket after reading data
    close(client_sock);
    WSACleanup();

    return 0;
}