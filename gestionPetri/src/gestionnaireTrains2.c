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




int main() {
    pthread_t thread_T3;
    pthread_t thread_T4;

    printf("Debut gestionnaire train 2\n");

    pthread_create(&thread_T3, NULL, gestion_T3, NULL);
    pthread_create(&thread_T4, NULL, gestion_T4, NULL);

    pthread_join(thread_T3, NULL);
    pthread_join(thread_T4, NULL);
    
    return 0;
}
