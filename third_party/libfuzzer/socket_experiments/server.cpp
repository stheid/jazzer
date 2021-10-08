#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define SOCKET_NAME "/tmp/9Lq7BNBnBycd6nxy.socket"
#define BUFFER_SIZE 12

int initSock() {
  // Create local socket.
  connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (connection_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /* For portability clear the whole structure, since some
   * implementations have additional (nonstandard) fields in
   * the structure. */
  memset(&name, 0, sizeof(name));

  // Bind socket to socket name.
  name.sun_family = AF_UNIX;
  strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

  if (bind(connection_socket, (const struct sockaddr *)&name, sizeof(name)) ==
      -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  /* Prepare for accepting connections. The backlog size is set
   * to 20. So while one request is being processed other requests
   * can be waiting. */
  if (listen(connection_socket, 20) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
  struct sockaddr_un name;
  int down_flag = 0;
  int ret;
  int connection_socket;
  int data_socket;
  int result;
  char buffer[BUFFER_SIZE];

  initSock()
  // This is the main loop for handling connections.
  for (;;) {

    // Wait for incoming connection.
    if (accept(connection_socket, NULL, NULL) == -1) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    ret = listen(connection_socket, 20);
    if (ret == -1) {
      perror("listen");
      exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */

    for (;;) {

      /* Wait for incoming connection. */

      data_socket = accept(connection_socket, NULL, NULL);

      // Wait for next data packet.
      if (read(data_socket, buffer, sizeof(buffer)) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
      }

      // Ensure buffer is 0-terminated.
      buffer[sizeof(buffer) - 1] = 0;

      // Handle commands.
      if (!strncmp(buffer, "DOWN", sizeof(buffer))) {
        down_flag = 1;
        break;
      }

      if (!strncmp(buffer, "END", sizeof(buffer))) {
        break;
      }

      // Add received summand.
      result += atoi(buffer);
    }

    // Send result.
    sprintf(buffer, "%d", result);
    if (write(data_socket, buffer, sizeof(buffer)) == -1) {
      perror("write");
      exit(EXIT_FAILURE);
    }

    // Close socket.
    close(data_socket);

    // Quit on DOWN command.
    if (down_flag) {
      break;
    }
  }

  close(connection_socket);
  unlink(SOCKET_NAME);
  exit(EXIT_SUCCESS);
}
