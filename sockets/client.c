#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Error handling function
void error(const char *msg)
{
    perror(msg); // Print error message
    exit(1);     // Exit the program with an error code
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server; // Structure for storing server details
    char buffer[256];       // Buffer to store data to send/receive

    // Check if both host and port are provided by the user
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s Hostname Port\n", argv[0]);
        exit(1);
    }

    // Convert port number from string to integer
    portno = atoi(argv[2]);

    // Create a socket using TCP protocol
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Error Opening Socket");
    }

    // Get server's address information based on the provided hostname
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error: No Such Host\n");
        exit(1);
    }

    // Zero out the server address structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // Use IPv4

    // Copy the server's IP address into the address structure
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr_list[0], server->h_length);

    // Set the port number (converted to network byte order)
    serv_addr.sin_port = htons(portno);

    // Establish a connection to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Error: Connecting");
    }

    // Prompt the user to enter a message to send to the server
    printf("Please Enter The Message: ");
    bzero(buffer, 256); // Zero out the buffer

    // Read the user's message
    fgets(buffer, 255, stdin);

    // Send the message to the server
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        error("Error: Writing To Socket");
    }

    // Zero out the buffer again to read the server's response
    bzero(buffer, 256);

    // Read the server's response
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        error("Error: Reading From Socket");
    }

    // Print the response from the server
    printf("%s\n", buffer);

    // Close the socket connection
    close(sockfd);
    return 0; // Exit the program successfully
}