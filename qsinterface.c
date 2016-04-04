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
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <QSopt_ex.h>
#include <gmp.h>
long getargs(char*, long**);
int load_test (mpq_QSprob *p);
int setqsobj(mpq_QSprob* , long* , long );
int setqslp(mpq_QSprob *p,long* , long );
int solveqslp(mpq_QSprob *,int);
int nextnum(char* ,int);
int getlpsol_primal(mpq_QSprob*);
int getlpsol_dual(mpq_QSprob*);


int main()
{
int i,j;
int newarg;
long nb_args;
int cmdcode,status,qs_algo;
long* args= (long*)NULL;
char* buf=(char*)NULL;
char* tempbuf;
size_t bufsize;
QSexactStart();
QSexact_set_precision (128);
mpq_QSprob p = (mpq_QSprob) NULL;
while(1)
{   
    if(getline(&buf,&bufsize,stdin)!=-1)
    {
        sscanf(buf,"%d",&cmdcode);
        //printf("read line %s, cmdcode=%d ",buf,cmdcode);
        if(cmdcode==1) // load LP
        {
            i=0;
            buf=buf+2;
            sscanf(buf,"%ld",&nb_args);
            //printf("nargs = %ld",nb_args);
            args=(long*)malloc(nb_args*sizeof(long));
            for(j=0;j<nb_args;j++)
            {
                i=nextnum(buf,i);
                sscanf(buf+i,"%ld",args+j);
                //printf("j= %d ,num = %ld||",j,args[j]);
            }
            //printf("loading problem");
            setqslp(&p,args,nb_args);
            free(args);
        }
        else if(cmdcode==2)
        {
            break;
        }
        else if(cmdcode==3) // get new objective and load it 
        {
            // set obj
            i=0;
            buf=buf+2;
            sscanf(buf,"%ld",&nb_args);
            //printf("nargs = %ld",nb_args);
            args=(long*)malloc(nb_args*sizeof(long));
            for(j=0;j<nb_args;j++)
            {
                i=nextnum(buf,i);
                sscanf(buf+i,"%ld",args+j);
            }
            setqsobj(&p,args, nb_args);
            free(args);
        }
        else if(cmdcode==4) // solve LP
        {
            buf=buf+2;
            sscanf(buf,"%d",&qs_algo);
            solveqslp(&p,qs_algo);
        }
        else if(cmdcode==5)// get primal solution
        {
            getlpsol_primal(&p);
        }
        else if(cmdcode==6)// get dual solution
        {
            getlpsol_dual(&p);
        }
    }
}
return 1;
}

int solveqslp(mpq_QSprob *p,int qs_algo)
{
    // solve the lp *p and return solution and arg-solution
    int status,rval,i;
    long nb_cols = mpq_QSget_colcount (*p);;
    rval = mpq_QSget_status (*p, &status);
    //printf("status before = %d ",status);
    rval = QSexact_solver (*p, NULL, NULL, NULL, qs_algo, &status);
    rval = mpq_QSget_status (*p, &status);
    // compose format string to print to stdout, to be picked up by GAP
    printf("status:=%d;;",status);
    return 1;
}

int getlpsol_primal(mpq_QSprob* p)
{
    int rval,i,status;
    mpq_t*x;
    mpq_t value;
    mpq_init(value);
    long nb_cols = mpq_QSget_colcount (*p);;
    rval = mpq_QSget_status (*p, &status);
    // compose format string to print to stdout, to be picked up by GAP
    switch (status) {
    case QS_LP_OPTIMAL:
        printf ("status:=%d;;",status);
        rval = mpq_QSget_objval (*p, &value);
        if (rval) 
        {
            printf("val_rval:= %d;;", rval);
            printf("val:= -1");
        } 
        else 
        {
            printf("val_rval:= %d;;", rval);
            gmp_printf("val:= %Qd;;", value);
        }
        x = (mpq_t *) malloc (nb_cols * sizeof (mpq_t));
        for (i = 0; i < nb_cols; i++) mpq_init (x[i]);
        rval = mpq_QSget_x_array (*p, x);
        if (rval) 
        {
            printf("x_rval:= %d;;", rval);
            printf("x:= [];;", rval);
        } 
        else 
        {
            printf("x_rval:= %d;;", rval);
            printf("x:=[");
            for (i = 0; i < nb_cols-1; i++) 
            {
                gmp_printf("%Qd,", x[i]);
            }
            gmp_printf("%Qd];;", x[nb_cols-1]);
        }   
        break;
    default:
        printf ("status:=%d;;", status);
        break;
    }
    return 1;
}

int getlpsol_dual(mpq_QSprob* p)
{
    int rval,i,status;
    mpq_t*y;
    mpq_t value;
    mpq_init(value);
    long nb_rows = mpq_QSget_rowcount (*p);;
    rval = mpq_QSget_status (*p, &status);
    // compose format string to print to stdout, to be picked up by GAP
    switch (status) {
    case QS_LP_OPTIMAL:
        printf ("status:=%d;;",status);
        rval = mpq_QSget_objval (*p, &value);
        if (rval) 
        {
            printf("val_rval:= %d;;", rval);
            printf("val:= -1");
        } 
        else 
        {
            printf("val_rval:= 1;;");
            gmp_printf("val:= %Qd;;", value);
        }
        y = (mpq_t *) malloc (nb_rows * sizeof (mpq_t));
        for (i = 0; i < nb_rows; i++) mpq_init (y[i]);
        rval = mpq_QSget_pi_array (*p, y);
        if (rval) 
        {
            printf("y_rval:= -1;;");
            printf("y:= [];;", rval);
        } 
        else 
        {
            printf("y_rval:= 1;;");
            printf("y:=[");
            for (i = 0; i < nb_rows-1; i++) 
            {
                gmp_printf("%Qd,", y[i]);
            }
            gmp_printf("%Qd];;", y[nb_rows-1]);
        }   
        break;
    default:
        printf ("status:=%d;;", status);
        break;
    }
    return 1;
}


int setqsobj(mpq_QSprob *p, long* args, long len)
{
    int j,i,rval;
    long nb_cols = mpq_QSget_colcount (*p);
    mpq_t* obj=(mpq_t*)malloc(nb_cols*sizeof(mpq_t));
    if(args[0]==1) // integer obj
    {
        for(j=1;j<nb_cols+1;j++) 
        {   
            mpq_init(obj[j-1]);
            mpq_set_si(obj[j-1],args[j],1);
        }
    }
    else 
    {
        long* objnum=(long*)malloc(nb_cols*sizeof(long));
        long* objden=(long*)malloc(nb_cols*sizeof(long));
        for(j=1;j<1+nb_cols;j++) 
           objnum[j-1]=args[j];
        for(j=nb_cols+1;j<2*nb_cols+1;j++) 
           objden[j-nb_cols-1]=args[j];
        for(j=0;j<nb_cols;j++) 
        {   
            mpq_init(obj[j]);
            mpq_set_si(obj[j],objnum[j],objden[j]);
        }
        free(objnum);
        free(objden);
    }
    rval=0;
    for(j=0;j<nb_cols;j++)
    rval |= mpq_QSchange_objcoef ( *p, j, obj[j]);
    free(obj);
    if(rval==0)
    {
        printf("rval_obj:=1;;");
        p[0]->qstatus=6;
        return 1;
    }
    else
    {
        printf("rval_obj:=-1;;");
        return -1;
    }
}
//long getargs(long** argspointer)
//{
    //long i;
    //long nb_args;
    //scanf("%ld",&nb_args);
    //*argspointer=(long*)malloc(nb_args*sizeof(long));
    //for(i=0;i<nb_args;i++)
    //scanf("%ld",*argspointer+i);
    //return nb_args; 
//}
//long getargs(char* buf, long** argspointer)
//{
    //long i,j;
    //long nb_args;
    //sscanf(buf,"%ld",&nb_args);
    //buf=nextnum(buf);
    //*argspointer=(long*)malloc(nb_args*sizeof(long));
    //for(i=0;i<nb_args;i++)
    //{
        //sscanf(buf,"%ld",(*argspointer)+i);
        ////fprintf(stdout,"i=%ld/args[i]=%ld/nargs=%ld\n",i,(*argspointer)[i],nb_args);
        //if(i!=nb_args-1)
        //{
            //buf=nextnum(buf);
        //}
    //}
    //return nb_args; 
//}

int nextnum(char* buf,int i)
{
    // caveat emptor: assumes infinite buffer. Use only when you are certain
    // there is indeed a number at or after buf[0]
    while(buf[i]!=' ' && buf[i]!='\0')
    i++;
    return i+1;
}


int setqslp(mpq_QSprob *p, long* args, long len)
{
    //printf("setqslp received %ld numbers",len);
    long i=0;// current index
    int qs_algo=0;
    long j,nb_cmatval,nb_cmatcnt,nb_cmatind,nb_sense,nb_obj,nb_rhs;
    mpq_t *obj,*cmatval,*rhs, *lower,*upper,*x; 
    int *cmatcnt, *cmatbeg, *cmatind;
    char* sense,**rownames,**colnames;
    int nb_rows,nb_cols,rval,status;
    long temp;
    mpq_t value;
    mpq_init(value);
    QSexactStart();
    QSexact_set_precision (128);
    if(args[i]==1) // integers only
    {
        i++;
        nb_rows=args[i];
        i++;
        nb_cols=args[i];
        i++;
        nb_obj=args[i];
        i++;
        
        obj=(mpq_t*)malloc(nb_obj*sizeof(mpq_t));
        for(j=i;j<i+nb_obj;j++) 
        {   //fprintf(stderr,"here1 %ld",j);
            mpq_init(obj[j-i]);
            mpq_set_si(obj[j-i],args[j],1);
        }
        i=j;
        nb_cmatval=args[i];
        i++;
        cmatval=(mpq_t*)malloc(nb_cmatval*sizeof(mpq_t));
        for(j=i;j<i+nb_cmatval;j++) 
        {
            mpq_init(cmatval[j-i]);
               mpq_set_si(cmatval[j-i],args[j],1);
        }
        i=j;
        nb_cmatcnt=args[i];
        i++;
        cmatcnt=(int*)malloc(nb_cmatcnt*sizeof(int));
        for(j=i;j<i+nb_cmatcnt;j++) 
        {
            cmatcnt[j-i]=args[j];
        }
        i=j;
        nb_cmatind=args[i];
        i++;
        cmatind=(int*)malloc(nb_cmatind*sizeof(int));
        for(j=i;j<i+nb_cmatind;j++) 
        {
            cmatind[j-i]=args[j];
        }
        i=j;
        nb_sense=args[i];
        i++;
        sense=(char*)malloc(nb_sense*sizeof(char));
        for(j=i;j<i+nb_sense;j++) 
        {
            sense[j-i]=(args[j]==1)?'E':'L';
        }
        i=j;
        nb_rhs=args[i];
        i++;
        rhs=(mpq_t*)malloc(nb_rhs*sizeof(mpq_t));
        for(j=i;j<i+nb_rhs;j++) 
        {
            mpq_init(rhs[j-i]);
            mpq_set_si(rhs[j-i],args[j],1);
        }
        i=j;
        qs_algo=args[i];
        
        // print obj:
        //printf("objective:\n");
        //for (j=0;j<nb_obj;j++)
        //gmp_printf("%Qd ", obj[j]);
        //printf("\n");
        //// print cmatval:
        //printf("cmatval:\n");
        //for (j=0;j<nb_cmatval;j++)
        //gmp_printf("%Qd  ", cmatval[j]);
        //printf("\n");
         //// print cmatcnt:
        //printf("cmatcnt:\n");
        //for (j=0;j<nb_cmatcnt;j++)
        //gmp_printf("%d  ", cmatcnt[j]);
        //printf("\n");
         //// print cmatind:
        //printf("cmatind:\n");
        //for (j=0;j<nb_cmatind;j++)
        //gmp_printf("%d  ", cmatind[j]);
        //printf("\n");
         //// print sense:
        //printf("Sense:\n");
        //for (j=0;j<nb_sense;j++)
        //printf("%c  ", sense[j]);
        //printf("\n");
        //// print rhs:
        //printf("rhs:\n");
        //for (j=0;j<nb_rhs;j++)
        //gmp_printf("%Qd ", rhs[j]);
        //printf("\n");
    }
    else // rationals
    {
        i++;
        nb_rows=args[i];
        i++;
        nb_cols=args[i];
        i++;
        nb_obj=args[i];
        i++;
        obj=(mpq_t*)malloc(nb_obj*sizeof(mpq_t));
        long* objnum=(long*)malloc(nb_obj*sizeof(long));
        long* objden=(long*)malloc(nb_obj*sizeof(long));
        for(j=i;j<i+nb_obj;j++) 
           objnum[j-i]=args[j];
        i=j;
        for(j=i;j<i+nb_obj;j++) 
           objden[j-i]=args[j];
        i=j;
        for(j=0;j<nb_obj;j++) 
        {   
            mpq_init(obj[j]);
            mpq_set_si(obj[j],objnum[j],objden[j]);
        }
        free(objnum);
        free(objden);
        nb_cmatval=args[i];
        i++;
        cmatval=(mpq_t*)malloc(nb_cmatval*sizeof(mpq_t));
        long* valnum=(long*)malloc(nb_cmatval*sizeof(long));
        long* valden=(long*)malloc(nb_cmatval*sizeof(long));
        for(j=i;j<i+nb_cmatval;j++) 
           valnum[j-i]=args[j];
        i=j;
        for(j=i;j<i+nb_cmatval;j++) 
           valden[j-i]=args[j];
        i=j;
        for(j=0;j<nb_cmatval;j++) 
        {
            mpq_init(cmatval[j]);
            //printf("%ld/%ld\n",valnum[j],valden[j]);
            mpq_set_si(cmatval[j],valnum[j],valden[j]);
        }
        free(valnum);
        free(valden);
        nb_cmatcnt=args[j];
        i++;
        cmatcnt=(int*)malloc(nb_cmatcnt*sizeof(int));
        for(j=i;j<i+nb_cmatcnt;j++) 
        {
            cmatcnt[j-i]=args[j];
        }
        i=j;
        nb_cmatind=args[i];
        i++;
        cmatind=(int*)malloc(nb_cmatind*sizeof(int));
        for(j=i;j<i+nb_cmatind;j++) 
        {
            cmatind[j-i]=args[j];
        }
        i=j;
        nb_sense=args[i];
        i++;
        sense=(char*)malloc(nb_sense*sizeof(char));
        for(j=i;j<i+nb_sense;j++) 
        {
            sense[j-i]=(args[j]==1)?'E':'L';
        }
        i=j;
        nb_rhs=args[i];
        i++;
        rhs=(mpq_t*)malloc(nb_rhs*sizeof(mpq_t));
        long* rhsnum=(long*)malloc(nb_rhs*sizeof(long));
        long* rhsden=(long*)malloc(nb_rhs*sizeof(long));
        for(j=i;j<i+nb_rhs;j++) 
           rhsnum[j-i]=args[j];
        i=j;
        for(j=i;j<i+nb_rhs;j++) 
           rhsden[j-i]=args[j];
        i=j;
        for(j=0;j<nb_rhs;j++) 
        {
            mpq_init(rhs[j]);
            mpq_set_si(rhs[j],rhsnum[j],rhsden[j]);
        }
        free(rhsnum);
        free(rhsden);
        qs_algo=args[i];
        //// print obj:
        //printf("objective:\n");
        //for (j=0;j<nb_obj;j++)
        //gmp_printf("%Qd ", obj[j]);
        //printf("\n");
        //// print cmatval:
        //printf("cmatval:\n");
        //for (j=0;j<nb_cmatval;j++)
        //gmp_printf("%Qd  ", cmatval[j]);
        //printf("\n");
         //// print cmatcnt:
        //printf("cmatcnt:\n");
        //for (j=0;j<nb_cmatcnt;j++)
        //gmp_printf("%d  ", cmatcnt[j]);
        //printf("\n");
         //// print cmatind:
        //printf("cmatind:\n");
        //for (j=0;j<nb_cmatind;j++)
        //gmp_printf("%d  ", cmatind[j]);
        //printf("\n");
         //// print sense:
        //printf("Sense:\n");
        //for (j=0;j<nb_sense;j++)
        //printf("%c  ", sense[j]);
        //printf("\n");
        //// print rhs:
        //printf("rhs:\n");
        //for (j=0;j<nb_rhs;j++)
        //gmp_printf("%Qd ", rhs[j]);
        //printf("\n");
    }
    //// construct cmatbeg from cmatcnt
    cmatbeg=malloc(nb_cmatcnt*sizeof(int));
    cmatbeg[0]=0;
    for(j=1;j<nb_cmatcnt;j++)
    cmatbeg[j]=cmatbeg[j-1]+cmatcnt[j-1];
    //printf("cmatbeg:\n");
    //for (j=0;j<nb_cmatcnt;j++)
    //printf("%d ", cmatbeg[j]);
    //printf("\n");
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
        rownames[j]=(char*)malloc(30*sizeof(char));
        //rownames[j][0]=j;
        //rownames[j][1]='\0';
        sprintf(rownames[j],"r%ld",j);
        //printf("%s",rownames[j]);
    }
    colnames=(char**)malloc(nb_cols*sizeof(char*));
    for(j=0;j<nb_cols;j++)
    {
        colnames[j]=(char*)malloc(30*sizeof(char));
        //colnames[j][0]=j;
        //colnames[j][1]='\0';
        sprintf(colnames[j],"c%ld",j);
        //printf("%s",colnames[j]);
    }
    *p=mpq_QSload_prob ("prob", nb_cols,nb_rows, cmatcnt, cmatbeg, cmatind, cmatval,
                    QS_MAX, obj, rhs, sense, lower, upper, (const char**)colnames,(const char**)rownames);
    if (*p == (mpq_QSprob) NULL) {
        printf("status:=[-1];;");
    }
    else
    printf("status:=[1];;");
    return 1;
}

int load_test (mpq_QSprob *p)
{
    int i, rval = 0;
    int cmatcnt[3] = { 3, 2, 3 };
    int cmatbeg[3] = { 0, 3, 5 };
    int cmatind[8] = { 0, 1, 2, 0, 1, 0, 3, 4 };
    char sense[5] = { 'L', 'E', 'L','L','L' };
    const char *colnames[3] = { "x", "y", "z" };
    const char *rownames[5] = { "c1", "c2","c3","c4","c5"};
    mpq_t cmatval[8];
    mpq_t obj[3];
    mpq_t rhs[5];
    mpq_t lower[3];
    mpq_t upper[3];

    for (i = 0; i < 8; i++) mpq_init (cmatval[i]);
    mpq_set_d (cmatval[0], 3.0);
    mpq_set_d (cmatval[1], 5.0);
    mpq_set_d (cmatval[2], -1.0);
    mpq_set_d (cmatval[3], 2.0);
    mpq_set_d (cmatval[4], 1.0);
    mpq_set_d (cmatval[5], 1.0);
    mpq_set_d (cmatval[6], 1.0);
    mpq_set_d (cmatval[7], -1.0);

    for (i = 0; i < 3; i++) mpq_init (obj[i]);
    mpq_set_d (obj[0], 3.0);
    mpq_set_d (obj[1], 2.0);
    mpq_set_d (obj[2], 4.0);

    for (i = 0; i < 5; i++) mpq_init (rhs[i]);
    mpq_set_d (rhs[0], 12.0);
    mpq_set_d (rhs[1], 10.0);
    mpq_set_d (rhs[1], -2.0);
    mpq_set_d (rhs[1], 10);
    mpq_set_d (rhs[1], -1);
   
    for (i = 0; i < 3; i++) mpq_init (lower[i]);
    mpq_set (lower[0], mpq_ILL_MINDOUBLE);
    mpq_set (lower[1], mpq_ILL_MINDOUBLE);
    mpq_set (lower[2], mpq_ILL_MINDOUBLE);

    for (i = 0; i < 3; i++) mpq_init (upper[i]);
    mpq_set (upper[0], mpq_ILL_MAXDOUBLE);
    mpq_set (upper[1], mpq_ILL_MAXDOUBLE);
    mpq_set (upper[2], mpq_ILL_MAXDOUBLE);

    /*  CPXcopylpwnames  */

    *p = mpq_QSload_prob ("small", 3, 5, cmatcnt, cmatbeg, cmatind, cmatval,
                      QS_MAX, obj, rhs, sense, lower, upper, colnames,
                      rownames);

    if (*p == (mpq_QSprob) NULL) {
        fprintf (stderr, "Unable to load the LP problem\n");
        rval = 1;  goto CLEANUP;
    }

CLEANUP:

    for (i = 0; i < 8; i++) mpq_clear (cmatval[i]);
    for (i = 0; i < 3; i++) mpq_clear (obj[i]);
    for (i = 0; i < 5; i++) mpq_clear (rhs[i]);
    for (i = 0; i < 3; i++) mpq_clear (lower[i]);
    for (i = 0; i < 3; i++) mpq_clear (upper[i]);

    return rval;
}
