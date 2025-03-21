enum res
{
    //*** Réseau de Petri de la ressource 1 ***//

    R1_free,
    Req_R1_TR,
    R1_TR_Alloc,
    Ach_R1_TR,
    R1_TR_Wait,
    Res_R1_TR,
    RAch_R1_TR,
    Req_R1_TV,
    Ach_R1_TV,
    R1_TV_Wait,
    Res_R1_TV,
    RAch_R1_TV,
    // deuxième graphe Gestionnaire de ressources 1
    Req_R1_TJ_1,
    Ach_R1_TJ_1,
    R1_TJ_Wait_1,
    Res_R1_TJ_1,
    RAch_R1_TJ_1,
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
    Rach_R2_TB
};

int prendRessource(int*, int);
int lacheRessource(int);