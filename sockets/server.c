#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Error handling function
void error(const char *msg)
{
    perror(msg); // Print error message
    exit(1);     // Exit the program with an error code
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];                       // Buffer to store received data
    struct sockaddr_in serv_addr, cli_addr; // Server and client socket address structures
    int n;

    // Check if the user provided a port number
    if (argc < 2)
    {
        fprintf(stderr, "Error: No Port Provided\n");
        exit(1);
    }

    // Create a socket using TCP protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error: Opening Socket");
    }

    // Zero out the server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));

    // Convert the port number from string to integer
    portno = atoi(argv[1]);

    // Set up the server address structure
    serv_addr.sin_family = AF_INET;         // Use IPv4
    serv_addr.sin_port = htons(portno);     // Set the port number (converted to network byte order)
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any network interface

    // Bind the socket to the provided address and port
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error: On Binding");
    }

    // Start listening for incoming connections (max 5 pending connections)
    listen(sockfd, 5);

    // Get the size of the client address structure
    clilen = sizeof(cli_addr);

    // Accept an incoming connection from a client
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
    {
        error("Error: On Accept");
    }

    // Zero out the buffer
    bzero(buffer, 256);

    // Read the message from the client
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        error("Error: Reading From Socket");
    }

    // Print the received message
    printf("Here Is The Message: %s\n", buffer);

    // Send a reply back to the client
    n = write(newsockfd, "I Got Your Message", 18);
    if (n < 0)
    {
        error("Error: Writing To Socket");
    }

    // Close the connection to the client and the server socket
    close(newsockfd);
    close(sockfd);

    return 0; // Exit the program successfully
}