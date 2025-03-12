#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_THREADS 100
int open_count = 0, close_count = 0;

// Struct to pass arguments to the thread function
struct ThreadArgs {
    char ip_address[16];
    int port;
};

// Function to scan a port on a specific IP address
int scan_port(char *ip_address, int port) {
    // Create a TCP socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Failed to create socket");
        return -1;
    }

    // Set the IP address and port for the socket
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip_address);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect the socket to the port at the IP address
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        // The port is not open
        close(socket_desc);
        return 0;
    } else {
        // The port is open
        close(socket_desc);
        return 1;
    }
}

// Thread function to scan a port
void *thread_scan_port(void *args) {
    struct ThreadArgs *thread_args = (struct ThreadArgs *)args;
    int result = scan_port(thread_args->ip_address, thread_args->port);

    if (result) {
        printf("Port %d is open\n", thread_args->port);
	open_count++;
    } 

    free(thread_args);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <IP address> <starting port> <ending port>\n", argv[0]);
        return 1;
    }

    char *ip_address = argv[1];
    int port_start = atoi(argv[2]);
    int port_end = atoi(argv[3]);

    if (port_start < 1 || port_start > 65535 || port_end < 1 || port_end > 65535) {
        printf("Invalid port range. Ports must be between 1 and 65535.\n");
        return 1;
    }

    if (port_start > port_end) {
        printf("Invalid port range. Starting port must be less than or equal to ending port.\n");
        return 1;
    }

    time_t start, end;
    double time_taken;

    int number_of_ports = port_end - port_start + 1;
    printf("\nScanning ports on %s\n", ip_address);
    printf("\nPlease wait...this may take some time...\n\n");

    // Get the current time as the start time
    start = time(NULL);

    pthread_t threads[MAX_THREADS];
    int thread_count = 0;

    for (int port = port_start; port <= port_end; port++) {
        struct ThreadArgs *args = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
        strcpy(args->ip_address, ip_address);
        args->port = port;

        // Create a thread to scan the port
        pthread_create(&threads[thread_count], NULL, thread_scan_port, (void *)args);
        thread_count++;

        // Limit the number of concurrent threads
        if (thread_count >= MAX_THREADS) {
            for (int i = 0; i < thread_count; i++) {
                pthread_join(threads[i], NULL);
            }
            thread_count = 0;
        }
    }

    // Join remaining threads
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Get the current time as the end time
    end = time(NULL);

    // Calculate the time taken in seconds
    time_taken = difftime(end, start);
    printf("\nTotal open ports: %d\n", open_count);
    printf("\n%d ports are scanned in %.2f seconds\n\n", number_of_ports, time_taken);

    return 0;
}
