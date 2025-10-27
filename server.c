#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
int main(int argc, char const* argv[]){
    int server_fd, new_socket;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
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
    
    // Принимаем очередное подключение из очереди и создаем для него сокет
    if ((new_socket = accept(server_fd, (struct sockaddr*) &address, &addrlen)) < 0){
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Читаем сообщение в буффер
    valread = read(new_socket, buffer, 1024-1);
    printf("%s \n", buffer); // Выводим полученное сообщение
    send(new_socket, hello, strlen(hello), 0); // Отправляем ответ клиенту
    printf("Hello message sent\n");
    
    close(new_socket); // Разрываем соединение с клиентом
    close(server_fd); // Закрываем сокет сервера

    return 0;
}
