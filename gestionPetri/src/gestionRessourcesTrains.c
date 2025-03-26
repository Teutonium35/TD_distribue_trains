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
#include <utils.h>

// Fonction pour demander une ressource en créant une nouvelle connexion

int genereRessource(int r, int sock) {
    // int sock;
    struct sockaddr_in serv_addr;
    char message[10];
    char buffer[1024] = {0};

    // // Création du socket pour cette requête
    // sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Erreur socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Adresse invalide");
        close(sock);
        return -1;
    }

    // Connexion au serveur
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connexion échouée");
        close(sock);
        return -1;
    }

    // Envoi de la requête
    snprintf(message, sizeof(message), "%d", -r); // on envoie - le numéro de la ressource à libérer
    send(sock, message, strlen(message), 0);

    // Attente de la réponse
    read(sock, buffer, sizeof(buffer));
    printf("Réponse du serveur : %s\n", buffer);

    int reponse;
    sscanf(buffer, "%d", &reponse);
    printf("Valeur de reponse : %d \n", reponse);

    close(sock); // Fermer la connexion après chaque requête
    return reponse;
}

// Fonction pour libérer une ressource en créant une nouvelle connexion
int utiliseRessource(int* r, int nombreRessourcesDemandées, int sock) {
    // int sock;
    struct sockaddr_in serv_addr;
    char message[1000000];
    char buffer[1024] = {0};

    // // Création du socket pour cette requête
    // sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Erreur socket");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Adresse invalide");
        close(sock);
        return -1;
    }

    // Connexion au serveur
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connexion échouée");
        close(sock);
        return -1;
    }

    int envoi = nombreRessourcesDemandées;
    for(int i=0; i<nombreRessourcesDemandées; i++)
    {
        envoi += r[i] * pow(500,i+1); // on génère le nombre qui demande les ressources de r
    }
    printf("Valeur de envoi : %d \n", envoi);
    // Envoi de la requête
    snprintf(message, sizeof(message), "%d", envoi); // on envoie + le numéro de la ressource à libérer
    send(sock, message, strlen(message), 0);

    // Attente de la réponse
    read(sock, buffer, sizeof(buffer));
    printf("Réponse du serveur : %s\n", buffer);

    int reponse;
    sscanf(buffer, "%d", &reponse);
    printf("Valeur de reponse : %d \n", reponse);

    close(sock); // Fermer la connexion après chaque requête
    return reponse;
}

void* gestion_T1(void* arg)
{
    int sock = *(int *)arg;
    while(1)
    {
        //  T3 vers T23
        CHECK(aiguillage(sock, 1, 31), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 1, 3), "Main : Troncon fail");

        genereRessource(Req_R4_TV,sock);
        int* transition1 = malloc(sizeof(int));
        *transition1 = Ach_R4_TV;

        // Demander R4
        while(utiliseRessource(transition1, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 1 attend la ressource Ach_R4_TV\n");
        }

        // T23 vers Ti10
        CHECK(aiguillage(sock, 1, 22), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 1, 23), "Main : Troncon fail");

        // Libérer R4
        genereRessource(Res_R4_TV,sock);
        int* transition2 = malloc(sizeof(int));
        *transition2 = Rach_R4_TV;
        while(utiliseRessource(transition2, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 1 attend la ressource Rach_R4_TV \n");
        }

         // Demander R5 et R2
        genereRessource(Req_R5_TV,sock);
        genereRessource(Req_R2_TV,sock);
        int* transition3 = malloc(2*sizeof(int));
        transition3[0] = Ach_R5_TV;
        transition3[1] = Ach_R2_TV;
        while(utiliseRessource(transition3, 2, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 1 attend les ressources Ach_R5_TV et Ach_R2_TV\n");
        }

        // Ti10 vers T29
        CHECK(aiguillage(sock, 1, 33), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 1, 10), "Main : Troncon fail");

        // Libère R2
        genereRessource(Res_R2_TV,sock);
        int* transition4 = malloc(sizeof(int));
        *transition4 = Rach_R2_TV;
        while(utiliseRessource(transition4, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 1 attend la ressource Rach_R2_TV\n");
        }

        // Demande R1
        genereRessource(Req_R1_TV,sock);
        int* transition5 = malloc(sizeof(int));
        *transition5 = Ach_R1_TV;
        while(utiliseRessource(transition5, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 1 attend la ressource Ach_R1_TV\n");
        }

        // T29 vers T19
        CHECK(aiguillage(sock, 1, 3), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 1, 29), "Main : Troncon fail");

        // Libère R1 et R5
        genereRessource(Res_R1_TV,sock);
        genereRessource(Res_R5_TV,sock);
        int* transition6 = malloc(2*sizeof(int));
        transition6[0] = Rach_R1_TV;
        transition6[1] = Rach_R5_TV;
        while(utiliseRessource(transition6, 2, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 1 attend les ressources Rach_R1_TV et Rach_R5_TV\n");
        }

        // T19 vers T3
        CHECK(troncon(sock, 1, 19), "Main : Troncon fail");
    }
}

void* gestion_T2(void* arg)
{
    
}