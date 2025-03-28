#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <math.h>
#include <gestionMutex.h>

pthread_mutex_t resources[NOMBRE_RESSOURCES];  // Ensemble des ressources, tout train confondu
// extern pthread_mutex_t resources[NOMBRE_RESSOURCES];  // Ensemble des ressources, tout train confondu

int prendRessource(int*r, int nombreRessourcesDemandees)
{
    for(int i=0; i<nombreRessourcesDemandees; i++)
    {
        if(pthread_mutex_trylock(&resources[r[i]]) != 0)
        {
            if(r[i] == Req_R4_TV)
            {
                printf("On ne peut pas prendre la ressource Req_R4_TV\n");
                sleep(3);
            }
            for(int j=0; j<i;j++) // vérifier l'indexage !
            {
                pthread_mutex_unlock(&resources[r[j]]);
                // printf(" Toutes les mutex ne sont pas dispos : on relaĉhe la ressource %d \n", r[j]+1);
            }
            return 1;
        }
        // printf("Ressource %d allouée \n", r[i] + 1);
    }
    return 0;
}

int lacheRessource(int numero_ressource)
{
    if(numero_ressource == Req_R4_TV)
    {
        printf("On va libérer Req_R4_TV \n");
    }
    pthread_mutex_unlock(&resources[numero_ressource]);
    if(numero_ressource == Req_R4_TV)
    {
        printf("On vient de libérer Req_R4_TV \n");
    }
    else
    {
        printf("On n'a pas libéré la ressource 60 ( Req_R4_TV) mais %d \n", numero_ressource);
    }
    sleep(3);
    return 0;
}