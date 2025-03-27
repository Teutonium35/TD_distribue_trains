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


pthread_t thread_T3;
pthread_t thread_T4;

int main() {

    sleep(1);

    int sock;

    // Création du socket pour cette requête
    sock = socket(AF_INET, SOCK_STREAM, 0);

    pthread_create(&thread_T3, NULL, gestion_T3, &sock);
    pthread_create(&thread_T4, NULL, gestion_T4, &sock);

    pthread_join(thread_T3, NULL);
    pthread_join(thread_T4, NULL);
    
    return 0;
}
