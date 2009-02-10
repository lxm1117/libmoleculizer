//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//
//        This file is part of Libmoleculizer
//
//        Copyright (C) 2001-2009 The Molecular Sciences Institute.
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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//


#include <string>
#include <iostream>
#include <iterator>
#include "utl/arg.hh"
#include "mzr/moleculizer.hh"

using namespace std;


// This should be refactored, as it is clearly getting idiotic...
void
processCommandLineArgs( int argc, char* argv[], std::string& theFileName, int& number, int& printOutput, std::string& theOutputFileName, int& maxSpecies, int& maxRxns);

void
displayHelpAndExitProgram();

bool getUninitializedSpecies( const mzr::moleculizer& moleculizerRef, std::string& speciesName);

void printAllSpeciesStreams(mzr::moleculizer& theMolzer);

void printStreamByName( mzr::moleculizer& refMolzer, const std::string& streamName);
void printStreamByTag( mzr::moleculizer& refMolzer, const std::string& streamName);

void printAllSpeciesByName(mzr::moleculizer& theMolzer);
void printAllSpeciesByID(mzr::moleculizer& theMolzer, std::string str = "");

mzr::moleculizer::CachePosition
createBoundedNetwork(mzr::moleculizer& refMolzer, int maxSpec, int maxRxns);

void createFullNetwork(mzr::moleculizer& refMolzer);
void doNIterations(mzr::moleculizer& refMolzer, int number);

int main(int argc, char* argv[])
{

  std::string fileName;
  std::string outputFileName("");
  int number = -1;
  int printOutput = 0;

  int maxSpecies = -1;
  int maxRxns = -1;

  enum RunMode { Full = 0,
                 BoundedRun = 1,
                 Iterations = 2 };

  int modeRun = -1;

  processCommandLineArgs(argc, argv, fileName, number, printOutput, outputFileName, maxSpecies, maxRxns);

  mzr::moleculizer theMoleculizer;
  theMoleculizer.attachFileName( fileName );

  std::cout << "There are initially " << theMoleculizer.getTotalNumberSpecies() << " species and " 
            << theMoleculizer.getTotalNumberReactions() << " reactions. " << std::endl;

  if( printOutput )
  {
      printAllSpeciesByID(theMoleculizer);
  }


  mzr::moleculizer::CachePosition pos;
  if (number < 0)
  {

      if (maxSpecies > 0 || maxRxns > 0)
      {
          
          modeRun = BoundedRun;

          std::cout << "Creating bounded network with (maxSpec, maxRxn) = ( " ;

          if (maxSpecies > 0 ) std::cout << maxSpecies;
          else std::cout << "inf";

          std::cout << ", ";

          if (maxRxns > 0 ) std::cout << maxRxns;
          else std::cout << "inf";

          std::cout << ") " << std::endl;

          pos = createBoundedNetwork( theMoleculizer, maxSpecies, maxRxns );
      }
      else
      {

          modeRun = Full;

          std::cout << "Expanding whole network..." << std::endl;
          createFullNetwork( theMoleculizer );
      }

  }
  else
  {
      modeRun = Iterations;
      doNIterations( theMoleculizer, number );
  }

  std::cout << "################################################" << '\n';
  
  std::cout << "After processing: \n" ;

  if (modeRun == Full || modeRun == Iterations)
   {
       std::cout << "There are " 
                 << theMoleculizer.getTotalNumberReactions() << " reactions and " 
                 << theMoleculizer.getTotalNumberSpecies() << " species in " 
                 << theMoleculizer.getNumberOfPlexFamilies() << " families in the network." << std::endl;
   }
   else
   {
       int numRxns = std::distance(theMoleculizer.theDeltaReactionList.begin(), pos.second);
       int numSpecies = std::distance(theMoleculizer.theDeltaSpeciesList.begin(), pos.first);
       std::cout << "There are " 
                 << numSpecies << " species and " 
                 << numRxns<< " reactions in the bounded network.  " << '\n'
                 << "Should be bounded by " << maxSpecies << " species and "
                 << maxRxns << " reactions."
                 << std::endl;
   }

   if (printOutput)
   {
       std::cout << "################################################" << '\n';
       printAllSpeciesStreams(theMoleculizer);

       std::cout << "################################################" << '\n';

       printAllSpeciesByName(theMoleculizer);
       std::cout << "################################################" << '\n';
       printAllSpeciesByID(theMoleculizer);

       std::cout << "################################################" << '\n';
   }

   if(outputFileName != "")
   {
       if (modeRun != BoundedRun)
       {
           theMoleculizer.writeOutputFile(outputFileName, true);
       }
       else
       {
           theMoleculizer.writeOutputFile( outputFileName, true, pos);
       }
   }

  return 0;
  
}

void printAllSpeciesStreams(mzr::moleculizer& refMolzer)
{
    std::vector<std::string> theStreams;
    refMolzer.getSpeciesStreams( theStreams );
    std::cout << " There are " << theStreams.size() << " streams." << std::endl;

    for( std::vector<std::string>::const_iterator strIter = theStreams.begin();
         strIter != theStreams.end();
         ++strIter)
    {
        printStreamByName( refMolzer, *strIter);
        std::cout << "################################################" << '\n';
        printStreamByTag( refMolzer, *strIter);
    }
}

void printStreamByName( mzr::moleculizer& refMolzer, const std::string& streamName)
{
    std::cout << "'" << streamName << "'\n[\n" ;

    std::vector<const mzr::mzrSpecies*> theSpecies;

    refMolzer.getSpeciesInSpeciesStream( streamName, theSpecies);

    for( std::vector<const mzr::mzrSpecies*>::const_iterator specIter = theSpecies.begin();
         specIter != theSpecies.end();
         ++specIter)
    {
        std::cout << streamName << "@@" << (*specIter)->getName() << '\n';
    }

    std::cout << ']' << std::endl;
}

void printStreamByTag( mzr::moleculizer& refMolzer, const std::string& streamName)
{
    std::cout << "'" << streamName << "'\n[\n" ;

    std::vector<const mzr::mzrSpecies*> theSpecies;

    refMolzer.getSpeciesInSpeciesStream( streamName, theSpecies);

    for( std::vector<const mzr::mzrSpecies*>::const_iterator specIter = theSpecies.begin();
         specIter != theSpecies.end();
         ++specIter)
    {
        std::cout << streamName << "@@" << (*specIter)->getTag() << '\n';
    }

    std::cout << ']' << std::endl;
}

bool getUninitializedSpecies( const mzr::moleculizer& moleculizerRef, std::string& speciesName)
{
    for( mzr::moleculizer::SpeciesCatalog::const_iterator specIter = moleculizerRef.getSpeciesCatalog().begin();
         specIter != moleculizerRef.getSpeciesCatalog().end();
         ++specIter)
    {
        if(!specIter->second->hasNotified())
        {
            speciesName = *(specIter->first);
            return true;
        }
    }

    return false;
}


void processCommandLineArgs( int argc, char* argv[], std::string& mzrFile, int& number, int& print, std::string& outputFileName, int& maxSpec, int& maxRxns)
{

    bool file( false );

    // Skip the command name
    argc--;
    argv++;

    if ( argc == 0 )
    {
        displayHelpAndExitProgram();
    }

    while ( 0 < argc )
    {
        std::string arg( *argv );
        argv++;
        argc--;

        if ( arg == "--help" )
        {
            displayHelpAndExitProgram();
        }

        if ( arg == "-f" || arg == "--file")
        {
          mzrFile = utl::mustGetArg( argc, argv );
	  file = true;
        }
        if( arg == "-n" )
        {
            std::string numAsString = utl::mustGetArg( argc, argv);
            number = utl::argMustBeNNInt( numAsString );
        }
        if( arg == "-v" )
        {
            print = 1;
        }
        if(arg == "-q")
        {
            print = 0;
        }
        if(arg == "-o")
        {
            outputFileName = utl::mustGetArg( argc, argv );
        }
        if(arg == "-s")
        {
            std::string numAsString = utl::mustGetArg( argc, argv);
            maxSpec = utl::argMustBeNNInt( numAsString );
        }
        if(arg == "-r")
        {
            std::string numAsString = utl::mustGetArg( argc, argv);
            maxRxns = utl::argMustBeNNInt( numAsString );
        }
    }

    if ( !file )
    {
      std::cerr << "Error, a file must be specified with an -f <FILE> parameter." << std::endl;
      exit( 1 );
    }

}

void printAllSpeciesByName(mzr::moleculizer& theMolzer)
{
    for( mzr::moleculizer::SpeciesCatalog::const_iterator specIter = theMolzer.theSpeciesListCatalog.begin();
         specIter != theMolzer.theSpeciesListCatalog.end();
         ++specIter)
    {
        std::cout << "ALL@@" << *specIter->first << std::endl;
    }
}

void printAllSpeciesByID(mzr::moleculizer& theMolzer, std::string str)
{
    for( mzr::moleculizer::SpeciesCatalog::const_iterator specIter = theMolzer.theSpeciesListCatalog.begin();
         specIter != theMolzer.theSpeciesListCatalog.end();
         ++specIter)
    {
        std::cout << str << theMolzer.convertSpeciesTagToSpeciesID( *specIter->first ) << std::endl;
    }
}

void displayHelpAndExitProgram()
{

    cout << "Usage: network_expander -f <FILE>" << endl;

    cout << "This is a demonstration program that demonstrates how libmoleculizer can be used as " << endl;
    cout << "a component for expanding whole reaction networks and displaying some information." << endl;

    cout << "on them.";

    cout << "Libmoleculizer should have come with associated documentation.  Please read it for more details." << endl;
    cout << "\tNathan Addy <addy@molsci.org>\n\tSeptember 18, 2008." << endl;

    exit( 0 );
}


mzr::moleculizer::CachePosition
createBoundedNetwork(mzr::moleculizer& refMolzer, int maxSpec, int maxRxns)
{
    return refMolzer.generateCompleteNetwork( maxSpec, maxRxns);
}

void createFullNetwork(mzr::moleculizer& refMolzer)
{
    std::cout << "Generating complete network... " << std::endl;
    refMolzer.generateCompleteNetwork();
    std::cout << "Done!" << std::endl;
}

void doNIterations(mzr::moleculizer& refMolzer, int number)
{

    std::cout << "Expanding network for " << number << " iterations. " << std::endl;


      for ( int iterNdx = 0; iterNdx != number; ++iterNdx)
      {
          std::cout << "----------------------------------------" << std::endl;
          std::cout << "Iteration " << iterNdx + 1 << "/" << number << ": " << std::endl;

          std::string tag;
          int numExpanded = 0;
          if( getUninitializedSpecies(refMolzer, tag) )
          {
              ++numExpanded;
              std::string uniqueId = refMolzer.convertSpeciesTagToSpeciesID( tag );
              std::cout << numExpanded << " Expanding (" << tag << " -> " << uniqueId << " ) " << std::endl;
              refMolzer.incrementNetworkBySpeciesTag( tag );

              std::cout << "-----------" << std::endl;

              std::cout << "(numSpec, numRxns ) = " << "(" << refMolzer.getTotalNumberSpecies() << ", " << refMolzer.getTotalNumberReactions() << ")" << std::endl;
              printAllSpeciesByID( refMolzer );

              std::cout << "-----------" << std::endl;


          }
          else
          {
              std::cout << "All species incremented." << std::endl;
              std::cout << "Breaking on iteration " << iterNdx << std::endl;
              break;
          }
      }

}
