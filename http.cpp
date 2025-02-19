#include <iostream>
#include <cstring>      // For memset
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <unistd.h>     // For close

#define PORT 8080       // Port to listen on

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Step 1: Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Bind the socket to an address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Listen on any network interface
    address.sin_port = htons(PORT);      // Convert port to network byte order

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Step 3: Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Prepare a simple HTTP response
    const char* http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 77\r\n"
        "\r\n"
        "<html><body><h1>Welcome to my simple Unikernel web server!</h1></body></html>";
    const size_t response_length = strlen(http_response);

    std::cout << "Server listening on port " << PORT << std::endl;
    while (true) {

        // Step 4: Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Step 5: Send a response
        send(new_socket, http_response, response_length, 0);

        // Step 6: Close sockets
        close(new_socket);
    }
    close(server_fd);

    return 0;
}