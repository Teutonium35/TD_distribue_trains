#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <math.h>

#define PORT 8080
#define MAX_CLIENTS 10

enum res
{
    R1_free,
    Req_R1_TR,
    R1_TR_Alloc,
    Ach_R1_TR,
    R1_TR_Wait,
    Res_R1_TR,
    RAch_R1_TR,
    Req_R1_TV,
    Ach_R1_TV,
    R1_TV_Wait,
    Res_R1_TV,
    RAch_R1_TV,
    // deuxième graphe Gestionnaire de ressources 1
    Req_R1_TJ_1,
    Ach_R1_TJ_1,
    R1_TJ_Wait_1,
    Res_R1_TJ_1,
    RAch_R1_TJ_1,
    Req_R1_TJ_2,
    Ach_R1_TJ_2,
    R1_TJ_Wait_2,
    Res_R1_TJ_2,
    Rach_R1_TJ_2,
};

pthread_mutex_t resources[20];  // Ensemble des ressources, tout train confondu

pthread_t thread_R1;

typedef struct {
    int client_socket;
    int resource_id;
} ClientRequest;

int prendRessource(int*r, int nombreRessourcesDemandées)
{
    for(int i=0; i<nombreRessourcesDemandées; i++)
    {
        if(pthread_mutex_trylock(&resources[r[i]]) != 0)
        {
            for(int j=0; j<i+1;j++) // verifier l'indice j<i+1
            {
                pthread_mutex_unlock(&resources[r[j]]);
                printf(" Toutes les mutex ne sont pas dispos : on relaĉhe la ressource %d \n", r[j]+1);
            }
            return 1;
        }
        printf("Ressource %d allouée \n", r[i] + 1);
    }
    return 0;
}

int lacheRessource(int numero_ressource)
{
    pthread_mutex_unlock(&resources[numero_ressource]);
    return 0;
}

void* gestion_R1(void* arg) {
    
    int* r1_tr_alloc = malloc(2*sizeof(int));
    r1_tr_alloc[0] = R1_free;
    r1_tr_alloc[1] = Req_R1_TR;
    // il manque la ressource R6_free pour le moment !

    int* r1_tv_alloc = malloc(2*sizeof(int));
    r1_tv_alloc[0] = R1_free;
    r1_tv_alloc[1] = Req_R1_TV;

    int* r1_tr_rest = malloc(2*sizeof(int));
    r1_tr_rest[0] = Res_R1_TR;
    r1_tr_rest[1] = R1_TR_Wait;

    int* r1_tv_rest = malloc(2*sizeof(int));
    r1_tv_rest[0] = R1_TV_Wait;
    r1_tv_rest[1] = Res_R1_TV;

    int* r1_tj_alloc_1 = malloc(2*sizeof(int));
    r1_tj_alloc_1[0] = R1_free;
    r1_tj_alloc_1[1] = Req_R1_TJ_1;
    // il manque la ressource R5_free pour le moment !

    int* r1_tj_rest_1 = malloc(2*sizeof(int));
    r1_tj_rest_1[0] = R1_TJ_Wait_1;
    r1_tj_rest_1[1] = Res_R1_TJ_1;

    int* r1_tj_alloc_2 = malloc(2*sizeof(int));
    r1_tj_alloc_2[0] = Req_R1_TJ_2;
    r1_tj_alloc_2[1] = R1_free;


    int* r1_tj_rest_2 = malloc(2*sizeof(int));
    r1_tj_rest_2[0] = Res_R1_TJ_2;
    r1_tj_rest_2[1] = Rach_R1_TJ_2;

    // pour l'instant on considère que la transition est faite à chaque fois instantanément
    int R1_TR_Alloc = 1; // transition
    int R1_TV_Alloc = 1; // transition
    int R1_TR_Rest = 1; // transition
    int R1_TV_Rest = 1; // transition
    int R1_TJ_Alloc_1 = 1; // transition
    int R1_TJ_Rest_1 = 1; // transition
    int R1_TJ_Alloc_2 = 1; // transition
    int R1_TJ_Rest_2 = 1; // transition

    // si la transition R1_TR_Alloc est activée
    if(R1_TR_Alloc)
    {
        if(prendRessource(r1_tr_alloc,2) == 0)
        {
            lacheRessource(R1_TR_Wait);
            printf("Ressource 1 est dans l'état R1_TR_Wait \n");
        }
    }

    // si la transition R1_TV_Alloc est activée
    if(R1_TV_Alloc)
    {
        if(prendRessource(r1_tv_alloc,2) == 0)
        {
            lacheRessource(Ach_R1_TV);
            lacheRessource(R1_TV_Wait);
            printf("Ressource 1 est dans l'état R1_TV_Wait \n");
        }
    }

    // si la transition R1_TR_Rest est activée
    if(R1_TR_Rest)
    {
        if(prendRessource(r1_tr_rest ,2) == 0)
        {
            lacheRessource(RAch_R1_TR); // sur le réseau de Petri c'est RAch_R1_TV, je pense que c'est une erreur
            lacheRessource(R1_free);
            printf("Ressource 1 est dans l'état R1_free \n");
        }
    }

    // si la transition R1_TV_Rest est activée
    if(R1_TV_Rest)
    {
        if(prendRessource(r1_tv_rest ,2) == 0)
        {
            lacheRessource(RAch_R1_TV);
            lacheRessource(R1_free);
            printf("Ressource 1 est dans l'état R1_free \n");
        }
    }

    // si la transition R1_TV_Rest est activée
    if(R1_TJ_Alloc_1)
    {
        if(prendRessource(r1_tj_alloc_1 ,2) == 0)
        {
            lacheRessource(Ach_R1_TJ_1);
            lacheRessource(R1_TJ_Wait_1);
            printf("Ressource 1 est dans l'état R1_TJ_Wait_1 \n");
        }
    }

    // si la transition R1_TV_Rest est activée
    if(R1_TJ_Rest_1)
    {
        if(prendRessource(r1_tj_rest_1 ,2) == 0)
        {
            lacheRessource(RAch_R1_TJ_1);
            lacheRessource(R1_free);
            printf("Ressource 1 est dans l'état R1_free \n");
        }
    }

    // si la transition R1_TJ_Alloc_2 est activée
    if(R1_TJ_Alloc_2)
    {
        if(prendRessource(r1_tj_alloc_2 ,2) == 0)
        {
            lacheRessource(Ach_R1_TJ_2);
            lacheRessource(R1_TJ_Wait_2);
            printf("Ressource 1 est dans l'état R1_TJ_Wait_2 \n");
        }
    }

    // si la transition R1_TJ_Rest_1 est activée
    if(R1_TJ_Rest_1)
    {
        if(prendRessource(r1_tj_rest_1 ,2) == 0)
        {
            lacheRessource(Rach_R1_TJ_2);
            lacheRessource(R1_free);
            printf("Ressource 1 est dans l'état R1_free \n");
        }
    }
}

// Fonction pour gérer la requête d'un client dans un thread
void* handle_client(void* arg) {
    ClientRequest* request = (ClientRequest*) arg;
    int client_socket = request->client_socket;
    int resource_code = request->resource_id; // dans la suite on considèrera que si id > 0 : demande d'une ressource, si id < 0 : libération d'une ressource
    char buffer[1024];
    int askForResource; // utilisé comme booléen

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

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Initialisation des mutex : ici il faudra distinguer quelles sont les mutex avec ou sans ressource à l'initialisation
    for (int i = 0; i < 20; i++) {
        pthread_mutex_init(&resources[i], NULL);
    }

    for(int i=0; i<20; i++)
    {
        if(i != R1_free && i != Req_R1_TR)
        {
            pthread_mutex_lock(&resources[i]);
        }
    }

    pthread_create(&thread_R1, NULL, gestion_R1, NULL);
    pthread_detach(thread_R1);

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
    for (int i = 0; i < 3; i++) {
        pthread_mutex_destroy(&resources[i]);
    }

}
