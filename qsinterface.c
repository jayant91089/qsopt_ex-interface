#include <stdio.h>
#include <stdlib.h>
#include <QSopt_ex.h>
#include <gmp.h>
/*****************************************************************************/
/* String format of input
 * argv[1] = 0 for integers, 1 for rationals (only for cmatval, obj and rhs rest always integers)
 * argv[2] = nb_rows
 * argv[3] = nb_cols
 * argv[4] = nb_obj 
 * argv[5..nb_obj+4] = obj (repeat for denominators)
 * argv[nb_obj+5] = nb_cmatval
 * argv[nb_obj+6..nb_obj+nb_cmatval+5] = cmatval (repeat for denominators)
 * argv[nb_obj+nb_cmatal+6] = nb_cmatcnt
 * argv[nb_obj+nb_cmatval+7..nb_obj+nb_cmatval+nb_cmatcnt+6] = cmatcnt
 * argv[nb_obj+nb_cmatval+nb_cmatcnt+7] = nb_cmatind
 * argv[nb_obj+nb_cmatval+nb_cmatcnt+8..nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+7] = cmatind
 * argv[nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+8] = nb_sense
 * argv[nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+9..nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+nb_sense+8] = sense
 * argv[nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+nb_sense+9] = nb_rhs
 * argv[nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+nb_sense+10..nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+nb_sense+nb_rhs+9]
 * argv[nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+nb_sense+nb_rhs+10] = nb_cmatbeg
 * argv[nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+nb_sense+nb_rhs+11..
 * nb_obj+nb_cmatval+nb_cmatcnt+nb_cmatind+nb_sense+nb_rhs+nb_cmatbeg+10] =cmatbeg
 * */
int main (long argc, char** argv){
    long i=1;// current index
    long j,nb_cmatval,nb_cmatcnt,nb_cmatind,nb_sense,nb_obj,nb_rhs;
    mpq_t *obj,*cmatval,*rhs, *lower,*upper,*x; 
    int *cmatcnt, *cmatbeg, *cmatind;
    char* sense,**rownames,**colnames;
    int nb_rows,nb_cols,rval,status;
    mpq_QSprob p = (mpq_QSprob) NULL;
    mpq_t value;
    mpq_init(value);
    QSexactStart();
    QSexact_set_precision (128);
    //p=(mpq_QSprob*)malloc(sizeof(mpq_QSprob)); 
    if(strtol(argv[i],0,10)==1) // integers only
    {
        i++;
        nb_rows=strtol(argv[i],0,10);
        i++;
        nb_cols=strtol(argv[i],0,10);
        i++;
        nb_obj=strtol(argv[i],0,10);
        i++;
        obj=(mpq_t*)malloc(nb_obj*sizeof(mpq_t));
        for(j=i;j<i+nb_obj;j++) 
        {   
            mpq_init(obj[j-i]);
            mpq_set_si(obj[j-i],strtol(argv[j],0,10),1);
        }
        i=j;
        nb_cmatval=strtol(argv[i],0,10);
        i++;
        cmatval=(mpq_t*)malloc(nb_cmatval*sizeof(mpq_t));
        for(j=i;j<i+nb_cmatval;j++) 
        {
            mpq_init(cmatval[j-i]);
            mpq_set_si(cmatval[j-i],strtol(argv[j],0,10),1);
        }
        i=j;
        nb_cmatcnt=strtol(argv[i],0,10);
        i++;
        cmatcnt=(int*)malloc(nb_cmatcnt*sizeof(int));
        for(j=i;j<i+nb_cmatcnt;j++) 
        {
            cmatcnt[j-i]=strtol(argv[j],0,10);
        }
        i=j;
        nb_cmatind=strtol(argv[i],0,10);
        i++;
        cmatind=(int*)malloc(nb_cmatind*sizeof(int));
        for(j=i;j<i+nb_cmatind;j++) 
        {
            cmatind[j-i]=strtol(argv[j],0,10);
        }
        i=j;
        nb_sense=strtol(argv[i],0,10);
        i++;
        sense=(char*)malloc(nb_sense*sizeof(char));
        for(j=i;j<i+nb_sense;j++) 
        {
            sense[j-i]=(strtol(argv[j],0,10)==1)?'E':'L';
        }
        i=j;
        nb_rhs=strtol(argv[i],0,10);
        i++;
        rhs=(mpq_t*)malloc(nb_rhs*sizeof(mpq_t));
        for(j=i;j<i+nb_rhs;j++) 
        {
            mpq_init(rhs[j-i]);
            mpq_set_si(rhs[j-i],strtol(argv[j],0,10),1);
        }
        i=j;
        // print obj:
        printf("objective:\n");
        for (j=0;j<nb_obj;j++)
        gmp_printf("%Qd ", obj[j]);
        printf("\n");
        // print cmatval:
        printf("cmatval:\n");
        for (j=0;j<nb_cmatval;j++)
        gmp_printf("%Qd  ", cmatval[j]);
        printf("\n");
         // print cmatcnt:
        printf("cmatcnt:\n");
        for (j=0;j<nb_cmatcnt;j++)
        gmp_printf("%d  ", cmatcnt[j]);
        printf("\n");
         // print cmatind:
        printf("cmatind:\n");
        for (j=0;j<nb_cmatind;j++)
        gmp_printf("%d  ", cmatind[j]);
        printf("\n");
         // print sense:
        printf("Sense:\n");
        for (j=0;j<nb_sense;j++)
        printf("%c  ", sense[j]);
        printf("\n");
        // print rhs:
        printf("rhs:\n");
        for (j=0;j<nb_rhs;j++)
        gmp_printf("%Qd ", rhs[j]);
        printf("\n");
    }
    else // rationals
    {
        i++;
        nb_rows=strtol(argv[i],0,10);
        i++;
        nb_cols=strtol(argv[i],0,10);
        i++;
        nb_obj=strtol(argv[i],0,10);
        i++;
        obj=(mpq_t*)malloc(nb_obj*sizeof(mpq_t));
        long* objnum=(long*)malloc(nb_obj*sizeof(long));
        long* objden=(long*)malloc(nb_obj*sizeof(long));
        for(j=i;j<i+nb_obj;j++) 
           objnum[j-i]=strtol(argv[j],0,10);
        i=j;
        for(j=i;j<i+nb_obj;j++) 
           objden[j-i]=strtol(argv[j],0,10);
        i=j;
        for(j=0;j<nb_obj;j++) 
        {   
            mpq_init(obj[j]);
            mpq_set_si(obj[j],objnum[j],objden[j]);
        }
        nb_cmatval=strtol(argv[i],0,10);
        i++;
        cmatval=(mpq_t*)malloc(nb_cmatval*sizeof(mpq_t));
        long* valnum=(long*)malloc(nb_cmatval*sizeof(long));
        long* valden=(long*)malloc(nb_cmatval*sizeof(long));
        for(j=i;j<i+nb_cmatval;j++) 
           valnum[j-i]=strtol(argv[j],0,10);
        i=j;
        for(j=i;j<i+nb_cmatval;j++) 
           valden[j-i]=strtol(argv[j],0,10);
        i=j;
        for(j=0;j<nb_cmatval;j++) 
        {
            mpq_init(cmatval[j]);
            printf("%ld/%ld\n",valnum[j],valden[j]);
            mpq_set_si(cmatval[j],valnum[j],valden[j]);
        }
        nb_cmatcnt=strtol(argv[i],0,10);
        i++;
        cmatcnt=(int*)malloc(nb_cmatcnt*sizeof(int));
        for(j=i;j<i+nb_cmatcnt;j++) 
        {
            cmatcnt[j-i]=strtol(argv[j],0,10);
        }
        i=j;
        nb_cmatind=strtol(argv[i],0,10);
        i++;
        cmatind=(int*)malloc(nb_cmatind*sizeof(int));
        for(j=i;j<i+nb_cmatind;j++) 
        {
            cmatind[j-i]=strtol(argv[j],0,10);
        }
        i=j;
        nb_sense=strtol(argv[i],0,10);
        i++;
        sense=(char*)malloc(nb_sense*sizeof(char));
        for(j=i;j<i+nb_sense;j++) 
        {
            sense[j-i]=(strtol(argv[j],0,10)==1)?'E':'L';
        }
        i=j;
        nb_rhs=strtol(argv[i],0,10);
        i++;
        rhs=(mpq_t*)malloc(nb_rhs*sizeof(mpq_t));
        long* rhsnum=(long*)malloc(nb_rhs*sizeof(long));
        long* rhsden=(long*)malloc(nb_rhs*sizeof(long));
        for(j=i;j<i+nb_rhs;j++) 
           rhsnum[j-i]=strtol(argv[j],0,10);
        i=j;
        for(j=i;j<i+nb_rhs;j++) 
           rhsden[j-i]=strtol(argv[j],0,10);
        i=j;
        for(j=0;j<nb_rhs;j++) 
        {
            mpq_init(rhs[j]);
            mpq_set_si(rhs[j],rhsnum[j],rhsden[j]);
        }
        // print obj:
        printf("objective:\n");
        for (j=0;j<nb_obj;j++)
        gmp_printf("%Qd ", obj[j]);
        printf("\n");
        // print cmatval:
        printf("cmatval:\n");
        for (j=0;j<nb_cmatval;j++)
        gmp_printf("%Qd  ", cmatval[j]);
        printf("\n");
         // print cmatcnt:
        printf("cmatcnt:\n");
        for (j=0;j<nb_cmatcnt;j++)
        gmp_printf("%d  ", cmatcnt[j]);
        printf("\n");
         // print cmatind:
        printf("cmatind:\n");
        for (j=0;j<nb_cmatind;j++)
        gmp_printf("%d  ", cmatind[j]);
        printf("\n");
         // print sense:
        printf("Sense:\n");
        for (j=0;j<nb_sense;j++)
        printf("%c  ", sense[j]);
        printf("\n");
        // print rhs:
        printf("rhs:\n");
        for (j=0;j<nb_rhs;j++)
        gmp_printf("%Qd ", rhs[j]);
        printf("\n");
    }
    // construct cmatbeg from cmatcnt
    cmatbeg=malloc(nb_cmatcnt*sizeof(int));
    cmatbeg[0]=0;
    for(j=1;j<nb_cmatcnt;j++)
    cmatbeg[j]=cmatbeg[j-1]+cmatcnt[j-1];
    printf("cmatbeg:\n");
    for (j=0;j<nb_cmatcnt;j++)
    printf("%d ", cmatbeg[j]);
    printf("\n");
    lower=(mpq_t*)malloc(nb_cols*sizeof(mpq_t));// lower bounds 
    for (i = 0; i < nb_cols; i++) 
    {
        mpq_init (lower[i]); // fill in the lower bounds
        mpq_set (lower[i], mpq_ILL_MINDOUBLE);
    }
    upper=(mpq_t*)malloc(nb_cols*sizeof(mpq_t));// upper bounds
    for (i = 0; i < nb_cols; i++) 
    {
        mpq_init (upper[i]); // fill in the upper bounds
        mpq_set (upper[i], mpq_ILL_MAXDOUBLE);
    }
    rownames=(char**)malloc(nb_rows*sizeof(char*));
    for(j=0;j<nb_rows;j++)
    {
        rownames[j]=(char*)malloc(2*sizeof(char));
        rownames[j][0]=j;
        rownames[j][1]='\0';
    }
    colnames=(char**)malloc(nb_cols*sizeof(char*));
    for(j=0;j<nb_cols;j++)
    {
        colnames[j]=(char*)malloc(2*sizeof(char));
        colnames[j][0]=j;
        colnames[j][1]='\0';
        printf("%s",colnames[j]);
    }
    p = mpq_QSload_prob ("prob", nb_cols,nb_rows, cmatcnt, cmatbeg, cmatind, cmatval,
                    QS_MAX, obj, rhs, sense, lower, upper, (const char**)colnames,(const char**)rownames);
    if (p == (mpq_QSprob) NULL) {
        fprintf (stderr, "Unable to load the LP problem\n");
    }
    rval = QSexact_solver (p, NULL, NULL, NULL, DUAL_SIMPLEX, &status);
    rval = mpq_QSget_status (p, &status);
    switch (status) {
    case QS_LP_OPTIMAL:
        printf ("An optimal solution is available\n");
        break;
    case QS_LP_INFEASIBLE:
        printf ("The LP has no feasible solution\n");
        break;
    case QS_LP_UNBOUNDED:
        printf ("The LP has unbounded objective value\n");
        break;
    case QS_LP_UNSOLVED:
        printf ("The optimizer could not solve the LP\n");
        break;
    case QS_LP_MODIFIED:
        printf ("The LP was modified since last optimization call\n");
        break;
    default:
        printf ("Unknown solution status: %d\n", status);
        break;
    }
    
    rval = mpq_QSget_objval (p, &value);
    if (rval) {
        fprintf (stderr, "Could not get obj value, error code %d\n", rval);
    } else {
        printf ("Objective Value = ");
        mpq_out_str (stdout, 10, value);
        printf ("\n");
        printf ("Objective Value (rounded to double) = %.6f\n",mpq_get_d (value));
    }

    /*  CPXgetx  */

    x = (mpq_t *) malloc (nb_cols * sizeof (mpq_t));
    for (i = 0; i < nb_cols; i++) mpq_init (x[i]);
    rval = mpq_QSget_x_array (p, x);
    if (rval) {
        fprintf (stderr, "Could not get x-vector, error code %d\n", rval);
    } else {
         printf ("Primal Solution:\n");
        colnames = (char **) malloc (nb_cols * sizeof (char *));
        mpq_QSget_colnames (p, colnames);
        for (i = 0; i < nb_cols; i++) {
            printf ("%s = ", colnames[i]);
            mpq_out_str (stdout, 10, x[i]);
            printf ("\n");
        }
    }
    
        rval = 1;  goto CLEANUP;

CLEANUP:

    for (i = 0; i < nb_cmatval; i++) mpq_clear (cmatval[i]);
    for (i = 0; i < nb_obj; i++) mpq_clear (obj[i]);
    for (i = 0; i < nb_rhs; i++) mpq_clear (rhs[i]);
    for (i = 0; i < nb_cols; i++) mpq_clear (lower[i]);
    for (i = 0; i < nb_cols; i++) mpq_clear (upper[i]);
    return 1;
}
