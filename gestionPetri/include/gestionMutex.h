enum res
{
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
};

int prendRessource(int*, int);
int lacheRessource(int);