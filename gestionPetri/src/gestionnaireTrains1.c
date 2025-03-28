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



int main() {
    

    printf("Debut gestionnaire train 1\n");

    pthread_create(&thread_T1, NULL, gestion_T1, NULL);
    pthread_create(&thread_T2, NULL, gestion_T2, NULL);

    pthread_join(thread_T1, NULL);
    pthread_join(thread_T2, NULL);
    
    return 0;
}
