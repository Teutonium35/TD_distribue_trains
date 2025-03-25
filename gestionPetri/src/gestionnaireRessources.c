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

#define PORT 8080
#define MAX_CLIENTS 10

extern pthread_mutex_t resources[NOMBRE_RESSOURCES];  // Ensemble des ressources, tout train confondu

pthread_t thread_R1;
pthread_t thread_R2;
pthread_t thread_R3;
pthread_t thread_R4;
pthread_t thread_R5;
pthread_t thread_R6;

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Initialisation des mutex 
    for (int i = 0; i < NOMBRE_RESSOURCES; i++) {
        pthread_mutex_init(&resources[i], NULL);
    }

    // la boucle for suivante sert à bloquer les ressources qui sont initialiement "vides"
    for(int i=0; i<NOMBRE_RESSOURCES; i++)
    {
        // pthread_mutex_lock(&resources[i]); // il y a un problème : le réseau de Petri ne devrait pas évoluer avec des ressources bloquées
        if(i != R1_free && i != Req_R1_TR && i!= R2_free && i != Req_R2_TV && i!= R3_free && i != Req_R3_TB
        && i!= R4_free && i != Req_R4_TV && i != R5_free && i != Req_R5_TJ && i != R6_free && i != Req_R6_TJ)
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
    // Création du socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Erreur listen");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexions...\n");

    // Boucle d'acceptation des connexions clients
    while (1) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("Erreur accept");
            continue;
        }

        // Lire la requête du client (indice de la ressource demandée)
        char buffer[1024] = {0};
        read(client_socket, buffer, sizeof(buffer));
        int resource_id = atoi(buffer);  // Convertir en entier

        // Créer un thread pour gérer la requête
        pthread_t thread;
        ClientRequest* request = malloc(sizeof(ClientRequest));
        request->client_socket = client_socket;
        request->resource_id = resource_id;

        pthread_create(&thread, NULL, handle_client, request);
        pthread_detach(thread);
    }

    // Nettoyage des mutex (non atteint dans ce programme)
    for (int i = 0; i < NOMBRE_RESSOURCES; i++) {
        pthread_mutex_destroy(&resources[i]);
    }

}
