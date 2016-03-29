LoadPackage( "AutoDoc" );

AutoDoc( "qsopt_ex-interface" : scaffold := true );

PrintTo( "VERSION", PackageInfo( "qsopt_ex-interface" )[1].Version );

QUIT;
