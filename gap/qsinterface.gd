#############################################################################
##
##                                                      itap package
##
##  Copyright 2015,           Jayant Apte and John Walsh, Drexel University
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
#!  In this section we shall look at the functions provided by qsopt_ex-interface.
#! @Description
#! This function solves an LP by involking external qsopt-exact LP solver process.
#! It accepts following arguments:
#! * $obj$ - Objective function coefficients, provided as a list
#! * $A$ - A list of lists corresponding to constraints
#! * $b$ - Right hand side of constraints
#! * $linrows$ - A list of indices of members of $A$ that are equalities
#! * $qs\_exec$ - A string describing complete path to 'qsi' executable
#! *
#! Returns a list $[rval,val\_rval,val,x\_rval,x]$
#! @Returns A list
#! @Arguments obj,A,b,linrows,qs_exec,optargs
DeclareGlobalFunction("SolveLPQS");

DeclareGlobalFunction("qsoptformatstr");
