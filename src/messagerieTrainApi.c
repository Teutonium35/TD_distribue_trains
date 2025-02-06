#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPXWAY_SRC      41

#include <utils.h>


char *creationRequete(char * requete, int idRequete, char * ecriture, int  ipSrc, int ipDest, int cree){
    char *requeteStr = (char *) malloc(100);
    char *requeteStrIntermediaire = (char *)malloc(100);
    int longueur = 0;

    sprintf(requeteStrIntermediaire, "f1%02x10%02x10%s%02x%s06%s", ipSrc, ipDest, (cree ==1) ? "09": "1e", idRequete, requete, ecriture);

    longueur = strlen(requeteStrIntermediaire)/2;

    sprintf(requeteStr, "0000000100%02x00%s", longueur +1, requeteStrIntermediaire);
    free(requeteStrIntermediaire);
    return requeteStr;
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

char * conversionBinaire2Ascii(char * binaire, int lgl){
    char * ascii = (char *) malloc(2 * lgl * sizeof(char));

    for(int i = 0; i < lgl; i++){
        ascii[2*i] = ((binaire[i] >> 4) & 0x0F) + (((binaire[i] >> 4) & 0x0F) < 10 ? '0' : 'A' - 10);
        ascii[2*i + 1] = (binaire[i] & 0x0F) + ((binaire[i] & 0x0F) < 10 ? '0' : 'A' - 10);
    }
    ascii[2 * lgl] = '\0';

    

    return ascii;
}

void afficherBinaire(char * binaire, int lgl){
    for(int i = 0; i < lgl; i++){
        printf("%d ", binaire[i]);
    }
    printf("\n");
}

char * ecritureMots(int nTrain, int ipXway, int codeTroncon, int codeAig){
    char req[25];
    int addrMot1;

    if (codeTroncon > -1 && codeAig > -1)
    {
        printf("Erreur : code troncon et code aiguillage ne peuvent pas etre tous les deux renseignés\n");
        return NULL;
    }

    switch (nTrain)
    {
    case 1:
        addrMot1 = 39; // 39 hexa ou 39 decimal ?
        break;
    case 2:
        addrMot1 = 42; 
        break;
    case 3:
        addrMot1 = 45;
        break;
    case 4: 
        addrMot1 = 48;  
        break;
    default:
        break;
    }

    sprintf(req, "6807%02x%02x0300%02x%02x%02x%02x%02x%02x", addrMot1 & 0xff, (addrMot1 >> 8) & 0xff, ipXway & 0xff, (ipXway >> 8) & 0xff, codeTroncon & 0xff, (codeTroncon >> 8) & 0xff, codeAig & 0xff, (codeAig >> 8) & 0xff);

    return strdup(req);
}   

int changementAiguillage(int sd, int nTrain, int codeAig){
    int idRequete = nTrain*10 + rand() % 10;
    char *requete, buff[100], *recu; 
    int nbcar, tailleRequete; 

    requete = creationRequete("37", idRequete, ecritureMots(nTrain, IPXWAY_SRC, -1, codeAig), IPXWAY_SRC, IPXWAY_DEST, 1); 
    DEBUG_PRINT("requete : %s\n", requete);

    tailleRequete = strlen(requete); 

    char *trameBinaire = conversionAscii2Binaire(requete, tailleRequete);
    CHECK(send(sd, trameBinaire, tailleRequete/2, 0), "Envoi fail !!!\n");
    DEBUG_PRINT("Envoi reussi\n");

    nbcar = recvfrom(sd,buff, 100, 0, NULL, NULL);
    recu = conversionBinaire2Ascii(buff, nbcar);
    DEBUG_PRINT("Reponse: %s\n", recu);

    char ack = recu[strlen(recu) - 1];
    printf("Ack: %c\n", ack);

    free(requete);
    free(trameBinaire);

    if (ack == 'E') return 1;
    else if (ack == 'D') return 0;
    else return -1;

}
    


int main(int argc, char const *argv[])
{   
    
    int sd1; //descripteur de socket de dialogue
    struct sockaddr_in addrServ;
    
    //Etape 1 - Creation de la socket

    sd1 = socket(AF_INET, SOCK_STREAM, 0);

    CHECK(sd1, "Creation fail !!!\n");

    //Etape2 - Adressage du destinataire

    addrServ.sin_family=AF_INET;
    addrServ.sin_port=htons(PORT_DEST); 
    addrServ.sin_addr.s_addr=inet_addr(IP_DEST);


    //Etape 3 - demande d'ouverture de connexion
    printf("IP_DEST : %s, PORT_DEST: %d\n", IP_DEST, PORT_DEST);
    CHECK(connect(sd1, (const struct sockaddr *)&addrServ, sizeof(struct sockaddr_in)), "Connexion fail !!!\n");
    printf("Connexion reussie\n");

    CHECK(changementAiguillage(sd1, 1, 22), "Changement aiguillage fail !!!\n");
    printf("Changement aiguillage reussi\n");

    // Faire allumage troncon
    // faire demande de localisation du train avec gestion de la reponse en lisant la chaine recu.
    
    
    close(sd1);
    return 0;
}
