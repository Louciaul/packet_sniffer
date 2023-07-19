#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFERS 1024

int argc_cmd = 0;
char argv_cmd[BUFFERS][BUFFERS];
int ctrl_socket = 0;
int data_socket = 0;

#define CHECK(x)                                            \
    do {                                                    \
        if (!(x)) {                                         \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x);                                     \
            exit(EXIT_FAILURE);                             \
        }                                                   \
    } while (0)

char *prompt_shell(void) {
    static char buf[512];

    snprintf(buf, sizeof(buf), "%s@ftp > ", getenv("USER"));
    fflush(stdout);

    return buf;
}

int takeInput(char *str) {
    char *buf;
    HIST_ENTRY *last;
    buf = readline(prompt_shell());
    last = previous_history();
    if (strlen(buf) != '\0' && (!last || strcmp(last->line, buf))) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        strcpy(str, buf);
        return 1;
    }
}

void parse_command(char command[BUFFERS]) {
    if (!strlen(command)) {
        return;
    }

    int idx = 0;
    for (size_t i = 0; i < strlen(command); i++) {
        if (command[i] == ' ') {
            argc_cmd += 1;
            idx = 0;
        } else {
            argv_cmd[argc_cmd][idx] = command[i];
            idx += 1;
        }
    }
    argc_cmd++;
}

// If you want some debug
void print_args() {
    printf("Here are the arguments:\n");
    for (int i = 0; i < argc_cmd; i++) {
        printf("%s\n", argv_cmd[i]);
    }
}

void reset_args() {
    memset(argv_cmd, 0, BUFFERS * BUFFERS);
    argc_cmd = 0;
}

int open_connexion(char *ip) {

    int fd;
    struct sockaddr_in dest;
    int connected = 1;

    char buffer[BUFFERS];
    char message[100];

    // on crée le socket
    CHECK((fd = socket(AF_INET, SOCK_STREAM, 0)) != -1);

    dest.sin_family = AF_INET;

    // On lui assinge l'adresse ip
    CHECK(inet_pton(AF_INET, ip, &(dest.sin_addr)) != -1);

    // On lui assigne le port
    dest.sin_port = htons(21);

    // connexion au serveur distant
    CHECK(connect(fd, (const struct sockaddr *)&dest, sizeof(struct sockaddr)) != -1);

    // On attend la réponse du serveur
    CHECK(recv(fd, buffer, BUFFERS, 0) != -1);

    printf("%s\n", buffer);

    // On l'affiche (demande d'identification)

    while (connected) {
        // On demande à l'utilisateur de saisir son login
        printf("Please login:\n");

        fgets(message, 100, stdin);
        sprintf(buffer, "USER %s", message);

        // On envoie le login
        CHECK(send(fd, buffer, strlen(buffer), 0) != -1);

        memset(buffer, 0, BUFFERS);

        // On attend la réponse du serveur
        CHECK(recv(fd, buffer, BUFFERS, 0) != -1);

        // On l'affiche
        printf("%s\n", buffer);

        memset(buffer, 0, BUFFERS);

        // On demande le mot de passe
        fgets(message, 100, stdin);
        sprintf(buffer, "PASS %s", message);

        // On envoie le mot de passe

        CHECK(send(fd, buffer, strlen(buffer), 0) != -1);

        CHECK(recv(fd, buffer, BUFFERS, 0) != -1);

        printf("%s\n", buffer);

        if (strstr(buffer, "230") != NULL) {
            connected = 0;
        } else {
            printf("Please reconnect\n");
        }
    }
    return fd;
}

int send_data_port(int socket_serv, int port) {
    int datafd;
    char buffer[BUFFERS];

    // structure d'adresse qui contiendra les params réseaux du récepteur
    struct sockaddr_in my_addr;

    // taille d'une structure sockaddr_in utile pour la fonction recv
    socklen_t fromlen = sizeof(struct sockaddr_in);

    // création du socket de connexion
    datafd = socket(AF_INET, SOCK_STREAM, 0);

    // famille d'adresses
    my_addr.sin_family = AF_INET;

    // récuperation du port du récepteur
    my_addr.sin_port = htons(port);

    // adresse IPv4 du récepteur
    inet_aton("192.168.15.11", &(my_addr.sin_addr));

    // association de la socket et des params réseaux du récepteur
    CHECK(bind(datafd, (void *)&my_addr, fromlen) != -1);

    CHECK(listen(datafd, 1) != -1);

    // envoi du port au serveur
    snprintf(buffer, sizeof(buffer), "PORT 192,168,15,11,%d,%d\r\n", port / 256, port % 256);
    CHECK(send(socket_serv, buffer, strlen(buffer), 0) != -1);
    CHECK(recv(socket_serv, buffer, BUFFERS, 0) != -1);
    printf("%s", buffer);

    return datafd;
}

int receive_data(int data_socket) {
    struct sockaddr_in clients;
    int socket;
    // taille d'une structure sockaddr_in utile pour la fonction recv
    socklen_t fromlen = sizeof(struct sockaddr_in);

    CHECK((socket = accept(data_socket, (void *)&clients, &fromlen)) != -1);

    return socket;
}

void show_dir(int ctrl_socket, int data_socket) {
    char buffer[BUFFERS];

    // On crée une commande LIST
    sprintf(buffer, "LIST\r\n");

    // On envoie la commande
    CHECK(send(ctrl_socket, buffer, strlen(buffer), 0) != -1);

    memset(buffer, 0, BUFFERS);

    // On reçoit le code de retour
    CHECK(recv(ctrl_socket, buffer, BUFFERS, 0) != -1);

    printf("%s", buffer);
    int socket = receive_data(data_socket);

    // on regarde si c'est une erreur
    if (strstr(buffer, "150") == NULL) {
        printf("Error with LIST");
        return;
    }

    // On fait la boucle pour afficher l'ensemble des données

    int fully_received = 1;

    while (fully_received) {
        memset(buffer, 0, BUFFERS);
        CHECK(recv(socket, buffer, BUFFERS, 0) != -1);
        printf("%s", buffer);
        fflush(stdout);
        memset(buffer, 0, BUFFERS);
        // On regarde si on arrive à la fin des données
        // On reçoit le code de retour
        CHECK(recv(ctrl_socket, buffer, BUFFERS, 0) != -1);

        if (strstr(buffer, "226") != NULL) {
            printf("whole directory listed\n");
            fully_received = 0;
        }
    }

    CHECK(close(socket) != -1);
    CHECK(close(data_socket) != -1);
}

void get(int ctrl_socket, int data_socket) {
    char buffer[BUFFERS * 2];
    memset(buffer, 0, BUFFERS * 2);
    int file;
    CHECK(file = open(argv_cmd[1], O_CREAT | O_TRUNC | O_WRONLY, 0666) != -1);

    // On crée une commande LIST
    sprintf(buffer, "RETR %s\r\n", argv_cmd[1]);

    // On envoie la commande
    CHECK(send(ctrl_socket, buffer, strlen(buffer), 0) != -1);

    memset(buffer, 0, BUFFERS);

    // On reçoit le code de retour
    CHECK(recv(ctrl_socket, buffer, BUFFERS, 0) != -1);

    printf("%s", buffer);
    int socket = receive_data(data_socket);

    // on regarde si c'est une erreur
    if (strstr(buffer, "150") == NULL) {
        printf("Error with get");
        return;
    }

    // On fait la boucle pour afficher l'ensemble des données

    int fully_received = 1;

    while (fully_received) {
        memset(buffer, 0, BUFFERS);
        CHECK(recv(socket, buffer, BUFFERS, 0) != -1);
        CHECK(write(file, buffer, strlen(buffer)) != 1);
        memset(buffer, 0, BUFFERS);
        // On regarde si on arrive à la fin des données
        // On reçoit le code de retour
        CHECK(recv(ctrl_socket, buffer, BUFFERS, 0) != -1);

        if (strstr(buffer, "226") != NULL) {
            printf("Transfer complete\n");
            fully_received = 0;
        }
    }

    CHECK(close(socket) != -1);
    CHECK(close(data_socket) != -1);
    CHECK(close(file) != -1);
}

void quit_ftp(int ctrl_socket) {
    char message[100];
    char buffer[BUFFERS];
    // On crée une commande QUIT
    sprintf(message, "QUIT\r\n");

    // On envoie la commande
    CHECK(send(ctrl_socket, message, strlen(message), 0) != -1);

    memset(buffer, 0, BUFFERS);

    // On reçoit le code de retour
    CHECK(recv(ctrl_socket, buffer, BUFFERS, 0) != -1);

    printf("%s", buffer);

    CHECK(close(ctrl_socket) != -1);
}

int process_input() {

    if (!strcmp("open", argv_cmd[0])) {
        ctrl_socket = open_connexion(argv_cmd[1]);

    } else if (!strcmp("dir", argv_cmd[0])) {
        data_socket = send_data_port(ctrl_socket, 55000);
        show_dir(ctrl_socket, data_socket);

    } else if (!strcmp("get", argv_cmd[0])) {
        data_socket = send_data_port(ctrl_socket, 55000);
        get(ctrl_socket, data_socket);
    } else if (!strcmp("ciao", argv_cmd[0])) {
        quit_ftp(ctrl_socket);
    } else if (!strcmp("exit", argv_cmd[0])) {
        exit(0);
    } else {
        printf("Command not found\n");
    }
    (void)ctrl_socket;
    (void)data_socket;
    return 0;
}

int main() {
    char command[BUFFERS];
    int cnt = 1;

    while (cnt) {
        reset_args();
        memset(command, 0, BUFFERS);

        takeInput(command);

        if (strcmp("quit", command) == 0) {
            cnt = 0;
            continue;
        }
        parse_command(command);
        process_input();
    }

    return 0;
}
