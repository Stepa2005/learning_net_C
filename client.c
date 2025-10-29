#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int args, char const* argv[]){
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char* hello = "Hello from client";
    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ // Создание сокета
        printf("\n Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET; // Указываем семейство адресов - IPv4
    serv_addr.sin_port = htons(PORT); // Делаем так, чтобы порт был понят всеми в сети

    // Преобразуем текстовый IP адрес в бинарный формат
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    // Устанавливаем TCP соединение с сервером
    if ((status = connect(client_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))) < 0){
        printf("\n Connection failed \n");
        return -1;
    }
    
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    valread = read(client_fd, buffer, 1024-1);
    printf("%s\n", buffer);

    close (client_fd);
    return 0;
}



