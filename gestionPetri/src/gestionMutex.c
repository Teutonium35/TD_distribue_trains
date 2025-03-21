#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <math.h>
#include <gestionMutex.h>

pthread_mutex_t resources[100];  // Ensemble des ressources, tout train confondu

int prendRessource(int*r, int nombreRessourcesDemandées)
{
    for(int i=0; i<nombreRessourcesDemandées; i++)
    {
        if(pthread_mutex_trylock(&resources[r[i]]) != 0)
        {
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
    pthread_mutex_unlock(&resources[numero_ressource]);
    return 0;
}