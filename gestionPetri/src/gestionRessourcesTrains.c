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
#include <messagerieTrainApi.h>
#define IP_GEST "127.0.0.1"

// Fonction pour demander une ressource en créant une nouvelle connexion

int genereRessource(int r, int sock) {
    // int sock;
    char message[10];
    char buffer[1024] = {0};

    // // Création du socket pour cette requête
    // sock = socket(AF_INET, SOCK_STREAM, 0);

    printf("Debut genereRessource pour la ressource %d\n", r);

    if (sock < 0) {
        perror("Erreur socket");
        return -1;
    }

    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_port = htons(PORT);

    // if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    //     perror("Adresse invalide");
    //     close(sock);
    //     return -1;
    // }

    // // Connexion au serveur
    // if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    //     perror("Connexion échouée");
    //     close(sock);
    //     return -1;
    // }

    // Envoi de la requête
    snprintf(message, sizeof(message), "%d", -r); // on envoie - le numéro de la ressource à libérer
    printf("genereRessource : Message envoyé : %s\n", message);
    int nbCar = send(sock, message, strlen(message), 0);
    printf("Nombre de caractères envoyés : %d\n", nbCar);

    // Attente de la réponse
    read(sock, buffer, sizeof(buffer));
    printf("genereRessource : Réponse du serveur : %s\n", buffer);

    int reponse = 1;
    sscanf(buffer, "%d", &reponse);
    printf("genereRessource : Valeur de reponse : %d \n", reponse);

    // close(sock); // Fermer la connexion après chaque requête
    return reponse;
}

// Fonction pour libérer une ressource en créant une nouvelle connexion
int utiliseRessource(int* r, int nombreRessourcesDemandees, int sock) {
    // int sock;
    char message[1000000];
    char buffer[1024] = {0};

    // // Création du socket pour cette requête
    // sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Erreur socket");
        return -1;
    }

    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_port = htons(PORT);

    // if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    //     perror("Adresse invalide");
    //     close(sock);
    //     return -1;
    // }

    // // Connexion au serveur
    // if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
    //     perror("Connexion échouée");
    //     close(sock);
    //     return -1;
    // }

    int envoi = nombreRessourcesDemandees;
    for(int i=0; i<nombreRessourcesDemandees; i++)
    {
        envoi += r[i] * pow(500,i+1); // on génère le nombre qui demande les ressources de r
    }
    printf("utiliseRessource :Valeur de envoi : %d \n", envoi);
    // Envoi de la requête
    snprintf(message, sizeof(message), "%d", envoi); // on envoie + le numéro de la ressource à libérer
    printf("utiliseRessource :Message envoyé : %s\n", message);
    int nbCar = send(sock, message, strlen(message), 0);
    printf("utiliseRessource :Nombre de caractères envoyés : %d\n", nbCar);
    
    // Attente de la réponse
    read(sock, buffer, sizeof(buffer));
    printf("Réponse du serveur : %s\n", buffer);

    int reponse;
    sscanf(buffer, "%d", &reponse);
    printf("Valeur de reponse : %d \n", reponse);

    // close(sock); // Fermer la connexion après chaque requête
    return reponse;
}

void* gestion_T1(void*)
{
    // int sock = *(int *)arg;


    // Ouverture socket avec le gestionnaire de ressources
    int sock; //descripteur de socket de dialogue
    struct sockaddr_in addrServRessource;
    
    

    //Etape 1 - Creation de la socket

    sock = socket(AF_INET, SOCK_STREAM, 0);

    CHECK(sock, "Creation fail sock");

    //Etape2 - Adressage du destinataire

    addrServRessource.sin_family=AF_INET;
    addrServRessource.sin_port=htons(PORT); 
    addrServRessource.sin_addr.s_addr=inet_addr(IP_GEST); 


    //Etape 3 - demande d'ouverture de connexion
    printf("IP : %s, PORT: %d\n", IP_GEST, PORT);
    CHECK(connect(sock, (const struct sockaddr *)&addrServRessource, sizeof(struct sockaddr_in)), "connect(adddServ) fail\n");
    printf("Connexion reussie\n");

    // Ouverture socket avec le train

    int sd1; //descripteur de socket de dialogue
    struct sockaddr_in addrServ;
    
    

    //Etape 1 - Creation de la socket

    sd1 = socket(AF_INET, SOCK_STREAM, 0);

    CHECK(sd1, "Creation fail n");

    //Etape2 - Adressage du destinataire

    addrServ.sin_family=AF_INET;
    addrServ.sin_port=htons(PORT_DEST); 
    addrServ.sin_addr.s_addr=inet_addr(IP_DEST); 


    //Etape 3 - demande d'ouverture de connexion
    printf("IP_DEST : %s, PORT_DEST: %d\n", IP_DEST, PORT_DEST);
    CHECK(connect(sd1, (const struct sockaddr *)&addrServ, sizeof(struct sockaddr_in)), "connect(adddServ) fail\n");
    printf("Connexion reussie\n");


    while(1)
    {
        //  T3 vers T23
        CHECK(aiguillage(sd1, 1, 31), "Main : Changement aiguillage fail");
        CHECK(troncon(sd1, 1, 3), "Main : Troncon fail");

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
        CHECK(aiguillage(sd1, 1, 22), "Main : Changement aiguillage fail");
        CHECK(troncon(sd1, 1, 23), "T1 : Troncon 23 fail");

        

        // Libérer R4
        genereRessource(Res_R4_TV,sock);
        printf("Coucou Lib R4\n");
        int* transition2 = malloc(sizeof(int));
        *transition2 = Rach_R4_TV;
        while(utiliseRessource(transition2, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 1 attend la ressource Rach_R4_TV \n");
        }

        printf("Le train 1 A la ressource Rach_R4_TV \n");

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

        printf("Le train 1 A les ressources Ach_R5_TV et Ach_R2_TV\n");

        // Ti10 vers T29
        CHECK(aiguillage(sd1, 1, 33), "Main : Changement aiguillage fail");
        CHECK(troncon(sd1, 1, 10), "Main : Troncon fail");

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
        CHECK(aiguillage(sd1, 1, 3), "Main : Changement aiguillage fail");
        CHECK(troncon(sd1, 1, 29), "Main : Troncon fail");

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
        CHECK(troncon(sd1, 1, 19), "Main : Troncon fail");
    }
}

void* gestion_T2(void* arg)
{
    printf("debut train 2\n");
    int sock = *(int *)arg;


    while(1)
    {
        //  Ti04 vers T22
        CHECK(troncon(sock, 2, 4), "Main : Troncon fail");

        // Demande R1 et R6
        genereRessource(Req_R1_TR,sock);
        genereRessource(Req_R6_TR,sock);
        int* transition1 = malloc(2*sizeof(int));
        transition1[0] = Ach_R1_TR;
        transition1[1] = Ach_R6_TR;
        while(utiliseRessource(transition1, 2, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 2 attend les ressources Ach_R1_TR et Ach_R6_TR\n");
        }

        // T22 vers T27
        CHECK(aiguillage(sock, 2, 7), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 2, 22), "Main : Troncon fail");

        // T27 vers T28
        CHECK(troncon(sock, 2, 27), "Main : Troncon fail");

        // Lache R1 
        genereRessource(Res_R1_TR,sock);
        int* transition2 = malloc(sizeof(int));
        *transition2 = Rach_R1_TR;
        while(utiliseRessource(transition2, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 2 attend la ressource Rach_R1_TR\n");
        }

        // Demande R3
        genereRessource(Req_R3_TR,sock);
        int* transition3 = malloc(sizeof(int));
        *transition3 = Ach_R3_TR;
        while(utiliseRessource(transition3, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 2 attend la ressource Ach_R3_TR\n");
        }

        // T28 vers Ti9
        CHECK(aiguillage(sock, 2, 13), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 2, 28), "Main : Troncon fail");

        // Libère R6
        genereRessource(Res_R6_TR,sock);
        int* transition4 = malloc(sizeof(int));
        *transition4 = Rach_R6_TR;
        while(utiliseRessource(transition4, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 2 attend la ressource Rach_R6_TR\n");
        }

        // Ti09 vers T24
        CHECK(aiguillage(sock, 2, 12), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 2, 9), "Main : Troncon fail");

        // T24 vers Ti04
        CHECK(aiguillage(sock, 2, 20), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 2, 24), "Main : Troncon fail");
    }
}

void* gestion_T3(void* arg)
{
    printf("debut train 3\n");
    int sock = *(int *)arg;

    while(1)
    {
        //  Ti00 vers T13
        CHECK(aiguillage(sock, 3, 0), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 3, 0), "Main : Troncon fail");

        // T13 vers T20
        CHECK(troncon(sock, 3, 13), "Main : Troncon fail");

        // T20 vers T30
        // Peut etre aiguillage PA2
        CHECK(troncon(sock, 3, 20), "Main : Troncon fail");

        // Demande R3 et R4
        genereRessource(Req_R3_TB,sock);
        genereRessource(Req_R4_TB,sock);
        int* transition1 = malloc(2*sizeof(int));
        transition1[0] = Ach_R3_TB;
        transition1[1] = Ach_R4_TB;
        while(utiliseRessource(transition1, 2, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 3 attend les ressources Ach_R3_TB et Ach_R4_TB\n");
        }

        // T30 vers Ti9
        CHECK(aiguillage(sock, 3, 14), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 3, 30), "Main : Troncon fail");

        // Libère R4
        genereRessource(Res_R4_TB,sock);
        int* transition2 = malloc(sizeof(int));
        *transition2 = Rach_R4_TB;
        while(utiliseRessource(transition2, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 3 attend la ressource Rach_R4_TB\n");
        }

        // Demande R2
        genereRessource(Req_R2_TB,sock);
        int* transition3 = malloc(sizeof(int));
        *transition3 = Ach_R2_TB;
        while(utiliseRessource(transition3, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 3 attend la ressource Ach_R2_TB\n");
        }

        // Ti09 vers T31
        CHECK(aiguillage(sock, 3, 13), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 3, 9), "Main : Troncon fail");

        // Libère R2 et R3
        genereRessource(Res_R2_TB,sock);
        genereRessource(Res_R3_TB,sock);
        int* transition4 = malloc(2*sizeof(int));
        transition4[0] = Rach_R2_TB;
        transition4[1] = Rach_R3_TB;
        while(utiliseRessource(transition4, 2, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 3 attend les ressources Rach_R2_TB et Rach_R3_TB\n");
        }

        // T31 vers T26
        CHECK(troncon(sock, 3, 31), "Main : Troncon fail");

        // T26 vers T15
        CHECK(aiguillage(sock, 3, 21), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 3, 26), "Main : Troncon fail");

        // T15 vers T12
        CHECK(aiguillage(sock, 3, 1), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 3, 15), "Main : Troncon fail");


        // T12 vers Ti00
        CHECK(troncon(sock, 3, 12), "Main : Troncon fail");
    }
}

// Doutes pour celui-là
void* gestion_T4(void* arg)
{
    printf("debut train 4\n");
    int sock = *(int *)arg;

    while(1)
    {
        // Attention : peut-être une couille au debut avec le démarrage à Ti07

        // Demande R1 et R5
        genereRessource(Req_R1_TJ_1,sock);
        genereRessource(Req_R5_TJ,sock);
        int* transition1 = malloc(2*sizeof(int));
        transition1[0] = Ach_R1_TJ_1;
        transition1[1] = Ach_R5_TJ;
        while(utiliseRessource(transition1, 2, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 4 attend les ressources Ach_R1_TJ et Ach_R5_TJ\n");
        }

        //  Ti07 vers T29
        CHECK(aiguillage(sock, 4, 10), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 4, 7), "Main : Troncon fail");

        // Libère R1
        genereRessource(Res_R1_TJ_1,sock);
        int* transition2 = malloc(sizeof(int));
        *transition2 = Rach_R1_TJ_1;
        while(utiliseRessource(transition2, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 4 attend la ressource Rach_R1_TJ\n");
        }

        // Demande R2,R3 et R6
        genereRessource(Req_R2_TJ,sock);
        genereRessource(Req_R3_TJ,sock);
        genereRessource(Req_R6_TJ,sock);
        int* transition3 = malloc(3*sizeof(int));
        transition3[0] = Ach_R2_TJ;
        transition3[1] = Ach_R3_TJ;
        transition3[2] = Ach_R6_TJ;
        while(utiliseRessource(transition3, 3, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 4 attend les ressources Ach_R2_TJ, Ach_R3_TJ et Ach_R6_TJ\n");
        }

        // T29 vers Ti09
        CHECK(aiguillage(sock, 4, 33), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 4, 29), "Main : Troncon fail");

        // Libère R2 et R5
        genereRessource(Res_R2_TJ,sock);
        genereRessource(Res_R5_TJ,sock);
        int* transition4 = malloc(2*sizeof(int));
        transition4[0] = Rach_R2_TJ;
        transition4[1] = Rach_R5_TJ;
        while(utiliseRessource(transition4, 2, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 4 attend les ressources Rach_R2_TJ et Rach_R5_TJ\n");
        }

        // Ti09 vers T28
        CHECK(aiguillage(sock, 4, 13), "Main : Changement aiguillage fail");
        troncon(sock, 4, 49);
        CHECK(troncon(sock, 4, 9), "Main : Troncon 9 fail");

        // Libère R3
        genereRessource(Res_R3_TJ,sock);
        int* transition5 = malloc(sizeof(int));
        *transition5 = Rach_R3_TJ;
        while(utiliseRessource(transition5, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 4 attend la ressource Rach_R3_TJ\n");
        }

        // Demande R1
        genereRessource(Req_R1_TJ_2,sock);
        int* transition6 = malloc(sizeof(int));
        *transition6 = Ach_R1_TJ_2;
        while(utiliseRessource(transition6, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 4 attend la ressource Ach_R1_TJ\n");
        }

        // T28 vers T27
        CHECK(troncon(sock, 4, 28), "Main : Troncon fail");

        // Libère R6
        genereRessource(Res_R6_TJ,sock);
        int* transition7 = malloc(sizeof(int));
        *transition7 = Rach_R6_TJ;
        while(utiliseRessource(transition7, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 4 attend la ressource Rach_R6_TJ\n");
        }

        // T27 vers Ti07
        CHECK(aiguillage(sock, 4, 23), "Main : Changement aiguillage fail");
        CHECK(troncon(sock, 4, 27), "Main : Troncon fail");

        // Libère R1
        genereRessource(Res_R1_TJ_2,sock);
        int* transition8 = malloc(sizeof(int));
        *transition8 = Rach_R1_TJ_2;
        while(utiliseRessource(transition8, 1, sock) != 0) 
        {
            sleep(0.5);
            printf("Le train 4 attend la ressource Rach_R1_TJ\n");
        }

        // Ti07 vers fin + inversion
        CHECK(troncon(sock, 4, 37), "Main : Troncon fail");
        troncon(sock, 4, 47);
    }
}