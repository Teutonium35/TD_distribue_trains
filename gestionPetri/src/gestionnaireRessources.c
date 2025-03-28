#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <math.h>
#include <gestionRessources.h>
#include <gestionMutex.h>
#include <gestionCommunication.h>
#include <errno.h>

#define LOCALIP "127.0.0.1"
#define LOCALPORT 3000
#define MAXCAR  80

#define CHECKERROR(var,val,msg)     if (var==val) {perror(msg); exit(1);}


#define PORT 8080
#define MAX_CLIENTS 10

extern pthread_mutex_t resources[NOMBRE_RESSOURCES];  // Ensemble des ressources, tout train confondu

pthread_t thread_R1;
pthread_t thread_R2;
pthread_t thread_R3;
pthread_t thread_R4;
pthread_t thread_R5;
pthread_t thread_R6;

void * listen_msg(void * arg){
    printf("thread lancé\n");
    int sd = *(int *)arg;
    free(arg);
    int nbcar;
    char buff[MAXCAR + 1];
    while (1){
        // Lire la requête du client (indice de la ressource demandée)
        nbcar=recv(sd,buff, MAXCAR, 0);
        buff[nbcar] = '\0';
        printf("listenMsg de %d = %d\n", sd, nbcar);
        if (nbcar>0){
            printf("SERVEUR : '%s', %d \n", buff, nbcar);
            int resource_id = atoi(buff);  // Convertir en entier

            // Créer un thread pour gérer la requête
            pthread_t thread;
            ClientRequest* request = malloc(sizeof(ClientRequest));
            request->client_socket = sd;
            request->resource_id = resource_id;

            pthread_create(&thread, NULL, handle_client, request);
            pthread_detach(thread);
        }
        else if (nbcar == 0){
            printf("Client %d déconnecté\n", sd);
            close(sd);
            break;
        }
        else
        {
            // If the error is due to an interruption, continue waiting
            if (errno == EINTR) continue;
            else {
                perror("threadConnexionClient: recv - Reception error");
                break;
            }
        }
    }
    pthread_exit(NULL);
    return NULL;
}

int main() {
    int se; // descripteur de la socket d'ecoute    
    struct sockaddr_in addrserv, addrcli;
    socklen_t addrcli_len = sizeof(struct sockaddr);

    //Etape 1 - Creation de la socket

    se=socket(AF_INET, SOCK_STREAM, 0);

    CHECKERROR(se,-1, "Creation fail !!!\n");

    //Etape2 - Adressage de la socket de la socket d'ecoute

    addrserv.sin_family=AF_INET;
    addrserv.sin_addr.s_addr=INADDR_ANY;//On recoit des data sur toutes les adresses IP de la machine

    //Definition d'un port de dialogue du lecteur
    addrserv.sin_port=htons(LOCALPORT);

    //Etape 3 - Affectation de l'adresse a la socket avec controle d'erreur
    CHECKERROR(bind(se, (const struct sockaddr *)&addrserv,sizeof(addrserv)), -1, "Erreur de bind !!!\n");

    //Etape 4 - definir le nombre de connexions simultannees

    listen(se,8);

    for (int i = 0;i<4;i++){
        pthread_t thread_id;
        int *sd;
        sd = malloc(sizeof(int));
        *sd=accept(se, (struct sockaddr *) &addrcli, &addrcli_len); //le serveur se met a l'ecoute des demande de connexion
        printf("Connexion acceptée %d\n", *sd);
        CHECKERROR(*sd,-1, "L'ouverture de connexion n'a pas fonctionne !!!\n");
        pthread_create(&thread_id, NULL, listen_msg, sd);
        pthread_detach(thread_id);
    }

    // Initialisation des mutex 
    for (int i = 0; i < NOMBRE_RESSOURCES; i++) {
        pthread_mutex_init(&resources[i], NULL);
    }

    // la boucle for suivante sert à bloquer les ressources qui sont initialiement "vides"
    for(int i=0; i<NOMBRE_RESSOURCES; i++)
    {
        // pthread_mutex_lock(&resources[i]); // il y a un problème : le réseau de Petri ne devrait pas évoluer avec des ressources bloquées
        if(i != R1_free && i != R2_free && i != R3_free 
        && i != R4_free && i != R5_free && i != R6_free )
        {
            pthread_mutex_lock(&resources[i]);
        }
    }

    pthread_create(&thread_R1, NULL, gestion_R1, NULL);
    pthread_detach(thread_R1);

    pthread_create(&thread_R2, NULL, gestion_R2, NULL);
    pthread_detach(thread_R2);

    pthread_create(&thread_R3, NULL, gestion_R3, NULL);
    pthread_detach(thread_R3);

    pthread_create(&thread_R4, NULL, gestion_R4, NULL);
    pthread_detach(thread_R4);

    pthread_create(&thread_R5, NULL, gestion_R5, NULL);
    pthread_detach(thread_R5);

    pthread_create(&thread_R6, NULL, gestion_R6, NULL);
    pthread_detach(thread_R6);

    /************************************/
             // Partie serveur //
    /************************************/
    // Boucle d'acceptation des connexions clients
    // On ne finit pas le programme, car il ne se finissait pas dans la version précédente et j'ai pas envie de tout casser
    while(1);
}
