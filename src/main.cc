//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//                                                                          
//                                                                          
//        This file is part of Libmoleculizer
//
//        Copyright (C) 2001-2008 The Molecular Sciences Institute.
//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
// Moleculizer is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published 
// by the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// Moleculizer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Moleculizer; if not, write to the Free Software Foundation
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307,  USA
//    
// END HEADER
// 
// Original Author:
//   Larry Lok, Research Fellow, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//              
//

#include "utl/insuffArgsXcpt.hh"
#include "utl/unkArgXcpt.hh"
#include "utl/arg.hh"
#include "mzr/moleculizer.hh"
#include "utl/badFileNameXcpt.hh"
#include "utl/utility.hh"
#include "utl/linearHash.hh"

#include "utl/stdIncludes.hh"
#include <stdlib.h>
#include <time.h>


void
processCommandLineArgs(int argc,
char* argv[],
mzr::moleculizer& theMzrObject,
std::string* fileName);



// This is a global that is used in processCommandLineArgs
bool INTERACTIVE = false;
bool VERBOSITY = false;
int DEBUG_NUM = 30;

int
main(int argc, char** argv)
{

try
{
std::string filename;
mzr::moleculizer theApp;

processCommandLineArgs( argc, argv, theApp, &filename);
theApp.attachFileName( filename );

return 0;
}

catch(const std::exception& rExcept)
{
std::cerr << rExcept.what()
<< std::endl;
return 1;
}
}




void
processCommandLineArgs(int argc,
char* argv[],
mzr::moleculizer& theMzrObject,
std::string* fileName)
{
// Set the default operation of the MzrObject
theMzrObject.setGenerateDepth( 1 );

srand(42);

bool filenameWasSeen( false );

// Skip the command name.
argc--;
argv++;

// Peel off arguments one by one.
while(0 < argc)
{
std::string arg(*argv);
argv++;
argc--;

if(arg.substr(0,2) == "-g")
{
if (arg.size() == 2) continue;

std::string strDepth( arg.substr(2, arg.size() ) );
int depth = utl::argMustBeNNInt(strDepth);

theMzrObject.setGenerateDepth( depth );
}
else if ( arg == "-n" )
{
std::string aNumber = utl::mustGetArg(argc, argv);
DEBUG_NUM = utl::argMustBeNNInt( aNumber );
}
else if (arg == "-v" )
{
VERBOSITY = true;
}
else if (arg == "-i")
{
INTERACTIVE = true;
}
else if (arg == "-s")
{
std::string argument = utl::mustGetArg(argc, argv);
utl::linearHash lh;
srand( lh(argument) );
}
else if (arg == "-r" )
{
srand(time(NULL));
}

// Sets the tolerance for reaction rescheduling.
else if (arg == "-T")
{
std::string toleranceString
= utl::mustGetArg(argc,
argv);

double tolerance
= utl::argMustBeNNDouble(toleranceString);

theMzrObject.setTolerance(tolerance);
}

// This is the filename, although it's useless because
// this was already taken care of elsewhere.
else if (arg == "-f")
{
filenameWasSeen = true;
*fileName = utl::mustGetArg(argc,
argv);
}

else throw utl::unkArgXcpt(arg);
}

if (!filenameWasSeen)
{
throw utl::badFileNameXcpt();
}
}


