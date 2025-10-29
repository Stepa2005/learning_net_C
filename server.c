#include <netinet/in.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const* argv[]){
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int opt = 1;
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

    // Создание файлового дескриптора сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Настраиваем поведение сокета
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){ // Делаем так, чтобы сокет можно было использовать сразу после закрытия программы
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET; // Семейство протоколов AF_INET -  Address Family - INTERNET (IPv4)
    address.sin_addr.s_addr = INADDR_ANY; // IP на котором будем принимать подключения (на всех сетевых интерфейсах)
    address.sin_port = htons(PORT); // Порт, на котором будет работать сервер
    
    // Присоединяем сокет к порту
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Переводим серверный сокет в режим прослушивания
    if (listen(server_fd, 3) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    while(1){
        // Принимаем очередное подключение из очереди и создаем для него сокет
        client_fd = accept(server_fd, (struct sockaddr*) &address, &addrlen);
        if (client_fd < 0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        read(client_fd, buffer, sizeof(buffer)-1);
        printf("%s", buffer);
        close(client_fd); // Разрываем соединение с клиентом
    }

    close(server_fd); // Закрываем сокет сервера

    return 0;
}
