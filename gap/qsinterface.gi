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
function(obj,A,b,linrows)
local At,cmatval,cmatcnt,cmatind,sense,i,j,c,r,ccnt,x,ostr,hasrat;
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
return ostr;
end);

InstallGlobalFunction(QSLP,
function(obj,A,b,linrows,qs_exec)
local str;
  str:=qsoptformatstr(obj,A,b,linrows);
  Process( DirectoryCurrent(),qs_exec, InputTextNone(), out,str);
end);
