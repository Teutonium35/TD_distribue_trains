#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// il faut écrire une fonction demandeRessource() qui va vérifier si une liste de ressources est disponible. Si toute la liste est bien disponible, alors là seulement tu peux les utiliser.
// -> Risque de blocage sinon si on utilise une ressource en attendant qu'une autre soit libérée

#define PORT 8080

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
int utiliseRessource(int r) {
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
    snprintf(message, sizeof(message), "%d", r); // on envoie + le numéro de la ressource à libérer
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

int main() {

    sleep(1);

    int reponse1 = utiliseRessource(2);
    //printf("Depuis le main : reponse1 = %d \n", reponse1); 
    if(reponse1 == 0)
    {
        printf("GestionnaitreTrains1 possède la ressource 2 \n");
    }
    
    sleep(3); // Attendre un peu avant de demander une autre ressource
    
    int reponse2 = genereRessource(2);
    //printf("Depuis le main : reponse2 = %d \n", reponse2);
    if(reponse2 == 0)
    {
        printf("GestionnaitreTrains1 lâche la ressource 2 \n");
    }

    sleep(3);

    int reponse3 = utiliseRessource(2);
    //printf("Depuis le main : reponse3 = %d \n", reponse3);
    if(reponse3 == 0)
    {
        printf("GestionnaitreTrains1 possède la ressource 2 \n");
    }

    sleep(4);

    int reponse4 = utiliseRessource(1);
    //printf("Depuis le main : reponse4 = %d \n", reponse4);
    if(reponse4 == 0)
    {
        printf("GestionnaitreTrains1 possède la ressource 1 \n");
    }
    
    return 0;
}
