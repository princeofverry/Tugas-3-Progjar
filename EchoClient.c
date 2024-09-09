#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
int main() {
 // Membuat objek untuk alamat server
 struct sockaddr_in server_addr;
 server_addr.sin_family = AF_INET; // Menggunakan protokol IPv4
 server_addr.sin_port = htons(4002); // Menentukan port (4002)
 server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Alamat IP server
 int sockfd;
 // Membuat socket client
 if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
 perror("couldn't create socket");
 return 1;
 }
 printf("Socket created\n");
 // Melakukan koneksi ke server
 if (connect(sockfd, (struct sockaddr *) &server_addr,
sizeof(server_addr)) < 0) {
 perror("couldn't connect");
 return 1;
 }
 printf("Connected to the server\n");
 char msg[100], server_msg[100];
 while (1) {
 printf("Write a message: ");
 scanf("%[^\n]%*c", msg);
 // Mengirim pesan ke server
 if (send(sockfd, msg, sizeof(msg), 0) < 0) {
 perror("send failed");
 break;
 }
 // Mengosongkan pesan dari server_msg
 memset(server_msg, 0, sizeof(server_msg));
 // Menerima pesan dari server
 if (recv(sockfd, server_msg, sizeof(server_msg), 0) < 0) {
 perror("receive failed");
 break;
 }
 printf("Server reply: %s\n", server_msg);
 // Memeriksa apakah pesan dari server adalah "bye" untuk keluar
 if (strcmp(server_msg, "bye") == 0) {
 printf("Exiting...\n");
 break;
 }
 }
 // Menutup socket client
 close(sockfd);
 return 0;
}