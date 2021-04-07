#include <iostream>
#include <cstring>
#include <thread>

// Socket Programming Headers
#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>

#include <netdb.h>


#define PORT 8080

void echo_back(int accepted_socket);

int create_listening_socket(){
    int socket_fd;
    int opt = 1;

    // Creating socket file descriptor
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

int listen_for_client() {
    int accepted_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    int server_fd = create_listening_socket();

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) &address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((accepted_socket = accept(server_fd, (struct sockaddr *) &address,
                                  (socklen_t *) &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);

    }
    return accepted_socket;
}

void echo_back(int accepted_socket) {
    int bytes_read;
    char buffer[1024] = {0};
    bytes_read = read(accepted_socket , buffer, 1024);
    printf("%s\n",buffer );
    printf("read bytes %d\n", bytes_read);

    send(accepted_socket , buffer , bytes_read , 0 );
    printf("Echo message sent\n");
}

int main() {

    struct hostent *lh = gethostbyname("www.google.com");

    std::cout << lh-> ;

//    int accepted_socket = listen_for_client();
//    std::thread communication_thread(echo_back,accepted_socket);
//    communication_thread.join();

    return 0;
}


