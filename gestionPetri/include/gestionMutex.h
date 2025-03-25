#define NOMBRE_RESSOURCES 500

enum res
{
    //*** Réseau de Petri de la ressource 1 ***//

    R1_free,
    Req_R1_TR,
    R1_TR_Alloc,
    Ach_R1_TR,
    R1_TR_Wait,
    Res_R1_TR,
    Rach_R1_TR,
    Req_R1_TV,
    Ach_R1_TV,
    R1_TV_Wait,
    Res_R1_TV,
    Rach_R1_TV,
    // deuxième graphe Gestionnaire de ressources 1
    Req_R1_TJ_1,
    Ach_R1_TJ_1,
    R1_TJ_Wait_1,
    Res_R1_TJ_1,
    Rach_R1_TJ_1,
    Req_R1_TJ_2,
    Ach_R1_TJ_2,
    R1_TJ_Wait_2,
    Res_R1_TJ_2,
    Rach_R1_TJ_2,

    //*** Réseau de Petri de la ressource 2 ***//

    R2_free,
    Req_R2_TV,
    Ach_R2_TV,
    R2_TV_Wait,
    Res_R2_TV,
    Rach_R2_TV,
    Req_R2_TJ,
    Ach_R2_TJ,
    R2_TJ_Wait,
    Res_R2_TJ,
    Rach_R1R2_TJ, // il y a écrit RAch_R1R2_TV sur le schéma, je pense que c'est une erreur
    Req_R2_TB,
    Ach_R2_TB,
    R2_TB_Wait,
    Res_R2_TB,
    Rach_R2_TB,

    //*** Réseau de Petri de la ressource 3 ***//

    R3_free,
    Req_R3_TR,
    Ach_R3_TR,
    R3_TR_Wait,
    Res_R3_TR,
    Rach_R3_TR,
    Req_R3_TB,
    Ach_R3_TB,
    R3_TB_Wait,
    Res_R3_TB,
    Rach_R3_TB,
    Req_R3_TJ,
    Ach_R3_TJ,
    R3_TJ_Wait,
    Res_R3_TJ,
    Rach_R3_TJ,

    //*** Réseau de Petri de la ressource 4 ***//

    R4_free,
    Req_R4_TB,
    Ach_R4_TB,
    R4_TB_Wait,
    Res_R4_TB,
    Rach_R4_TB,
    Req_R4_TV,
    Ach_R4_TV,
    R4_TV_Wait,
    Res_R4_TV,
    Rach_R4_TV,

    //*** Réseau de Petri de la ressource 5 ***//

    R5_free,
    Req_R5_TV,
    Ach_R5_TV,
    R5_TV_Wait,
    Res_R5_TV,
    Rach_R5_TV,
    Req_R5_TJ,
    Ach_R5_TJ,
    R5_TJ_Wait,
    Res_R5_TJ,
    Rach_R5_TJ,

    //*** Réseau de Petri de la ressource 6 ***//

    R6_free,
    Req_R6_TJ,
    Ach_R6_TJ,
    R6_TJ_Wait,
    Res_R6_TJ,
    Rach_R6_TJ,
    Req_R6_TR,
    Ach_R6_TR,
    R6_TR_Wait,
    Res_R6_TR,
    Rach_R6_TR
};

int prendRessource(int*, int);
int lacheRessource(int);