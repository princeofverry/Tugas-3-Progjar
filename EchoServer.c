#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main() {
 // Membuat objek untuk alamat server
 struct sockaddr_in server_addr;
 server_addr.sin_family = AF_INET; // Menggunakan protokol IPv4
 server_addr.sin_port = htons(4002); // Menentukan port (4002)
 server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Mengikuti alamat IP lokal
 int sockfd;
 // Membuat socket
 if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
 perror("couldn't create socket");
 return 1;
 }
 printf("Socket created\n");
 // Mengikat socket ke alamat server
 if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr))
< 0) {
 perror("couldn't bind socket");
 return 1;
 }
 printf("Bind at port 4002\n");
 // Mendengarkan koneksi dengan batasan antrian 5
 if (listen(sockfd, 5) < 0) {
 perror("couldn't listen to socket");
 return 1;
 }
 printf("Listening for connections (queue size: 5)\n");
 struct sockaddr_in client_addr;
 socklen_t client_addr_size = sizeof(client_addr);
 while (1) {
 int client_sockfd;
 // Menerima koneksi dari client
 if ((client_sockfd = accept(sockfd, (struct sockaddr *)
&client_addr, &client_addr_size)) < 0) {
 perror("couldn't accept connection");
 return 1;
 }
 printf("Accepted connection from %s\n",
inet_ntoa(client_addr.sin_addr));
 char msg[100];
 while (1) {
 // Menerima pesan dari client
 ssize_t recv_result = recv(client_sockfd, msg, sizeof(msg), 0);
 if (recv_result <= 0) {
 perror("receive failed or connection closed");
 break;
 }
 printf("Received message: %s\n", msg);
 // Mengirim pesan kembali ke client
 ssize_t send_result = send(client_sockfd, msg, recv_result, 0);
 if (send_result <= 0) {
 perror("send failed or connection closed");
 break;
 }
 // Memeriksa apakah pesan adalah "bye" untuk keluar
 if (strcmp(msg, "bye") == 0) {
 printf("Exiting...\n");
 break;
 }
 }
 // Menutup koneksi dengan client
 close(client_sockfd);
 }
 // Menutup socket server
 close(sockfd);
 return 0;
}