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
// Fonction pour demander une ressource en créant une nouvelle connexion
int genereRessource(int r) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[10];
    char buffer[1024] = {0};

    // Création du socket pour cette requête
    sock = socket(AF_INET, SOCK_STREAM, 0);
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
int utiliseRessource(int* r, int nombreRessourcesDemandées) {
    int sock;
    struct sockaddr_in serv_addr;
    char message[1000000];
    char buffer[1024] = {0};

    // Création du socket pour cette requête
    sock = socket(AF_INET, SOCK_STREAM, 0);
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