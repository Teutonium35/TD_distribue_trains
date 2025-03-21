#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <math.h>
#include <gestionRessources.h>
#include <gestionMutex.h>

// Réseau de Petri de la ressource 1
void* gestion_R1(void* arg) {
    
    int* r1_tr_alloc = malloc(2*sizeof(int));
    r1_tr_alloc[0] = R1_free;
    r1_tr_alloc[1] = Req_R1_TR;
    // il manque la ressource R6_free pour le moment !

    int* r1_tv_alloc = malloc(2*sizeof(int));
    r1_tv_alloc[0] = R1_free;
    r1_tv_alloc[1] = Req_R1_TV;

    int* r1_tr_rest = malloc(2*sizeof(int));
    r1_tr_rest[0] = Res_R1_TR;
    r1_tr_rest[1] = R1_TR_Wait;

    int* r1_tv_rest = malloc(2*sizeof(int));
    r1_tv_rest[0] = R1_TV_Wait;
    r1_tv_rest[1] = Res_R1_TV;

    int* r1_tj_alloc_1 = malloc(2*sizeof(int));
    r1_tj_alloc_1[0] = R1_free;
    r1_tj_alloc_1[1] = Req_R1_TJ_1;
    // il manque la ressource R5_free pour le moment !

    int* r1_tj_rest_1 = malloc(2*sizeof(int));
    r1_tj_rest_1[0] = R1_TJ_Wait_1;
    r1_tj_rest_1[1] = Res_R1_TJ_1;

    int* r1_tj_alloc_2 = malloc(2*sizeof(int));
    r1_tj_alloc_2[0] = Req_R1_TJ_2;
    r1_tj_alloc_2[1] = R1_free;


    int* r1_tj_rest_2 = malloc(2*sizeof(int));
    r1_tj_rest_2[0] = Res_R1_TJ_2;
    r1_tj_rest_2[1] = Rach_R1_TJ_2;

    // pour l'instant on considère que la transition est faite à chaque fois instantanément
    int R1_TR_Alloc = 1; // transition
    int R1_TV_Alloc = 1; // transition
    int R1_TR_Rest = 1; // transition
    int R1_TV_Rest = 1; // transition
    int R1_TJ_Alloc_1 = 1; // transition
    int R1_TJ_Rest_1 = 1; // transition
    int R1_TJ_Alloc_2 = 1; // transition
    int R1_TJ_Rest_2 = 1; // transition

    while(1)
    {
         // si la transition R1_TR_Alloc est activée
        if(R1_TR_Alloc)
        {
            if(prendRessource(r1_tr_alloc,2) == 0)
            {
                lacheRessource(R1_TR_Wait);
                printf("Ressource 1 est dans l'état R1_TR_Wait \n");
            }
        }

        // si la transition R1_TV_Alloc est activée
        if(R1_TV_Alloc)
        {
            if(prendRessource(r1_tv_alloc,2) == 0)
            {
                lacheRessource(Ach_R1_TV);
                lacheRessource(R1_TV_Wait);
                printf("Ressource 1 est dans l'état R1_TV_Wait \n");
            }
        }

        // si la transition R1_TR_Rest est activée
        if(R1_TR_Rest)
        {
            if(prendRessource(r1_tr_rest ,2) == 0)
            {
                lacheRessource(RAch_R1_TR); // sur le réseau de Petri c'est RAch_R1_TV, je pense que c'est une erreur
                lacheRessource(R1_free);
                printf("Ressource 1 est dans l'état R1_free \n");
            }
        }

        // si la transition R1_TV_Rest est activée
        if(R1_TV_Rest)
        {
            if(prendRessource(r1_tv_rest ,2) == 0)
            {
                lacheRessource(RAch_R1_TV);
                lacheRessource(R1_free);
                printf("Ressource 1 est dans l'état R1_free \n");
            }
        }

        // si la transition R1_TV_Rest est activée
        if(R1_TJ_Alloc_1)
        {
            if(prendRessource(r1_tj_alloc_1 ,2) == 0)
            {
                lacheRessource(Ach_R1_TJ_1);
                lacheRessource(R1_TJ_Wait_1);
                printf("Ressource 1 est dans l'état R1_TJ_Wait_1 \n");
            }
        }

        // si la transition R1_TV_Rest est activée
        if(R1_TJ_Rest_1)
        {
            if(prendRessource(r1_tj_rest_1 ,2) == 0)
            {
                lacheRessource(RAch_R1_TJ_1);
                lacheRessource(R1_free);
                printf("Ressource 1 est dans l'état R1_free \n");
            }
        }

        // si la transition R1_TJ_Alloc_2 est activée
        if(R1_TJ_Alloc_2)
        {
            if(prendRessource(r1_tj_alloc_2 ,2) == 0)
            {
                lacheRessource(Ach_R1_TJ_2);
                lacheRessource(R1_TJ_Wait_2);
                printf("Ressource 1 est dans l'état R1_TJ_Wait_2 \n");
            }
        }

        // si la transition R1_TJ_Rest_2 est activée
        if(R1_TJ_Rest_2)
        {
            if(prendRessource(r1_tj_rest_1 ,2) == 0)
            {
                lacheRessource(Rach_R1_TJ_2);
                lacheRessource(R1_free);
                printf("Ressource 1 est dans l'état R1_free \n");
            }
        }
    }
   return NULL;
}

// Réseau de Petri de la ressource 2
void* gestion_R2(void* arg) {
    
    int* r2_tv_alloc = malloc(2*sizeof(int));
    r2_tv_alloc[0] = R1_free;
    r2_tv_alloc[1] = Req_R1_TR;
    // il manque la ressource R5_free pour le moment !

    int* r2_tv_rest = malloc(2*sizeof(int));
    r2_tv_rest[0] = R2_TV_Wait;
    r2_tv_rest[1] = Res_R2_TV;

    int* r2_tj_alloc = malloc(2*sizeof(int));
    r2_tj_alloc[0] = R2_free;
    r2_tj_alloc[1] = Req_R2_TJ;
    // il manque les ressources R3_free et R6_free pour le moment !

    int* r2_tj_rest = malloc(2*sizeof(int));
    r2_tj_rest[0] = R2_TJ_Wait;
    r2_tj_rest[1] = Res_R2_TJ;

    int* r2_tb_alloc = malloc(2*sizeof(int));
    r2_tb_alloc[0] = Req_R2_TB;
    r2_tb_alloc[1] = R2_free;


    int* r2_tb_rest = malloc(2*sizeof(int));
    r2_tb_rest[0] = R2_TB_Wait;
    r2_tb_rest[1] = Res_R2_TB;

    // pour l'instant on considère que la transition est faite à chaque fois instantanément
    int R2_TV_Alloc = 1; // transition
    int R2_TV_Rest = 1; // transition
    int R2_TJ_Alloc = 1; // transition
    int R2_TJ_Rest = 1; // transition
    int R2_TB_Alloc = 1; // transition
    int R2_TB_Rest = 1; // transition

    while(1)
    {
        if(R2_TV_Alloc)
        {
            if(prendRessource(r2_tv_alloc,2) == 0)
            {
                lacheRessource(R2_TV_Wait);
                lacheRessource(Ach_R2_TV);
                printf("Ressource 2 est dans l'état R2_TV_Wait \n");
            }
        }

        if(R2_TV_Rest)
        {
            if(prendRessource(r2_tv_rest,2) == 0)
            {
                lacheRessource(Rach_R2_TV);
                lacheRessource(R2_free);
                printf("Ressource 2 est dans l'état R2_free \n");
            }
        }

        if(R2_TJ_Alloc)
        {
            if(prendRessource(r2_tj_alloc ,2) == 0)
            {
                lacheRessource(Ach_R2_TJ); // sur le réseau de Petri c'est RAch_R1_TV, je pense que c'est une erreur
                lacheRessource(R2_TJ_Wait);
                printf("Ressource 2 est dans l'état R2_TJ_Wait \n");
            }
        }

        if(R2_TJ_Rest)
        {
            if(prendRessource(r2_tj_rest ,2) == 0)
            {
                lacheRessource(Rach_R1R2_TJ); // sur le réseau de Petri c'est RAch_R1R2_TV, je pense que c'est une erreur
                lacheRessource(R2_free);
                printf("Ressource 2 est dans l'état R2_free \n");
            }
        }

        if(R2_TB_Alloc)
        {
            if(prendRessource(r2_tb_alloc ,2) == 0)
            {
                lacheRessource(Ach_R2_TB);
                lacheRessource(R2_TB_Wait);
                printf("Ressource 2 est dans l'état R2_TB_Wait \n");
            }
        }

        if(R2_TB_Rest)
        {
            if(prendRessource(r2_tb_rest ,2) == 0)
            {
                lacheRessource(Rach_R2_TB);
                lacheRessource(R2_free);
                printf("Ressource 2 est dans l'état R2_free \n");
            }
        }
    }
   return NULL;
}