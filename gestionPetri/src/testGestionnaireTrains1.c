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


pthread_t thread_T1;
pthread_t thread_T2;

int main() {

    sleep(1);

    int* r1 = malloc(sizeof(int));
    r1[0] = 2;
    int reponse1 = utiliseRessource(r1,1);
    //printf("Depuis le main : reponse1 = %d \n", reponse1); 
    if(reponse1 == 0)
    {
        printf("GestionnaitreTrains1 possède la ressource 2 \n");
    }
    
    sleep(3); // Attendre un peu avant de demander une autre ressource
    
    r1[0] = 2;
    int reponse2 = genereRessource(2);
    //printf("Depuis le main : reponse2 = %d \n", reponse2);
    if(reponse2 == 0)
    {
        printf("GestionnaitreTrains1 lâche la ressource 2 \n");
    }

    sleep(3);

    int reponse3 = utiliseRessource(r1,1);
    //printf("Depuis le main : reponse3 = %d \n", reponse3);
    if(reponse3 == 0)
    {
        printf("GestionnaitreTrains1 possède la ressource 2 \n");
    }

    sleep(4);

    r1[0] = 1;
    int reponse4 = utiliseRessource(r1,1);
    //printf("Depuis le main : reponse4 = %d \n", reponse4);
    if(reponse4 == 0)
    {
        printf("GestionnaitreTrains1 possède la ressource 1 \n");
    }

    r1[0] = 1;
    r1[1] = 2;
    int reponse5 = utiliseRessource(r1,2);
    //printf("Depuis le main : reponse4 = %d \n", reponse4);
    if(reponse5 == 0)
    {
        printf("GestionnaitreTrains1 possède les ressources 1 et 2 \n");
    }
    else
    {
        printf("GestionnaitreTrains1 ne peut pas posséder les ressources 1 et 2 \n");
    }
    
    return 0;
}
