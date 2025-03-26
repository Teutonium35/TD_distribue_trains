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
    
    int* r1_tr_alloc = malloc(3*sizeof(int));
    r1_tr_alloc[0] = R1_free;
    r1_tr_alloc[1] = Req_R1_TR;
    r1_tr_alloc[2] = R6_free;

    int* r1_tv_alloc = malloc(2*sizeof(int));
    r1_tv_alloc[0] = R1_free;
    r1_tv_alloc[1] = Req_R1_TV;

    int* r1_tr_rest = malloc(2*sizeof(int));
    r1_tr_rest[0] = Res_R1_TR;
    r1_tr_rest[1] = R1_TR_Wait;

    int* r1_tv_rest = malloc(2*sizeof(int));
    r1_tv_rest[0] = R1_TV_Wait;
    r1_tv_rest[1] = Res_R1_TV;

    int* r1_tj_alloc_1 = malloc(3*sizeof(int));
    r1_tj_alloc_1[0] = R1_free;
    r1_tj_alloc_1[1] = Req_R1_TJ_1;
    r1_tj_alloc_1[2] = R5_free;

    int* r1_tj_rest_1 = malloc(2*sizeof(int));
    r1_tj_rest_1[0] = R1_TJ_Wait_1;
    r1_tj_rest_1[1] = Res_R1_TJ_1;

    int* r1_tj_alloc_2 = malloc(2*sizeof(int));
    r1_tj_alloc_2[0] = Req_R1_TJ_2;
    r1_tj_alloc_2[1] = R1_free;


    int* r1_tj_rest_2 = malloc(2*sizeof(int));
    r1_tj_rest_2[0] = Res_R1_TJ_2;
    r1_tj_rest_2[1] = Rach_R1_TJ_2;


    while(1)
    {
        if(prendRessource(r1_tr_alloc,3) == 0)
        {
            lacheRessource(R1_TR_Wait);
            printf("Ressource 1 est dans l'état R1_TR_Wait \n");
        }

        else if(prendRessource(r1_tv_alloc,2) == 0)
        {
            lacheRessource(Ach_R1_TV);
            lacheRessource(R1_TV_Wait);
            printf("Ressource 1 est dans l'état R1_TV_Wait \n");
        }

        else if(prendRessource(r1_tr_rest ,2) == 0)
        {
            lacheRessource(Rach_R1_TR); // sur le réseau de Petri c'est RAch_R1_TV, je pense que c'est une erreur
            lacheRessource(R1_free);
            printf("Ressource 1 est dans l'état R1_free \n");
        }

        else if(prendRessource(r1_tv_rest ,2) == 0)
        {
            lacheRessource(Rach_R1_TV);
            lacheRessource(R1_free);
            printf("Ressource 1 est dans l'état R1_free \n");
        }

        else if(prendRessource(r1_tj_alloc_1 ,3) == 0)
        {
            lacheRessource(Ach_R1_TJ_1);
            lacheRessource(R1_TJ_Wait_1);
            printf("Ressource 1 est dans l'état R1_TJ_Wait_1 \n");
        }

        else if(prendRessource(r1_tj_rest_1 ,2) == 0)
        {
            lacheRessource(Rach_R1_TJ_1);
            lacheRessource(R1_free);
            printf("Ressource 1 est dans l'état R1_free \n");
        }

        else if(prendRessource(r1_tj_alloc_2 ,2) == 0)
        {
            lacheRessource(Ach_R1_TJ_2);
            lacheRessource(R1_TJ_Wait_2);
            printf("Ressource 1 est dans l'état R1_TJ_Wait_2 \n");
        }

        else if(prendRessource(r1_tj_rest_1 ,2) == 0)
        {
            lacheRessource(Rach_R1_TJ_2);
            lacheRessource(R1_free);
            printf("Ressource 1 est dans l'état R1_free \n");
        }
    }
   return NULL;
}

// Réseau de Petri de la ressource 2
void* gestion_R2(void* arg) {
    
    int* r2_tv_alloc = malloc(3*sizeof(int));
    r2_tv_alloc[0] = R2_free;
    r2_tv_alloc[1] = Req_R2_TV;
    r2_tv_alloc[2] = R5_free;

    int* r2_tv_rest = malloc(2*sizeof(int));
    r2_tv_rest[0] = R2_TV_Wait;
    r2_tv_rest[1] = Res_R2_TV;

    int* r2_tj_alloc = malloc(4*sizeof(int));
    r2_tj_alloc[0] = R2_free;
    r2_tj_alloc[1] = Req_R2_TJ;
    r2_tj_alloc[2] = R3_free;
    r2_tj_alloc[3] = R6_free;

    int* r2_tj_rest = malloc(2*sizeof(int));
    r2_tj_rest[0] = R2_TJ_Wait;
    r2_tj_rest[1] = Res_R2_TJ;

    int* r2_tb_alloc = malloc(2*sizeof(int));
    r2_tb_alloc[0] = Req_R2_TB;
    r2_tb_alloc[1] = R2_free;


    int* r2_tb_rest = malloc(2*sizeof(int));
    r2_tb_rest[0] = R2_TB_Wait;
    r2_tb_rest[1] = Res_R2_TB;


    while(1)
    {
        if(prendRessource(r2_tv_alloc,3) == 0)
        {
            lacheRessource(R2_TV_Wait);
            lacheRessource(Ach_R2_TV);
            printf("Ressource 2 est dans l'état R2_TV_Wait \n");
        }

        else if(prendRessource(r2_tv_rest,2) == 0)
        {
            lacheRessource(Rach_R2_TV);
            lacheRessource(R2_free);
            printf("Ressource 2 est dans l'état R2_free \n");
        }

        else if(prendRessource(r2_tj_alloc ,4) == 0)
        {
            lacheRessource(Ach_R2_TJ); // sur le réseau de Petri c'est Rach_R1_TV, je pense que c'est une erreur
            lacheRessource(R2_TJ_Wait);
            printf("Ressource 2 est dans l'état R2_TJ_Wait \n");
        }

        else if(prendRessource(r2_tj_rest ,2) == 0)
        {
            lacheRessource(Rach_R1R2_TJ); // sur le réseau de Petri c'est Rach_R1R2_TV, je pense que c'est une erreur
            lacheRessource(R2_free);
            printf("Ressource 2 est dans l'état R2_free \n");
        }

        else if(prendRessource(r2_tb_alloc ,2) == 0)
        {
            lacheRessource(Ach_R2_TB);
            lacheRessource(R2_TB_Wait);
            printf("Ressource 2 est dans l'état R2_TB_Wait \n");
        }

        else if(prendRessource(r2_tb_rest ,2) == 0)
        {
            lacheRessource(Rach_R2_TB);
            lacheRessource(R2_free);
            printf("Ressource 2 est dans l'état R2_free \n");
        }
    }
   return NULL;
}

// Réseau de Petri de la ressource 3
void* gestion_R3(void* arg) {
    
    int* r3_tr_alloc = malloc(2*sizeof(int));
    r3_tr_alloc[0] = R3_free;
    r3_tr_alloc[1] = Req_R3_TR;

    int* r3_tr_rest = malloc(2*sizeof(int));
    r3_tr_rest[0] = R3_TR_Wait;
    r3_tr_rest[1] = Res_R3_TR;

    int* r3_tb_alloc = malloc(3*sizeof(int));
    r3_tb_alloc[0] = R3_free;
    r3_tb_alloc[1] = Req_R3_TB;
    r3_tb_alloc[2] = R4_free;

    int* r3_tb_rest = malloc(2*sizeof(int));
    r3_tb_rest[0] = R3_TB_Wait;
    r3_tb_rest[1] = Res_R3_TB;

    int* r3_tj_alloc = malloc(3*sizeof(int));
    r3_tj_alloc[0] = Req_R3_TJ;
    r3_tj_alloc[1] = R2_free;
    r3_tj_alloc[2] = R6_free;

    int* r3_tj_rest = malloc(2*sizeof(int));
    r3_tj_rest[0] = R3_TJ_Wait;
    r3_tj_rest[1] = Res_R3_TJ;


    while(1)
    {
        if(prendRessource(r3_tr_alloc,2) == 0)
        {
            lacheRessource(R3_TR_Wait);
            lacheRessource(Ach_R3_TR);
            printf("Ressource 3 est dans l'état R3_TR_Wait \n");
        }

        else if(prendRessource(r3_tr_rest,2) == 0)
        {
            lacheRessource(Rach_R3_TR);
            lacheRessource(R3_free);
            printf("Ressource 3 est dans l'état R3_free \n");
        }

        else if(prendRessource(r3_tb_alloc,3) == 0)
        {
            lacheRessource(Ach_R3_TB); // sur le réseau de Petri c'est Rach_R1_TV, je pense que c'est une erreur
            lacheRessource(R3_TB_Wait);
            printf("Ressource 3 est dans l'état R3_TB_Wait \n");
        }

        else if(prendRessource(r3_tb_rest,2) == 0)
        {
            lacheRessource(Rach_R3_TB); 
            lacheRessource(R3_free);
            printf("Ressource 3 est dans l'état R3_TB_Wait \n");
        }

        else if(prendRessource(r3_tj_alloc ,3) == 0)
        {
            lacheRessource(Ach_R3_TJ); // sur le réseau de Petri c'est Rach_R1R2_TV, je pense que c'est une erreur
            lacheRessource(R3_TJ_Wait);
            printf("Ressource 3 est dans l'état R3_TJ_Wait \n");
        }

        else if(prendRessource(r3_tj_rest ,2) == 0)
        {
            lacheRessource(Rach_R3_TJ);
            lacheRessource(R3_free);
            printf("Ressource 3 est dans l'état R3_free \n");
        }
    }
   return NULL;
}

// Réseau de Petri de la ressource 4
void* gestion_R4(void* arg) {
    
    int* r4_tb_alloc = malloc(3*sizeof(int));
    r4_tb_alloc[0] = R3_free;
    r4_tb_alloc[1] = R4_free;
    r4_tb_alloc[2] = Req_R4_TB;

    int* r4_tb_rest = malloc(2*sizeof(int));
    r4_tb_rest[0] = R4_TB_Wait;
    r4_tb_rest[1] = Res_R4_TB;

    int* r4_tv_alloc = malloc(2*sizeof(int));
    r4_tv_alloc[0] = R4_free;
    r4_tv_alloc[1] = Req_R4_TV;

    int* r4_tv_rest = malloc(2*sizeof(int)); // il y a marqué r4_tv_wait dans le Petri mais je pense que c'est une erreur
    r4_tv_rest[0] = R4_TV_Wait;
    r4_tv_rest[1] = Res_R4_TV;

    while(1)
    {
        if(prendRessource(r4_tb_alloc,3) == 0) 
        {
            lacheRessource(R4_TB_Wait);
            lacheRessource(Ach_R4_TB);
            printf("Ressource 4 est dans l'état R4_TB_Wait \n");
        }

        else if(prendRessource(r4_tb_rest,2) == 0)
        {
            lacheRessource(Rach_R4_TB);
            lacheRessource(R4_free);
            printf("Ressource 4 est dans l'état R4_free \n");
        }

        else if(prendRessource(r4_tv_alloc,2) == 0)
        {
            lacheRessource(Ach_R4_TV); // sur le réseau de Petri c'est Rach_R1_TV, je pense que c'est une erreur
            lacheRessource(R4_TV_Wait);
            printf("Ressource 4 est dans l'état R4_TV_Wait \n");
        }

        else if(prendRessource(r4_tv_rest,2) == 0)
        {
            lacheRessource(Rach_R4_TV); 
            lacheRessource(R4_free);
            printf("Ressource 4 est dans l'état R4_free \n");
        }
    }
   return NULL;
}

// Réseau de Petri de la ressource 5
void* gestion_R5(void* arg) {
    
    int* r5_tv_alloc = malloc(3*sizeof(int));
    r5_tv_alloc[0] = R2_free;
    r5_tv_alloc[1] = R5_free;
    r5_tv_alloc[2] = Req_R5_TV;

    int* r5_tv_rest = malloc(2*sizeof(int));
    r5_tv_rest[0] = R5_TV_Wait;
    r5_tv_rest[1] = Res_R5_TV;

    int* r5_tj_alloc = malloc(3*sizeof(int));
    r5_tj_alloc[0] = R1_free;
    r5_tj_alloc[1] = R5_free;
    r5_tj_alloc[2] = Req_R5_TJ; 

    int* r5_tj_rest = malloc(2*sizeof(int)); // il y a marqué r5_tj_wait dans le Petri mais je pense que c'est une erreur
    r5_tj_rest[0] = R5_TJ_Wait;
    r5_tj_rest[1] = Res_R5_TJ;

    while(1)
    {
        if(prendRessource(r5_tv_alloc,3) == 0) 
        {
            lacheRessource(R5_TV_Wait);
            lacheRessource(Ach_R5_TV);
            printf("Ressource 5 est dans l'état R5_TV_Wait \n");
        }

        if(prendRessource(r5_tv_rest,2) == 0)
        {
            lacheRessource(Rach_R5_TV);
            lacheRessource(R5_free);
            printf("Ressource 5 est dans l'état R5_free \n");
        }

        if(prendRessource(r5_tj_alloc,3) == 0)
        {
            lacheRessource(Ach_R5_TJ); // sur le réseau de Petri c'est Rach_R1_TV, je pense que c'est une erreur
            lacheRessource(R5_TV_Wait);
            printf("Ressource 5 est dans l'état R5_TJ_Wait \n");
        }

        if(prendRessource(r5_tj_rest,2) == 0)
        {
            lacheRessource(Rach_R5_TJ); 
            lacheRessource(R5_free);
            printf("Ressource 5 est dans l'état R5_free \n");
        }
    }
   return NULL;
}

// Réseau de Petri de la ressource 6
void* gestion_R6(void* arg) {
    
    int* r6_tj_alloc = malloc(4*sizeof(int));
    r6_tj_alloc[0] = R6_free;
    r6_tj_alloc[1] = R3_free;
    r6_tj_alloc[2] = R2_free;
    r6_tj_alloc[3] = Req_R6_TJ;

    int* r6_tj_rest = malloc(2*sizeof(int));
    r6_tj_rest[0] = R6_TJ_Wait;
    r6_tj_rest[1] = Res_R6_TJ;

    int* r6_tr_alloc = malloc(3*sizeof(int));
    r6_tr_alloc[0] = R6_free;
    r6_tr_alloc[1] = R1_free;
    r6_tr_alloc[2] = Req_R6_TR; 

    int* r6_tr_rest = malloc(2*sizeof(int)); // il y a marqué r6_tr_wait dans le Petri mais je pense que c'est une erreur
    r6_tr_rest[0] = R6_TR_Wait;
    r6_tr_rest[1] = Res_R6_TR;

    while(1)
    {
        if(prendRessource(r6_tj_alloc,4) == 0) 
        {
            lacheRessource(R6_TJ_Wait);
            lacheRessource(Ach_R6_TJ);
            printf("Ressource 6 est dans l'état R6_TJ_Wait \n");
        }

        else if(prendRessource(r6_tj_rest,2) == 0)
        {
            lacheRessource(Rach_R6_TJ);
            lacheRessource(R6_free);
            printf("Ressource 6 est dans l'état R6_free \n");
        }

        else if(prendRessource(r6_tr_alloc,3) == 0)
        {
            lacheRessource(Ach_R6_TR); // sur le réseau de Petri c'est Rach_R1_TV, je pense que c'est une erreur
            lacheRessource(R6_TR_Wait);
            printf("Ressource 6 est dans l'état R6_TR_Wait \n");
        }

        else if(prendRessource(r6_tr_rest,2) == 0)
        {
            lacheRessource(Rach_R6_TR); 
            lacheRessource(R6_free);
            printf("Ressource 6 est dans l'état R6_free \n");
        }
    }
   return NULL;
}