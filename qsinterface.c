/*******************************************************************************/
/* Command codes:
The GAP  part of the interface communicates with the C part via stdout, and in
particular, via command codes. These are integers that tell the C part of the
code to take some action eg. read the following numbers and load them into the
objective or read the following number to change a particular rhs oefficient.
The numbers following the command code follow a certain pattern, irrespective of
what the command is. The general format is as follows:
cmdcode <nargs> <rat/int (opt)> <numerators> <denominators>
Here, <nargs> is an argument used to specify the number of numbers
following cmdcode, if it is variable-length (not used otherwise). <rat/int> is a
0/1 flag that indicates if the numbers are rational/integers. If numbers are rational
then they are sent as a list of numerators followed by a list of denominators.
The command codes and their meanings are as follows:
1 - load a LP. FORMAT: cmdcode nargs <rat/int (opt)> <numerators> <denominators>
2 - exit. FORMAT: cmdcode
3 - get new objective and load it.
    FORMAT: cmdcode nargs <rat/int (opt)> <numerators> <denominators>
4 - solve LP
5 - get primal solution. FORMAT: cmdcode
6 - get dual solution. FORMAT: cmdcode
7 - delete a row from constraint matrix. FORMAT: cmdcode rowindex(base 0)
8 - modify a specified rhs coef.
    FORMAT: cmdcode nargs rowindex numcoef dencoef(optional, for rationals)
9 - modify rhs sense (<= or =). FORMAT: cmdcode nargs rowindex sense
10 - change a particular entry in constraint matrix.
    FORMAT: cmdcode nargs rowindex colindex numcoef dencoef(optional, for rationals)
11 - print currently loaded LP to stedrr. FORMAT: cmdcode
12 - display status on stderr. FORMAT: cmdcode
/**/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <QSopt_ex.h>
#include <gmp.h>
#define BUFFER_SIZE 20
long getargs(char*, long**);
int load_test (mpq_QSprob *p);
int setqsobj(mpq_QSprob* , long* , long );
int setqslp(mpq_QSprob *p,long* , long );
int solveqslp(mpq_QSprob *,int);
int nextnum(char* ,int);
int getlpsol_primal(mpq_QSprob*);
int getlpsol_dual(mpq_QSprob*);
int delrow(mpq_QSprob* , long);
int changerhs(mpq_QSprob* , long , mpq_t );
int changesense(mpq_QSprob* , long , int );
int changecoef(mpq_QSprob* ,long ,long ,mpq_t );

int main()
{
int i,j;
int newarg;
long nb_args,row;
int cmdcode,status,qs_algo;
long* args= (long*)NULL;
char* buf=(char*)NULL;
char* buf_original=(char*)NULL; // to free getline's buffer
char* tempbuf;
size_t bufsize;
QSexactStart();
QSexact_set_precision (128);
mpq_QSprob p = (mpq_QSprob) NULL;
while(1)
{
    if(getline(&buf_original,&bufsize,stdin)!=-1)
    {
        buf=buf_original;
        //fprintf(stderr,"buffer addresses = %p",buf_original);
        sscanf(buf,"%d",&cmdcode);
        //fprintf(stderr,"read line %s, cmdcode=%d ",buf,cmdcode);
        if(cmdcode==1) // load LP
        {
            i=0;
            buf=buf+2;
            sscanf(buf,"%ld",&nb_args);
            //fprintf(stderr,"nargs = %ld",nb_args);
            args=(long*)malloc(nb_args*sizeof(long));
            for(j=0;j<nb_args;j++)
            {
                i=nextnum(buf,i);
                sscanf(buf+i,"%ld",args+j);
                //fprintf(stderr,"j= %d ,num = %ld||",j,args[j]);
            }
            //printf("loading problem");
            setqslp(&p,args,nb_args);
            free(args);
            fflush(stdout);
        }
        else if(cmdcode==2)
        {
            break;
        }
        else if(cmdcode==3) // get new objective and load it
        {
            // set obj
            //fprintf(stderr,"Loading obj, here 0\n");
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
	    fflush(stdout);
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
        else if(cmdcode==7)
        {
            buf=buf+2;
            sscanf(buf,"%ld",&row);
            delrow(&p, row);
        }
        else if(cmdcode==8)
        {
            // get new rhs coef and row index
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
            mpq_t coef;
            mpq_init(coef);
            if(nb_args==2)// integer
            {
                mpq_set_si(coef,args[1],1);
            }
            else
            {
                mpq_set_si(coef,args[1],args[2]);
            }
            changerhs(&p,args[0],coef);
            free(args);
        }
        else if(cmdcode==10)
        {
                // get new coef and row,column indices
            i=0;
            buf=buf+3;
            sscanf(buf,"%ld",&nb_args);
            //printf("nargs = %ld",nb_args);
            args=(long*)malloc(nb_args*sizeof(long));
            for(j=0;j<nb_args;j++)
            {
                i=nextnum(buf,i);
                sscanf(buf+i,"%ld",args+j);
            }
            mpq_t coef;
            mpq_init(coef);
            if(nb_args==3)// integer
            {
                mpq_set_si(coef,args[2],1);
            }
            else
            {
                mpq_set_si(coef,args[2],args[3]);
            }
            //gmp_fprintf(stderr,"Recieved coef %Qd, row=%ld,col=%ld\n",coef,args[0],args[1]);
            changecoef(&p,args[0],args[1],coef);
            free(args);
        }
        else if(cmdcode==9)
        {
            // change rhs sense of a row
            i=0;
            buf=buf+2;
            sscanf(buf,"%ld",&nb_args);

            args=(long*)malloc(nb_args*sizeof(long));
            for(j=0;j<nb_args;j++)
            {
                i=nextnum(buf,i);
                sscanf(buf+i,"%ld",args+j);
            }
            if(args[1]==76)
            changesense(&p,args[0],'L');
            else
            changesense(&p,args[0],'E');
            free(args);
        }
        else if(cmdcode==11) // print LP
        {
            if(p)
            mpq_QSwrite_prob_file(p,stderr,"LP");
        }
        else if(cmdcode==12) // display status on stderr
        {
            int status_current=0;
            mpq_QSget_status (p, &status_current);
            fprintf(stderr,"Current status = %d",status_current);
        }
        //fprintf(stderr,"original buffer address = %p, mod = %p",buf_original,buf);
    }
    if(buf_original!=(char*)NULL)
    free(buf_original);
    buf_original=(char*)NULL;
}
free(buf);
free(buf_original);
return 1;
}

int solveqslp(mpq_QSprob *p,int qs_algo)
{
    // solve the lp *p and return solution and arg-solution
    int status,rval,i;
    long nb_cols = mpq_QSget_colcount (*p);;
    rval = mpq_QSget_status (*p, &status);
    //fprintf(stderr,"status before = %d ",status);
    rval = QSexact_solver (*p, NULL, NULL, NULL, qs_algo, &status);
    rval = mpq_QSget_status (*p, &status);
    // compose format string to print to stdout, to be picked up by GAP
    //fprintf(stderr,"status after = %d ",status);
    printf("status:=%d;;\n",status);
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
            printf("x:= [];;\n");
        }
        else
        {
            printf("x_rval:= %d;;", rval);
            printf("x:=[");
            for (i = 0; i < nb_cols-1; i++)
            {
                gmp_printf("%Qd,", x[i]);
            }
            gmp_printf("%Qd];;\n", x[nb_cols-1]);
        }
        free(x);
        break;
    default:
        printf ("status:=%d;;\n", status);
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
            printf("val_rval:= %d;;",rval);
            gmp_printf("val:= %Qd;;", value);
        }
        y = (mpq_t *) malloc (nb_rows * sizeof (mpq_t));
        for (i = 0; i < nb_rows; i++) mpq_init (y[i]);
        rval = mpq_QSget_pi_array (*p, y);
        if (rval)
        {
            printf("y_rval:= -1;;");
            printf("y:= [];;");
        }
        else
        {
            printf("y_rval:= %d;;",rval);
            printf("y:=[");
            for (i = 0; i < nb_rows-1; i++)
            {
                gmp_printf("%Qd,", y[i]);
            }
            gmp_printf("%Qd];;\n", y[nb_rows-1]);
        }
        break;
    default:
        printf ("status:=%d;;\n", status);
        break;
    }
    return 1;
}


int setqsobj(mpq_QSprob *p, long* args, long len)
{
    int j,i,rval;
    long nb_cols = mpq_QSget_colcount (*p);
    //fprintf(stderr,"Loading obj, here 1\n");
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
        //fprintf(stderr,"Loading obj, here 2\n");
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
    //fprintf(stderr,"Loading obj, here 3\n");
    for(j=0;j<nb_cols;j++)
    {
        rval |= mpq_QSchange_objcoef ( *p, j, obj[j]);
    }
    free(obj);
    if(rval==0)
    {
        printf("rval_obj:=1;;\n");
        p[0]->qstatus=6;
        return 1;
    }
    else
    {
        printf("rval_obj:=-1;;\n");
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

int delrow(mpq_QSprob* p, long row)
{
    //row is base-0 index
    int rval;
    //mpq_QSwrite_prob_file(*p,stdout,"LP");
    rval=mpq_QSdelete_row(*p,row);
    //mpq_QSwrite_prob_file(*p,stdout,"LP");
    printf("delrow_rval:=%d;;\n",rval);
    p[0]->qstatus=6;
    return 1;
}

int changerhs(mpq_QSprob* p, long row, mpq_t coef)
{
    // row is base 0 index
    int rval;
    //mpq_QSwrite_prob_file(*p,stdout,"LP");
    rval=mpq_QSchange_rhscoef(*p,row,coef);
    printf("rhs_rval:=%d;;\n",rval);
    p[0]->qstatus=6;
    //mpq_QSwrite_prob_file(*p,stdout,"LP");
    return 1;
}

int changesense(mpq_QSprob* p, long row, int sense)
{
    // row is base 0 index
    int rval;
    //mpq_QSwrite_prob_file(*p,stderr,"LP");

    rval=mpq_QSchange_sense(*p,row,sense);
    printf("sense_rval:=%d;;\n",rval);
    p[0]->qstatus=6;
    //mpq_QSwrite_prob_file(*p,stderr,"LP");
    return 1;
}

int changecoef(mpq_QSprob* p,long row,long col,mpq_t coef)
{
     // row is base 0 index
    int rval;
    //mpq_QSwrite_prob_file(*p,stderr,"LP");
    rval=mpq_QSchange_coef(*p,row,col,coef);
    printf("coef_rval:=%d;;\n",rval);
    p[0]->qstatus=6;
    //mpq_QSwrite_prob_file(*p,stderr,"LP");
    return 1;
}

int nextnum(char* buf,int i)
{
    // caveat emptor: assumes infinite buffer. Use only when you are certain
    // there is indeed a number at or after buf[0]
    while(buf[i]!=' ' && buf[i]!='\0')
    i++;
    return i+1;
}

int getnextcnt(long* arr, int num_obj, int* offset){
    *offset +=num_obj;
    long ret = arr[*offset];
    *offset+=1;
    return ret;
}
int rationalgetnextcnt(long* arr, int num_obj, int* offset){
    *offset +=num_obj*2;
    long ret = arr[*offset];
    *offset+=1;
    return ret;
}

int* getsliceofarr(long* arr, int start, int nb_obj){
    int* obj=(int*)malloc(nb_obj*sizeof(int));
    int j;
    for(j=start;j<nb_obj+start;j++)
    {
        obj[j-start]=arr[j];
    }
    return obj;
}

mpq_t* inttompq(long* arr, int start, int nb_obj){
    mpq_t* obj=(mpq_t*)malloc(nb_obj*sizeof(mpq_t));
    for(int j=start;j<nb_obj+start;j++)
    {   
        mpq_init(obj[j-start]);
        mpq_set_si(obj[j-start],arr[j],1);
    }
    return obj;
}
mpq_t* rationaltompq(long* arr, int start, int nb_obj){
    long* objnum=(long*)malloc(nb_obj*sizeof(long));
    long* objden=(long*)malloc(nb_obj*sizeof(long));
    mpq_t* obj=(mpq_t*)malloc((nb_obj)*sizeof(mpq_t));
    int j;
    for(j=start;j<start+nb_obj;j++){
       objnum[j-start]=arr[j];
    }
    start=j;
    for(j=start;j<start+nb_obj;j++)
       objden[j-start]=arr[j];
    for(j=0;j<nb_obj;j++)
    {
        mpq_init(obj[j]);
        mpq_set_si(obj[j],objnum[j],objden[j]);
    }
    free(objnum);
    free(objden);
    return obj;
}

mpq_t* initializempqarr(mpq_t init_value, int size){
    mpq_t* objs=(mpq_t*)malloc(size*sizeof(mpq_t));
    for (int i = 0; i < size; i++)
    {
        mpq_init (objs[i]); // fill in the lower bounds
        mpq_set (objs[i], init_value);
    }
    return objs;
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
    int offset = 4;
    if(args[0]==1) // integers only
    {

        nb_rows=args[1];
        nb_cols=args[2];
        nb_obj=args[3];
        obj = inttompq(args, offset, nb_obj);
        nb_cmatval= getnextcnt(args, nb_obj, &offset);
        cmatval = inttompq(args, offset, nb_cmatval);
        nb_cmatcnt= getnextcnt(args, nb_cmatval, &offset);
        cmatcnt = getsliceofarr(args, offset, nb_cmatcnt);
        nb_cmatind= getnextcnt(args, nb_cmatcnt, &offset);
        cmatind = getsliceofarr(args, offset, nb_cmatind);
	// julius error was offset++ below... wtf
        nb_sense= getnextcnt(args, nb_cmatind, &offset);
        sense=(char*)malloc(nb_sense*sizeof(char));
	//fprintf(stderr,"%d",nb_sense);
        for(int j=offset;j<offset+nb_sense;j++)
        {
            sense[j-offset]=(args[j]==1)?'E':'L';
	    //fprintf(stderr,"%c\n",sense[j-offset]);
        }
        nb_rhs= getnextcnt(args, nb_sense, &offset);
        rhs = inttompq(args, offset, nb_rhs);
        qs_algo=getnextcnt(args, nb_rhs, &offset);
    }
    else // rationals
    {
        nb_rows=args[1];
        nb_cols=args[2];
        nb_obj=args[3];
        obj = rationaltompq(args, offset, nb_obj);
        nb_cmatval= rationalgetnextcnt(args, nb_obj, &offset);
        cmatval = rationaltompq(args, offset, nb_cmatval);
        nb_cmatcnt= rationalgetnextcnt(args, nb_cmatval, &offset);
        cmatcnt = getsliceofarr(args, offset, nb_cmatcnt);
        nb_cmatind= getnextcnt(args, nb_cmatcnt, &offset);
        cmatind = getsliceofarr(args, offset, nb_cmatind);
        // julius -- this should not have been here, eh?  offset++;
        nb_sense= getnextcnt(args, nb_cmatind, &offset);
        sense=(char*)malloc(nb_sense*sizeof(char));
        for(int j=offset;j<offset+nb_sense;j++)
        {
            sense[j-offset]=(args[j]==1)?'E':'L';
        }
        nb_rhs= getnextcnt(args, nb_sense, &offset);
        rhs = rationaltompq(args, offset, nb_rhs);
        qs_algo=rationalgetnextcnt(args, nb_rhs, &offset);
    }
    //// construct cmatbeg from cmatcnt
    cmatbeg=malloc(nb_cmatcnt*sizeof(int));
    cmatbeg[0]=0;
    for(j=1;j<nb_cmatcnt;j++)
    cmatbeg[j]=cmatbeg[j-1]+cmatcnt[j-1];

    lower=initializempqarr(mpq_ILL_MINDOUBLE, nb_cols);// lower bounds
    upper=initializempqarr(mpq_ILL_MAXDOUBLE, nb_cols);// upper bounds

    rownames=(char**)malloc(nb_rows*sizeof(char*));
    for(j=0;j<nb_rows;j++)
    {
        rownames[j]=(char*)malloc(BUFFER_SIZE*sizeof(char));
        snprintf(rownames[j], (size_t)BUFFER_SIZE,"r%ld",j);
    }
    colnames=(char**)malloc(nb_cols*sizeof(char*));
    for(j=0;j<nb_cols;j++)
    {
        colnames[j]=(char*)malloc(BUFFER_SIZE*sizeof(char));
        snprintf(colnames[j],(size_t)BUFFER_SIZE, "c%ld",j);
    }
    *p=mpq_QSload_prob ("prob", nb_cols,nb_rows, cmatcnt, cmatbeg, cmatind, cmatval,
                    QS_MAX, obj, rhs, sense, lower, upper, (const char**)colnames,(const char**)rownames);
    if (*p == (mpq_QSprob) NULL) {
        printf("status:=[-1];;\n");
    }
    else
    printf("status:=[1];;\n");

    //free everything
    for(j=0;j<nb_rows;j++)
    {
        free(rownames[j]);
    }
    for(j=0;j<nb_cols;j++)
    {
        free(colnames[j]);
    }
    free(cmatcnt);
    free(cmatbeg);
    free(cmatind);
    free(cmatval);
    free(obj);
    free(rhs);
    free(sense);
    free(lower);
    free(upper);

    free(colnames);
    free(rownames);

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
