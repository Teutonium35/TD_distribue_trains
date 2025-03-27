#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>
#include <pthread.h>
#include <gestionnaireTrains.h>
#include <gestionMutex.h>

#define LOCALIP "127.0.0.1"
#define REMOTEPORT 3000
#define REMOTEIP "127.0.0.1"

#define CHECKERROR(var,val,msg)     if (var==val) {perror(msg); exit(1);}

pthread_t thread_T1;
pthread_t thread_T2;

int init_socket(struct sockaddr_in * addrServ, int * sd1){
    //Etape 1 - Creation de la socket

    *sd1=socket(AF_INET, SOCK_STREAM, 0);

    CHECKERROR(*sd1,-1, "Creation fail !!!\n");

    //Etape2 - Adressage du destinataire

    (*addrServ).sin_family=AF_INET;
    (*addrServ).sin_port=htons(REMOTEPORT);
    (*addrServ).sin_addr.s_addr=inet_addr(REMOTEIP);

    //Etape 3 - demande d'ouverture de connexion

    CHECKERROR(connect(*sd1, (const struct sockaddr *) addrServ, sizeof(struct sockaddr_in)),-1, "Connexion fail !!!\n");

    return 0;
}

int main() {
    int sd1; //descripteur de socket de dialogue
    int sd2;
    struct sockaddr_in addrServ;

    init_socket(&addrServ, &sd1);
    init_socket(&addrServ, &sd2);

    printf("sockets init\n");

    pthread_create(&thread_T1, NULL, gestion_T1, &sd1);
    pthread_create(&thread_T2, NULL, gestion_T2, &sd2);

    pthread_join(thread_T1, NULL);
    pthread_join(thread_T2, NULL);
    
    return 0;
}
