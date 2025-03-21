#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
// le gestionnaire de ressources va créer un thread à chaque requête d'un des trains. Ce thread va atendre la libération de la ressource demandée et envoyer 
// au gestionnaire adéquat qu'elle est libérée quand c'est le cas

// coder transitionPetri() - actionPetri() comme en VBA

void printState(char* Etat)
{
    printf("Dans l'etat %s \n", Etat);
}

int demandeRessource(int r)
{
    printf("Demande de la ressource %d \n", r);
    return 1;
}

int main()
{
    printState("Start");
    return 0;
}