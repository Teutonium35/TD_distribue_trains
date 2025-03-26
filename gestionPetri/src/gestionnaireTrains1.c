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


pthread_t thread_T1;
pthread_t thread_T2;

int main() {

    sleep(1);

    int sock;

    // Création du socket pour cette requête
    sock = socket(AF_INET, SOCK_STREAM, 0);

    pthread_create(&thread_T1, NULL, gestion_T1, &sock);
    pthread_create(&thread_T2, NULL, gestion_T2, &sock);

    pthread_join(thread_T1, NULL);
    pthread_join(thread_T2, NULL);
    
    return 0;
}
