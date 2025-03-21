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

void* handle_client(void* arg) {
    ClientRequest* request = (ClientRequest*) arg;
    int client_socket = request->client_socket;
    int resource_code = request->resource_id; // dans la suite on considèrera que si id > 0 : demande d'une ressource, si id < 0 : libération d'une ressource
    char buffer[1024];
    // int askForResource; // utilisé comme booléen

    if(resource_code > 0)
    {
        int nombreRessources = resource_code % 500;
        int *ressources_demandées = malloc(sizeof(int)* nombreRessources);
        // on décode le message transmis pour en déduire les ressources demandées
        for(int i=0; i<nombreRessources; i++)
        {
            ressources_demandées[i] = (resource_code / (int)pow(500,i+1)) % 500 - 1;
        }
        printf("Le client demande une ou des ressources par le code %d \n", resource_code);

        // on n'a pas pu prendre toutes les ressources 
        if(prendRessource(ressources_demandées, nombreRessources) == 1)
        {
            snprintf(buffer, sizeof(buffer), "%d \n", 1); // envoi de '1' pour signifier que toutes les ressources demandées ne sont pas dispos
            send(client_socket, buffer, strlen(buffer), 0);

            // Fermer la connexion et libérer la mémoire
            close(client_socket);
            free(request);
            pthread_exit(NULL);
        }

        //pthread_mutex_lock(&resources[resource_code]);
        // Simuler l'utilisation de la ressource
        sleep(3);
    }
    else
    {
        resource_code = -resource_code -1;
        printf("Client libère la ressource %d\n", resource_code+1);
        // Libérer le mutex de la ressource demandée
        lacheRessource(resource_code);
        printf("Ressource %d libérée \n", resource_code+1);
    }  
    // Envoyer la réponse au client
    // snprintf(buffer, sizeof(buffer), "Ressource %d allouée\n", resource_id);
    snprintf(buffer, sizeof(buffer), "%d \n", 0); // envoi de '0' pour signifier que la ressource a bien été allouée
    send(client_socket, buffer, strlen(buffer), 0);

    // Fermer la connexion et libérer la mémoire
    close(client_socket);
    free(request);
    printf("\n");
    pthread_exit(NULL);
}
