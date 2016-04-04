################################################################################
##
##                                                 qsopt_ex-interface package
##
##  Copyright 2016,           Jayant Apte, Drexel University
##
##  The .gi file containing implementation part of the qsopt_ex-interface package.
##
################################################################################

##

InstallGlobalFunction(qsoptformatstr,
function(obj,A,b,linrows,optargs)
local At,cmatval,cmatcnt,cmatind,sense,i,j,c,r,ccnt,x,ostr,hasrat,qs_algo;
# find out if A or b have rationals
hasrat:=false;
for i in [1..Size(obj)] do
  if not DenominatorRat(obj[i])=1 then
    hasrat:=true;
    break;
  fi;
od;
for i in [1..Size(A)] do
  for j in [1..Size(A[i])] do
    if not DenominatorRat(A[i][j] )=1 then
      hasrat:=true;
      break;
    fi;
  od;
od;
if hasrat=false then
  for i in [1..Size(b)] do
    if not DenominatorRat(b[i])=1 then
      hasrat:=true;
      break;
    fi;
  od;
fi;
At:=TransposedMat(A);
cmatval:=[];
cmatcnt:=[];
cmatind:=[];
sense:=[];
for c in [1..Size(At)] do
  ccnt:=0;
  for r in [1..Size(At[c])] do
    if not At[c][r]=0 then
      ccnt:=ccnt+1;
      Append(cmatval,[At[c][r]]);
      Append(cmatind,[r-1]);
    fi;
  od;
  Append(cmatcnt,[ccnt]);
od;
for i in [1..Size(A)] do
  if i in linrows then
    Append(sense,[1]);
  else
    Append(sense,[0]);
  fi;
od;
if hasrat=false then
 Display("norat");
 ostr:=[];
 Append(ostr,[String(1)]);
 Append(ostr,[String(Size(A))]);
 Append(ostr,[String(Size(obj))]);
 Append(ostr,[String(Size(obj))]);;
 for x in obj do
  Append(ostr,[String(x)]);
 od;
 Append(ostr,[String(Size(cmatval))]);
 for x in cmatval do
  Append(ostr,[String(x)]);
 od;
 Append(ostr,[String(Size(cmatcnt))]);
 for x in cmatcnt do
  Append(ostr,[String(x)]);;
 od;
 Append(ostr,[String(Size(cmatind))]);;
 for x in cmatind do
  Append(ostr,[String(x)]);;
 od;
 Append(ostr,[String(Size(sense))]);;
 for x in sense do
  Append(ostr,[String(x)]);
 od;
 Append(ostr,[String(Size(b))]);
 for x in b do
  Append(ostr,[String(x)]);
 od;
else
 Display("yesrat");
  ostr:="";
  Append(ostr,[String(0)]);
  Append(ostr,[String(Size(A))]);
  Append(ostr,[String(Size(obj))]);
  Append(ostr,[String(Size(obj))]);
  for x in obj do
   Append(ostr,[String(NumeratorRat(x))]);
  od;
  for x in obj do
   Append(ostr,[String(DenominatorRat(x))]);
  od;
  ##
  Append(ostr,[String(Size(cmatval))]);
  for x in cmatval do
    Append(ostr,[String( NumeratorRat(x))]);
  od;
  for x in cmatval do
    Append(ostr,[String( DenominatorRat(x))]);
  od;
  Append(ostr,[String(Size(cmatcnt))]);
  for x in cmatcnt do
   Append(ostr,[String(x)]);
  od;
  Append(ostr,[String(Size(cmatind))]);
  for x in cmatind do
   Append(ostr,[String(x)]);
  od;
  Append(ostr,[String(Size(sense))]);
  for x in sense do
   Append(ostr,[String(x)]);
  od;
  Append(ostr,[String(Size(b))]);
  for x in b do
   Append(ostr,[String(NumeratorRat(x))]);
  od;
  for x in b do
   Append(ostr,[String(DenominatorRat(x))]);
  od;
fi;
if not Size(optargs) > 0 then
  qs_algo:=0; # primal simplex
else
  qs_algo:=optargs[1];
fi;
Append(ostr,[String(qs_algo)]);
return ostr;
end);

InstallGlobalFunction(qsoptformatstr2,
function(obj,A,b,linrows,optargs)
local At,cmatval,cmatcnt,cmatind,sense,i,j,c,r,ccnt,x,ostr,hasrat,qs_algo,ostr2;
# find out if A or b have rationals
hasrat:=false;
for i in [1..Size(obj)] do
  if not DenominatorRat(obj[i])=1 then
    hasrat:=true;
    break;
  fi;
od;
for i in [1..Size(A)] do
  for j in [1..Size(A[i])] do
    if not DenominatorRat(A[i][j] )=1 then
      hasrat:=true;
      break;
    fi;
  od;
od;
if hasrat=false then
  for i in [1..Size(b)] do
    if not DenominatorRat(b[i])=1 then
      hasrat:=true;
      break;
    fi;
  od;
fi;
At:=TransposedMat(A);
cmatval:=[];
cmatcnt:=[];
cmatind:=[];
sense:=[];
for c in [1..Size(At)] do
  ccnt:=0;
  for r in [1..Size(At[c])] do
    if not At[c][r]=0 then
      ccnt:=ccnt+1;
      Append(cmatval,[At[c][r]]);
      Append(cmatind,[r-1]);
    fi;
  od;
  Append(cmatcnt,[ccnt]);
od;
for i in [1..Size(A)] do
  if i in linrows then
    Append(sense,[1]);
  else
    Append(sense,[0]);
  fi;
od;
if hasrat=false then
 Display("norat");
 ostr:=[];
 Append(ostr,[String(1)]);
 Append(ostr,[String(Size(A))]);
 Append(ostr,[String(Size(obj))]);
 Append(ostr,[String(Size(obj))]);;
 for x in obj do
  Append(ostr,[String(x)]);
 od;
 Append(ostr,[String(Size(cmatval))]);
 for x in cmatval do
  Append(ostr,[String(x)]);
 od;
 Append(ostr,[String(Size(cmatcnt))]);
 for x in cmatcnt do
  Append(ostr,[String(x)]);;
 od;
 Append(ostr,[String(Size(cmatind))]);;
 for x in cmatind do
  Append(ostr,[String(x)]);;
 od;
 Append(ostr,[String(Size(sense))]);;
 for x in sense do
  Append(ostr,[String(x)]);
 od;
 Append(ostr,[String(Size(b))]);
 for x in b do
  Append(ostr,[String(x)]);
 od;
else
 Display("yesrat");
  ostr:="";
  Append(ostr,[String(0)]);
  Append(ostr,[String(Size(A))]);
  Append(ostr,[String(Size(obj))]);
  Append(ostr,[String(Size(obj))]);
  for x in obj do
   Append(ostr,[String(NumeratorRat(x))]);
  od;
  for x in obj do
   Append(ostr,[String(DenominatorRat(x))]);
  od;
  ##
  Append(ostr,[String(Size(cmatval))]);
  for x in cmatval do
    Append(ostr,[String( NumeratorRat(x))]);
  od;
  for x in cmatval do
    Append(ostr,[String( DenominatorRat(x))]);
  od;
  Append(ostr,[String(Size(cmatcnt))]);
  for x in cmatcnt do
   Append(ostr,[String(x)]);
  od;
  Append(ostr,[String(Size(cmatind))]);
  for x in cmatind do
   Append(ostr,[String(x)]);
  od;
  Append(ostr,[String(Size(sense))]);
  for x in sense do
   Append(ostr,[String(x)]);
  od;
  Append(ostr,[String(Size(b))]);
  for x in b do
   Append(ostr,[String(NumeratorRat(x))]);
  od;
  for x in b do
   Append(ostr,[String(DenominatorRat(x))]);
  od;
fi;
if not Size(optargs) > 0 then
  qs_algo:=0; # primal simplex
else
  qs_algo:=optargs[1];
fi;
Append(ostr,[String(qs_algo)]);
ostr2:="";
for x in ostr do
  ostr2:=Concatenation(ostr2,x," ");
od;
return ostr2;
end);


InstallGlobalFunction(LoadQSLPobj,
function(s,obj)
# setup a QSopt LP
# return the stream
# if obj is empty list, a dummy all-coefficients-1 objective is loaded
  local ostr,hasrat,ostr_list,x,i,istr;
  hasrat:=false;
  for i in [1..Size(obj)] do
    if not DenominatorRat(obj[i])=1 then
      hasrat:=true;
      break;
    fi;
  od;
  ostr:="";
  ostr_list:=[];
  if hasrat=true then
    ostr:=Concatenation(ostr,"0 ");
    Append(ostr_list,[0]);
    for x in obj do
      ostr:=Concatenation(ostr,String(NumeratorRat(x))," ");
      Append(ostr_list,[String(NumeratorRat(x))]);
    od;
    for x in obj do
      ostr:=Concatenation(ostr,String(DenominatorRat(x))," ");
      Append(ostr_list,[String(DenominatorRat(x))]);
    od;
  else
    ostr:=Concatenation(ostr,"1 ");
    Append(ostr_list,[1]);
    for x in obj do
      Append(ostr_list,[String(x)]);
      ostr:=Concatenation(ostr,String(x)," ");
    od;
  fi;
  ostr:=Concatenation("3 ",String(Size(ostr_list))," ",ostr);
  WriteLine(s,ostr);
  istr:=Concatenation("local rval_obj;", ReadLine(s), "return rval_obj;");
  istr:= InputTextString( istr);;
  return ReadAsFunction(istr)();
end);

InstallGlobalFunction(GetQSLPsol_primal,
function(s)
# solve the already loaded Qsopt LP on stream s
# use specified algorithm if mentioned in optargs[1]
local ostr,i;
WriteLine(s,"5 ");
ostr:=ReadLine(s);;
Display(ostr);
ostr:=Concatenation("local status,val,val_rval,x,x_rval;",ostr,"if status=1 then return [status,val_rval,val,x_rval,x];else return [status]; fi;");
i := InputTextString( ostr);;
return ReadAsFunction(i)();
end);

InstallGlobalFunction(GetQSLPsol_dual,
function(s)
# solve the already loaded Qsopt LP on stream s
# use specified algorithm if mentioned in optargs[1]
local ostr,i;
WriteLine(s,"6 ");
ostr:=ReadLine(s);;
ostr:=Concatenation("local status,val,val_rval,y,y_rval;",ostr,"if status=1 then return [status,val_rval,val,y_rval,y]; else return [status]; fi;");
i := InputTextString( ostr);;
return ReadAsFunction(i)();
end);

InstallGlobalFunction(SolveQSLP,
function(s,optargs)
# solve the already loaded Qsopt LP on stream s
# use specified algorithm if mentioned in optargs[1]
local qs_algo,i,ostr;
if Size(optargs)>1 then
qs_algo:=optargs[1];
else
qs_algo:=0;
fi;
WriteLine(s,Concatenation("4 ",String(qs_algo)));
ostr:=ReadLine(s);
Display(ostr);
ostr:=Concatenation("local status;",ostr,"return status;");
i := InputTextString( ostr);;
return ReadAsFunction(i)();
end);

InstallGlobalFunction(LoadQSLP,
function(obj,A,b,linrows,qs_exec)
# setup a QSopt LP
# return the stream
# if obj is empty list, a dummy all-coefficients-1 objective is loaded
    local  ostr, onemap, nb_ostr, s,istr,i;
    if Size( obj ) = 0  then
        onemap := function ( x )
              return 1;
          end;
        obj := List( [ 1 .. Size( A[1] ) ], onemap );
    fi;
    ostr := qsoptformatstr2( obj, A, b, linrows, [] );;
    nb_ostr := Size( qsoptformatstr( obj, A, b, linrows, [] ) );;
    ostr := Concatenation( "1 ", String( nb_ostr ), " ", ostr );;
    s := InputOutputLocalProcess( DirectoryCurrent(  ), qs_exec, [  ] );
    WriteLine( s, ostr );
    istr:=ReadLine(s);
    istr:=Concatenation("local status;",istr,"return status;");
    i := InputTextString( istr);;
    return [s,ReadAsFunction(i)()];
end);

InstallGlobalFunction(FlushQSLP,
function(s)
# flush the currently loaded qsopt LP and close stream
WriteLine(s,"2 ");
CloseStream(s);
end);
