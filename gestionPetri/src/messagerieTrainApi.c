#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <utils.h>

int getIPXwaySrc();

char *creationRequete(int  ipSrc, int ipDest, int cree, int idRequete, char * requete, char * ecriture){
    char *requeteStr = (char *) malloc(100);
    char *adressageRequete = (char *) malloc(14);
    char *requeteStrIntermediaire = (char *) malloc(100);
    int longueur = 0;

    sprintf(adressageRequete, "f1%02x10%02x10%s%02x%s", ipSrc, ipDest, (cree ==1) ? "09": "19", idRequete, requete);

    if (cree == 1){
        sprintf(requeteStrIntermediaire, "%s06%s", adressageRequete, ecriture);
    } else {
        sprintf(requeteStrIntermediaire, "%s", adressageRequete);
    }

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
        addrMot1 = 49;
        break;
    case 4: 
        addrMot1 = 52;  
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
    
    int ipXwaySrc = getIPXwaySrc();

    requete = creationRequete(ipXwaySrc, IPXWAY_DEST, 1, idRequete, "37", ecritureMots(nTrain, ipXwaySrc, -1, codeAig)); 
    DEBUG_PRINT("changementAiguillage : requete=%s\n", requete);

    tailleRequete = strlen(requete); 

    char *trameBinaire = conversionAscii2Binaire(requete, tailleRequete);
    CHECK(send(sd, trameBinaire, tailleRequete/2, 0), "changementAiguillage : Envoi rate\n");
    DEBUG_PRINT("changementAiguillage : Envoi reussi\n");

    nbcar = recvfrom(sd,buff, 100, 0, NULL, NULL);
    recu = conversionBinaire2Ascii(buff, nbcar);
    DEBUG_PRINT("changementAiguillage : Reponse=%s\n", recu);

    char ack = recu[strlen(recu) - 1];
    

    free(requete);
    free(trameBinaire);
    free(recu);

    DEBUG_PRINT("changementAiguillage : Ack=%c\n", ack);

    if (ack == 'E') return 1;
    else if (ack == 'D') return 0;
    else return -1;

}

int allumageTroncon(int sd, int nTrain, int codeTroncon){
    int idRequete = nTrain*10 + rand() % 10;
    char *requete, buff[100], *recu; 
    int nbcar, tailleRequete; 

    int ipXwaySrc = getIPXwaySrc();

    requete = creationRequete(ipXwaySrc, IPXWAY_DEST, 1, idRequete, "37", ecritureMots(nTrain, ipXwaySrc, codeTroncon, -1)); 
    DEBUG_PRINT("allumageTroncon : Requete=%s\n", requete);

    tailleRequete = strlen(requete); 

    char *trameBinaire = conversionAscii2Binaire(requete, tailleRequete);
    CHECK(send(sd, trameBinaire, tailleRequete/2, 0), "allumageTroncon : Envoi rate\n");
    DEBUG_PRINT("allumageTroncon : Envoi reussi\n");

    nbcar = recvfrom(sd,buff, 100, 0, NULL, NULL);
    recu = conversionBinaire2Ascii(buff, nbcar);
    DEBUG_PRINT("allumageTroncon : Reponse=%s\n", recu);

    char ack = recu[strlen(recu) - 1];

    free(requete);
    free(trameBinaire);
    free(recu);

    DEBUG_PRINT("allumageTroncon : Ack=%c\n", ack);

    if (ack == 'E') return 1;
    else if (ack == 'D') return 0;
    else return -1;

}

int reponseXway(int sd, int *nTrain, int *nType, int *nCapteur){
    char buff[100], *recu; 
    int nbcar; 

    DEBUG_PRINT("reponseXway : Attente reponse Xway\n");

    nbcar = recvfrom(sd, buff, 100, 0, NULL, NULL);
    recu = conversionBinaire2Ascii(buff, nbcar);
    DEBUG_PRINT("reponseXway : Reponse=%s\n", recu);

    
    int idRequete = (recu[26] <= '9' ? recu[26] - '0' : recu[26] - 'A' + 10) * 16 + (recu[27] <= '9' ? recu[27] - '0' : recu[27] - 'A' + 10);
    DEBUG_PRINT("reponseXway : idRequete=%d\n", idRequete);
    
    int tailleRequete;
    char *requete; 
    int ipXwaySrc = getIPXwaySrc();

    requete = creationRequete(ipXwaySrc, IPXWAY_DEST, 0, idRequete, "fe",   NULL); 
    DEBUG_PRINT("reponseXway : Requete=%s\n", requete);

    tailleRequete = strlen(requete); 

    char *trameBinaire = conversionAscii2Binaire(requete, tailleRequete);
    CHECK(send(sd, trameBinaire, tailleRequete/2, 0), "reponseXway : Envoi rate\n");
    DEBUG_PRINT("reponseXway : Envoi reussi\n");


    int nCapt = 0;
    nCapt += (recu[44] <= '9' ? recu[44] - '0' : recu[44] - 'A' + 10) * 16;
    nCapt += (recu[45] <= '9' ? recu[45] - '0' : recu[45] - 'A' + 10);

    nCapt += (recu[46] <= '9' ? recu[46] - '0' : recu[46] - 'A' + 10) * 16*16*16;
    nCapteur += (recu[47] <= '9' ? recu[47] - '0' : recu[47] - 'A' + 10) * 16*16;

    DEBUG_PRINT("reponseXway : nCapt=%d\n", nCapt);

    *nCapteur = nCapt;


    int nMot = 0;
    nMot += (recu[36] <= '9' ? recu[36] - '0' : recu[36] - 'A' + 10) * 16;
    nMot += (recu[37] <= '9' ? recu[37] - '0' : recu[37] - 'A' + 10);

    nMot += (recu[38] <= '9' ? recu[38] - '0' : recu[38] - 'A' + 10) * 16*16*16;
    nMot += (recu[39] <= '9' ? recu[39] - '0' : recu[39] - 'A' + 10) * 16*16;

    DEBUG_PRINT("reponseXway : nMot=%d\n", nMot);

    /*
    nType = 0 : Valeur du capteur d'arrivée sur le tronçon suivant
    nType = 1 : Code requête envoyé pour demander le service
    
    */

    switch (nMot)
    {
    case 8:
        *nTrain = 1;
        *nType = TYPE_TRONCON;
        break;
    case 9:
        *nTrain = 1;
        *nType = TYPE_AIGUILLAGE;
        break;
    case 10:
        *nTrain = 3;
        *nType = TYPE_TRONCON;
        break;
    case 11:
        *nTrain = 3;
        *nType = TYPE_AIGUILLAGE;
        break;
    case 12:
        *nTrain = 4;
        *nType = TYPE_TRONCON;
        break;
    case 13:
        *nTrain = 4;
        *nType = TYPE_AIGUILLAGE;
        break;
    case 14:
        *nTrain = 4;
        *nType = TYPE_INVERSION;
        break;
    case 15:
        *nTrain = 2;
        *nType = TYPE_TRONCON;
        break;
    case 16:
        *nTrain = 2;
        *nType = TYPE_AIGUILLAGE;
        break;
    default:
        printf("reponseXway : nMot inconnu\n");
        return -1;
        break;
    }

    /* Cf. TP

    | Adresse | Nom            | Description                                        |
    |---------|----------------|----------------------------------------------------|
    | %MW8    | AckTroncTr1    | Valeur du capteur d'arrivée sur le tronçon suivant |
    | %MW9    | AckTAigTr1     | Code requête envoyé pour demander le service       |
    | %MW10   | AckTroncTr3    | Valeur du capteur d'arrivée sur le tronçon suivant |
    | %MW11   | AckTAigTr3     | Code requête envoyé pour demander le service       |
    | %MW12   | AckTroncTr4    | Valeur du capteur d'arrivée sur le tronçon suivant |
    | %MW13   | AckTAigTr4     | Code requête envoyé pour demander le service       |
    | %MW14   | AckInvTr4      | Code requête envoyé pour demander le service       |
    | %MW15   | AckTroncTr2    | Valeur du capteur d'arrivée sur le tronçon suivant |
    | %MW16   | AckTAigTr2     | Code requête envoyé pour demander le service       |
    
    */


    free(recu);
    free(requete);
    free(trameBinaire);

    return 0;
}

    
int getIPXwaySrc(){
    FILE *fichier;
    int ipXwaySrc;
    CHECK_NULL(fichier = fopen("IP_XWAY_SRC", "r"), "fopen(XP_XWAY_SRC)");
    
    fscanf(fichier, "%d", &ipXwaySrc);

    DEBUG_PRINT("getIPXwaySrc : ipXwaySrc=%d\n", ipXwaySrc);

    fclose(fichier);
    return ipXwaySrc;
}

int troncon(int sd, int nTrain, int codeTroncon){
    // Faire allumage troncon
    CHECK(allumageTroncon(sd, nTrain, codeTroncon), "Allumage troncon fail n");
    printf("Troncon : Allumage troncon reussi\n");

    int nTrainRecu=-1, nType=-1, nCapteur=-1;
    CHECK(reponseXway(sd, &nTrainRecu, &nType, &nCapteur), "Reponse Xway fail");

    printf("Troncon retour : Train=%d et capteur=%d\n", nTrain, nCapteur);
    printf("Troncon nType=%d\n", nType);
    return (nType == TYPE_TRONCON) -1;
}



int aiguillage(int sd, int nTrain, int codeAig){
    // Essai changement aiguillage
    CHECK(changementAiguillage(sd, nTrain, codeAig), "Changement aiguillage fail n");
    printf("Aiguillage : Changement aiguillage reussi\n");

    int nTrainRecu=-1, nType=-1, codeRequete=-1;
    CHECK(reponseXway(sd, &nTrainRecu, &nType, &codeRequete), "Reponse Xway fail");
    // Voir pour l'interface

    printf("Aiguillage retour : Train=%d et code requete=%d\n", nTrain, codeRequete);
    return (nType == TYPE_AIGUILLAGE && codeAig == codeRequete) -1;
}

void train1(int sd1){
    // Trajet train 1 OK
    //  T3 vers T23
    CHECK(aiguillage(sd1, 1, 31), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 1, 3), "Main : Troncon fail");

    // Demander R4

    // T23 vers Ti10
    CHECK(aiguillage(sd1, 1, 22), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 1, 23), "Main : Troncon fail");

    // Libérer R4
    // Demander R5 et R2

    // Ti10 vers T29
    CHECK(aiguillage(sd1, 1, 33), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 1, 10), "Main : Troncon fail");

    // Libère R2
    // Demande R1

    // T29 vers T19
    CHECK(aiguillage(sd1, 1, 3), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 1, 29), "Main : Troncon fail");

    // Libère R1 et R5

    // T19 vers T3
    CHECK(troncon(sd1, 1, 19), "Main : Troncon fail");
}

void train2(int sd1){
    // Trajet train 2 OK
    //  Ti04 vers T22
    CHECK(troncon(sd1, 2, 4), "Main : Troncon fail");

    // T22 vers T27
    CHECK(aiguillage(sd1, 2, 7), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 2, 22), "Main : Troncon fail");

    // T27 vers T28
    CHECK(troncon(sd1, 2, 27), "Main : Troncon fail");

    // T28 vers Ti9
    CHECK(aiguillage(sd1, 2, 13), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 2, 28), "Main : Troncon fail");

    // Ti09 vers T24
    CHECK(aiguillage(sd1, 2, 12), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 2, 9), "Main : Troncon fail");

    // T24 vers Ti04
    CHECK(aiguillage(sd1, 2, 20), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 2, 24), "Main : Troncon fail");
}

void train3(int sd1){
    // Trajet train 2 OK
    //  Ti00 vers T13
    CHECK(aiguillage(sd1, 3, 0), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 3, 0), "Main : Troncon fail");

    // T13 vers T20
    CHECK(troncon(sd1, 3, 13), "Main : Troncon fail");

    // T20 vers T30
    // Peut etre aiguillage PA2
    CHECK(troncon(sd1, 3, 20), "Main : Troncon fail");

    // T30 vers Ti9
    CHECK(aiguillage(sd1, 3, 14), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 3, 30), "Main : Troncon fail");

    // Ti09 vers T31
    CHECK(aiguillage(sd1, 3, 13), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 3, 9), "Main : Troncon fail");

    // T31 vers T26
    CHECK(troncon(sd1, 3, 31), "Main : Troncon fail");

    // T26 vers T15
    CHECK(aiguillage(sd1, 3, 21), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 3, 26), "Main : Troncon fail");

    // T15 vers T12
    CHECK(aiguillage(sd1, 3, 1), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 3, 15), "Main : Troncon fail");

    // T12 vers Ti00
    CHECK(troncon(sd1, 3, 12), "Main : Troncon fail");
}

void train4(int sd1){
    
    //  Ti07 vers T29
    CHECK(aiguillage(sd1, 4, 10), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 4, 7), "Main : Troncon fail");

    // T29 vers Ti09
    CHECK(aiguillage(sd1, 4, 33), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 4, 29), "Main : Troncon fail");

    // Ti09 vers T28
    CHECK(aiguillage(sd1, 4, 13), "Main : Changement aiguillage fail");
    troncon(sd1, 4, 49);
    CHECK(troncon(sd1, 4, 9), "Main : Troncon 9 fail");

    // T28 vers T27
    CHECK(troncon(sd1, 4, 28), "Main : Troncon fail");

    // T27 vers Ti07
    CHECK(aiguillage(sd1, 4, 23), "Main : Changement aiguillage fail");
    CHECK(troncon(sd1, 4, 27), "Main : Troncon fail");

    // Ti07 vers fin + inversion
    CHECK(troncon(sd1, 4, 37), "Main : Troncon fail");
    troncon(sd1, 4, 47);
}