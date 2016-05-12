#############################################################################
##
##                                                   qsopt_ex-interface package
##
##  Copyright 2016,                              Jayant Apte, Drexel University
##
##  The .gd file containing global function declarations and the documentation
##  of the itap package. The documentation is written using the AutoDoc Package.
##
##
#############################################################################
#! @Chapter Introduction
#! $\texttt{qsopt}\_\texttt{ex-interface}$ is a GAP package that provides an interface to $QSopt$ exact rational
#! linear program solver <Cite Key="qs"/> by Applegate,Cook,Dash and Espinoza. This is a minimalist package exposing parts of qsopt to GAP. The particular version of QSopt-exact
#! solver this package currently follows is 2.5.10-patch 3 of a fork of the original software maintained by Jon Lund Steffenson <Cite Key="qsjon"/>, which removes certain dependencies and
#! makes the software easier to build. $\texttt{qsopt}\_\texttt{ex-interface}$ provides a C wrapper qsinterface.c to the solver. It is currently available for Unix/Linux systems running GAP $4.5+$.
#! @Chapter Installation
#!   @BeginCode pkg
GAPInfo.("RootPaths");
#!   @EndCode
#! Assuming you already have GAP 4.5+ installed, you can follow the steps below to install the package:
#! * To get the newest version of $\texttt{qsopt}\_\texttt{ex-interface}$, download the .zip archive from <URL>https://github.com/jayant91089/qsopt_ex-interface</URL>
#!   and unpack it using $\texttt{unzip  qsopt}\_\texttt{ex-interface-x.zip}$ in the terminal.
#!   Do this preferably inside the $pkg$ subdirectory
#!   of your GAP 4 installation. It creates a subdirectory called $\texttt{qsopt}\_\texttt{ex-interface}$.
#!   If you do not know the whereabouts of the $pkg$ subdirectory, invoke the following in GAP:
#!   @InsertCode pkg
#!   Look for pkg directory inside any of the paths returned.
#! * Once unpacked, go to $\texttt{qsopt}\_\texttt{ex-interface}$ directory and
#!   run the install script $\texttt{unix-install.sh}$ from the terminal as $\texttt{sh unix-install.sh}$.
#!   This locally installs qsopt exact and its dependencies (GMP <Cite Key="Granlund12"/>,libz and libbz2) in lib and include folders. Alternatively,
#!   if you have qsopt-exact and GMP already installed on your system, you can edit the Makefile inside $\texttt{qsopt}\_\texttt{ex-interface}$  directory
#!   so that gcc finds the .so libraries. In latter case, you must manually '\texttt{make all}' from the terminal inside $\texttt{qsopt}\_\texttt{ex-interface}$ directory.
#! * Above step creates an executable \texttt{qsi} inside the $\texttt{qsopt}\_\texttt{ex-interface}$  directory, which serves as the interface. Note that
#!   before using the package in GAP, one must edit either the environment variable $\texttt{LD}\_\texttt{LIBRARY}\_\texttt{PATH}$ or the
#!   so that \texttt{qsi} finds the locally installed libraries.
#! * One can now start using $\texttt{qsopt}\_\texttt{ex-interface}$ by invoking
#! @BeginCode Read
 LoadPackage( "qsopt_ex-interface");
 #! @EndCode
#! @InsertCode Read
#! from within GAP.
#! @Chapter Usage
#! @Section Available functions
#!  In this section we shall look at the functions provided by qsopt_ex-interface. $\texttt{qsopt}\_\texttt{ex-interface}$ allows
#! GAP to communicate with external LP solver process via a stream object of category IsInputOutputStream(). This steam serves as a
#! handle via which one can load/solve/modify linear programs. Note that it is possible to maintain several such steams (and hence LPs)
#! at any given time. However, the gap commands to solve/modify these LPs that currently available in this package are blocking functions.
#! @Description
#! This function loads an LP by invoking external qsopt-exact LP solver process.
#! It accepts following arguments:
#! * $obj$ - Objective function coefficients, provided as a list
#! * $A$ - A list of lists corresponding to constraints
#! * $b$ - Right hand side of constraints
#! * $linrows$ - A list of indices of members of $A$ that are equalities
#! * $qs\_exec$ - A string describing complete path to 'qsi' executable (including 'qsi')
#! Returns a list $[s,rval]$ where 's' is a gap object of category IsInputOutputStream() and 'rval'
#! $=1/-1$ indicates success/failure. If 'rval=1', 's' is ready to be be used to solve linear programs.
#! @Returns A list
#! @Arguments obj,A,b,linrows,qs_exec,optargs
DeclareGlobalFunction("LoadQSLP");
#! @Description
#! This function loads a new objective.
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! * $obj$ - Objective function coefficients, provided as a list
#! Returns an integer 'rval' $=1/-1$ that indicate success/failure. If 'rval=1',
#! the LP associated with 's' is successfully modified.
#! @Returns An integer
#! @Arguments s,obj
DeclareGlobalFunction("LoadQSLPobj");
#! @Description
#! This function loads an LP by invoking external qsopt-exact LP solver process.
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! * $optargs$ - A list of optional arguments. Currently supports only one optional argument, which is an integer
#!   specifying simplex variant to use: $optargs=[1]$ for primal simplex, $optargs=[2]$ for dual simplex and $optargs=[3]$ for either
#! Returns an integer $status$ that is the integer returned by $\texttt{mpq}\_\texttt{QSget}\_\texttt{status}()$ function.
#! @Returns An integer
#! @Arguments s,optargs
DeclareGlobalFunction("SolveQSLP");
#! @Description
#! This function terminates the external processes associated with given LP handle.
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! Returns Nothing
#! @Returns
#! @Arguments s
DeclareGlobalFunction("FlushQSLP");
#! @Description
#! This function obtains the primal solution along with the associated vertex vertex, for the most recently solved LP.
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! Returns A list $[status,val\_rval,val,x\_rval,x]$ if optimal solution exists and a list $[status]$ otherwise. If $status=1$,
#! $val\_rval$ and $x\_rval$ indicate validity of $val$ and $x$ (valid if $1$ and invalid if $-1$) which are optimal solution
#! and (primal) vertex achieving optimal solution respectively. Other status values correspond to the integer returned by $\texttt{mpq}\_\texttt{QSget}\_\texttt{status}()$ function.
#! @Returns A list
#! @Arguments s
DeclareGlobalFunction("GetQSLPsol_primal");
#! @Description
#! This function obtains the primal solution along with the associated vertex vertex, for the most recently solved LP.
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! Returns A list $[status,val\_rval,val,y\_rval,y]$ if optimal solution exists and a list $[status]$ otherwise. If $status=1$,
#! $val\_rval$ and $x\_rval$ indicate validity of $val$ and $x$ (valid if $1$ and invalid if $-1$) which are optimal solution
#! and (dual) vertex achieving optimal solution respectively. Other status values correspond to the integer returned by $\texttt{mpq}\_\texttt{QSget}\_\texttt{status}()$ function.
#! @Returns A list
#! @Arguments s
DeclareGlobalFunction("GetQSLPsol_dual");
#! @Description
#! This function changes the value of single rhs coefficient in specified row.
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! * $row$ - row index of the inequility whose rhs is to be changed
#! * $coef$ - new rhs coefficient
#! Returns A an integer which is itself returned by QSopt$\_$ex function $\texttt{mpq}\_\texttt{QSchange}\_\texttt{rhscoef}$
#! @Returns An integer
#! @Arguments s,row,coef
DeclareGlobalFunction("ChangeQSrhs");
#! @Description
#! This function deletes the specified row.  (Note that for repeated use, one must
#! relabel rows as QSopt$\_$ex would treat  eg. the second row as first row if we delete the first row)
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! * $row$ - row index of the inequility whose rhs is to be changed
#! Returns A an integer which is itself returned by QSopt$\_$ex function $\texttt{mpq}\_\texttt{QSchange}\_\texttt{rhscoef}$
#! @Returns An integer
#! @Arguments s,row
DeclareGlobalFunction("DelQSrow");
#! @Description
#! This function changes the sense (equality or inequality) of a particular row.
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! * $row$ - row index of the inequility whose sense is to be changed
#! * $newsense$ - A single character string describing the new sense, "L" for $\leq$ and "E" for $=$
#! Returns A an integer which is itself returned by QSopt$\_$ex function $\texttt{mpq}\_\texttt{QSchange}\_\texttt{sense}$
#! @Returns An integer
#! @Arguments s,row,coef
DeclareGlobalFunction("ChangeQSsense");
#! @Description
#! This function changes a particular coefficient in the constraint matrix.
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! * $row$ - row index of the inequility to which the coefficient to be changed belongs
#! * $col$ - column index of the inequility whose sense is to be changed
#! * $coef$ - A rational number or an integer
#! Returns A an integer which is itself returned by QSopt$\_$ex function $\texttt{mpq}\_\texttt{QSchange}\_\texttt{sense}$
#! @Returns An integer
#! @Arguments s,row,coef
DeclareGlobalFunction("ChangeQScoef");
#! @Description
#! This function displays an already loaded LP.
#! It accepts following arguments:
#! * $s$ - gap object of category IsInputOutputStream(), handle to an already loaded LP
#! Returns Nothing
#! @Returns Nothing
#! @Arguments s
DeclareGlobalFunction("DisplayLPQS");

DeclareGlobalFunction("qsoptformatstr");
DeclareGlobalFunction("qsoptformatstr2");
