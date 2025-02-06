/****************************************************/
/* Application : serveurTCP                         */
/* Date : 13/01/2023                                */
/* Version : 1                                      */
/* Compilation : gcc serveurTCP.c -o serveurTCP     */
/* Usage : ./serveurUdp  [port]                     */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MAXCAR  80
#define LOCALPORT 3000
#include <utils.h>
#define CHECKERROR(var, val, msg) if (var == val) { perror(msg); exit(1); }

void handle_client(int sd) {
    char buff[MAXCAR + 1];
    int nbcar;

    while (1) {
        nbcar = recv(sd, buff, MAXCAR + 1, 0);
        if (nbcar > 0) {
            printf("\n SERVEUR : '%s' ", buff);
        } else {
            printf("Ca ne fonctionne pas !!!\n");
        }

        if (strcmp(buff, "FIN") == 0 || strcmp(buff, "fin") == 0) {
            printf("\n Arret de la connexion !!! \n");
            close(sd);
            break;
        }
    }
}

char * conversionBinaire2Ascii(char * binaire, int lgl){
    char * ascii = (char *) malloc(2 * lgl * sizeof(char));

    for(int i = 0; i < lgl; i++){
        ascii[2*i] = ((binaire[i] >> 4) & 0x0F) + (((binaire[i] >> 4) & 0x0F) < 10 ? '0' : 'A' - 10);
        ascii[2*i + 1] = (binaire[i] & 0x0F) + ((binaire[i] & 0x0F) < 10 ? '0' : 'A' - 10);
    }
    ascii[2 * lgl] = '\0';

    

    return ascii;
}

char * conversionAscii2Binaire(char * trame, int lgl){
    char * binaire = (char *) malloc(lgl / 2 * sizeof(char));
    
    for(int i = 0; i < lgl / 2; i++){
        char high = trame[2 * i];
        char low = trame[2 * i + 1];
        
        high = (high >= '0' && high <= '9') ? high - '0' : (high >= 'A' && high <= 'F') ? high - 'A' + 10 : high - 'a' + 10;
        low = (low >= '0' && low <= '9') ? low - '0' : (low >= 'A' && low <= 'F') ? low - 'A' + 10 : low - 'a' + 10;
        
        binaire[i] = (high << 4) + low;
    }

    return binaire;
}

int main(int argc, char *argv[]) {
    int se, sd;
    struct sockaddr_in addrserv;
    int opt = 1; // Add this line

    // Etape 1 - Creation de la socket
    se = socket(AF_INET, SOCK_STREAM, 0);
    CHECKERROR(se, -1, "Creation fail !!!\n");

    // Add these lines:
    if (setsockopt(se, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    // Etape 2 - Adressage de la socket d'ecoute
    addrserv.sin_family = AF_INET;
    addrserv.sin_addr.s_addr = INADDR_ANY;
    addrserv.sin_port = htons(LOCALPORT);

    // Etape 3 - Affectation de l'adresse a la socket avec controle d'erreur
    CHECKERROR(bind(se, (const struct sockaddr *)&addrserv, sizeof(addrserv)), -1, "Erreur de bind !!!\n");

    // Etape 4 - definir le nombre de connexions simultannees
    listen(se, 1);

    // Etape 5 - mise du serveur a l'ecoute des demandes de connexion
    printf("\n\tSERVEUR TCP\n\n");

   
    sd = accept(se, NULL, NULL);
    CHECKERROR(sd, -1, "L'ouverture de connexion n'a pas fonctionne !!!\n");

    printf("Ouverture d'une nouvelle connexion !!! \n");
    int nbcar;
    char buff[MAXCAR + 1];
    nbcar = recv(sd, buff, MAXCAR + 1, 0);
    printf("nbcar = %d\n", nbcar);



    if (nbcar > 0) {
        printf("\nSERVEUR : '%s'\n", conversionBinaire2Ascii(buff, nbcar));
        CHECK(send(sd, conversionAscii2Binaire("1d",2), 1, 0), "Envoi fail !!!\n");
    } else {
        printf("Ca ne fonctionne pas !!!\n");
    }
    
    // Envoyer D en binaire au client. 
    

    return EXIT_SUCCESS;
}
