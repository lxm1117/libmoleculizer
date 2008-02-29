/////////////////////////////////////////////////////////////////////////////
// Moleculizer - a stochastic simulator for cellular chemistry.
// Copyright (C) 2008 Nathan Addy
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//    
/////////////////////////////////////////////////////////////////////////////


#ifndef RXNNETWORKCATALOG_HH
#define RXNNETWORKCATALOG_HH


#include "fndExceptions.hh"
#include "rnHelperClasses.hh"
#include "utl/autoCatalog.hh"

#include <vector>
#include <list>
#include <stack>
#include <sstream>

// NOTES
// 2/26/08 
// Trying to update this file to be faster.

namespace fnd
{

    // I obviously need to add some kind of public API function for expanding by a speciesID.

    // Both speciesType and reactionType must have a public function
    // void expandReactionNetwork(). 
    // That is,  derived from fnd::reactionNetworkComponent.
  
    template <typename speciesT,
              typename reactionT>
    class ReactionNetworkDescription
    {
    
    public:
        typedef speciesT SpeciesType;
        typedef SpeciesType* ptrSpeciesType;
    
        typedef reactionT ReactionType;
        typedef ReactionType* ptrReactionType;

        typedef std::map< const std::string*, ptrSpeciesType, aux::compareByPtrValue<std::string> > SpeciesCatalog;
        typedef typename SpeciesCatalog::iterator SpeciesCatalogIter;
        typedef typename SpeciesCatalog::const_iterator SpeciesCatalogCIter;

        typedef std::list<ptrSpeciesType> SpeciesList;
        typedef std::list<ptrReactionType> ReactionList;

        typedef std::list< std::pair<std::string*, ptrSpeciesType> > SpeciesListCatalog;

        typedef typename SpeciesListCatalog::iterator SpeciesListCatalogIter;
        typedef typename SpeciesListCatalog::const_iterator SpeciesListCatalogCIter;
        
        typedef typename SpeciesList::iterator SpeciesListIter;
        typedef typename SpeciesList::const_iterator SpeciesListCIter;
    
        typedef typename ReactionList::iterator ReactionListIter;
        typedef typename ReactionList::const_iterator ReactionListCIter;

    public:

        SpeciesType&
        findSpecies(const std::string& name) 
        {
            const std::string* pString = &name;
            SpeciesCatalogCIter theIter = theSpeciesListCatalog.find(pString);
            if (theIter != theSpeciesListCatalog.end() )
            {
                return *(theIter->second);
            }
            else
            {
                throw fnd::NoSuchSpeciesXcpt(name);
            }
            
        }

        SpeciesListCatalog& 
        getSpeciesCatalog()
        {
            return theSpeciesListCatalog;
        }
      

        const SpeciesListCatalog& 
        getSpeciesCatalog() const
        {
            return theSpeciesListCatalog;
        }

        const ReactionList&
        getReactionList() const
        {
            return theCompleteReactionList;
        }

        // These two functions are the interface that reaction generators use 
        // to record their species.  They add the (speciesName, speciesPtr)
        // and (reactionName, reactionPtr) entries to the catalog respectively.
        // If the object being recorded is new, we also record it as a hit in the
        // total number of species/reactions as well as the delta number of species/
        // reactions.
      
        bool 
        recordSpecies( ptrSpeciesType pSpecies)
        {
//              std::string* pSpeciesName = new std::string( pSpecies->getName() );
             
//              Species
             
             

//             // Remove this loop and find some real algorithm for doing this.
//             SpeciesListCatalogIter iter = theSpeciesListCatalog.begin();
//             SpeciesListCatalogIter endIter = theSpeciesListCatalog.end();
            
//             while( *(iter->first) < *pSpeciesName && iter != endIter) ++iter;            
            
//             // So now iter points to the first <string*, species*>  element that has 
//             // *(iter->first) >= the <pSpeciesName, ptrSpecies> we are trying to insert.
//             // If we are already in the dictionary, return.
//             if (*pSpeciesName == *(iter->first)) return false;

//             // Otherwise, put is in the various lists.
//             theSpeciesListCatalog.insert(iter, std::make_pair( pSpeciesName, pSpecies) );
//             theDeltaSpeciesList.push_back( pSpecies );
                                                               
//             return true;

            std::string* pSpeciesName = new std::string( pSpecies->getName() );

            SpeciesCatalogCIter iter = theSpeciesListCatalog.find( pSpeciesName);
            if (iter == theSpeciesListCatalog.end() )
            {
                theSpeciesListCatalog.insert( std::make_pair( pSpeciesName, pSpecies) );
                theDeltaSpeciesList.push_back( pSpecies );
                return true;
            }
            else
            {
                return false;
            }
                                                               

        }

        bool
        recordReaction( ptrReactionType pRxn )
        {
            // At the moment, this does not check to see if a reaction has been added before.
            theCompleteReactionList.push_back( pRxn );
            theDeltaReactionList.push_back( pRxn );
            return true;
        }
        
        // These are not used by anyone at the moment.  It might be better to use them instead
        // of the plain old recordSpecies/recordReaction functions, but who knows.
        void mustRecordSpecies( ptrSpeciesType pSpecies ) throw(utl::xcpt)
        {
            if ( !recordSpecies( pSpecies ) )
            {
                throw DuplicatedCatalogEntryXcpt( pSpecies->getName() );
            }
        }
  
        void
        mustRecordReaction( ptrReactionType pRxn) throw(utl::xcpt)
        {

          if ( recordReactions( pRxn ) ) 
            {
              return;
            }
          else
            {
              throw DuplicatedCatalogEntryXcpt( pRxn->getName() );
            }

        }

        unsigned int 
        getTotalNumberSpecies() const
        {
            return theSpeciesListCatalog.size();
        }

        unsigned int 
        getTotalNumberReactions() const
        {
            return theCompleteReactionList.size();
        }

        // The getDeltaNumber(Species|Reactions) functions return the number
        // of those objects created since the last time the recordCurrentState
        // funsource ction was source called.
        unsigned int 
        getNumberDeltaReactions() const
        {
            return theDeltaReactionList.size();
        }

        unsigned int 
        getNumberDeltaSpecies() const
        {
            return theDeltaSpeciesList.size();
        }

        void resetCurrentState()
        { 
           theDeltaSpeciesList.clear();
           theDeltaReactionList.clear();
        }

        // I doubt very much this will work.
        // void resetCurrentState() = recordCurrentState;

        void incrementNetworkBySpeciesName(const std::string& rName) throw(utl::xcpt)
        {
            resetCurrentState();
            
            SpeciesCatalogCIter iter = theSpeciesListCatalog.find( &rName );

            if (iter != theSpeciesListCatalog.end() )
            {
                iter->second->expandReactionNetwork();
            }
            else
            {
                throw NoSuchSpeciesXcpt( rName );
            }
        }

        ~ReactionNetworkDescription()
        {
          // We don't memory manage any SpeciesType* or ReactionType*, but we do memory 
          // manage the string* in theSpeciesListCatalog.

            // IMPORTANT.
            // Not deleting the std::string*'s leaks memory here...
//           std::for_each(theSpeciesListCatalog.begin(),
//                         theSpeciesListCatalog.end(),
//                         aux::doDeleteStringPtrs<SpeciesListCatalog>());

        }

        ReactionNetworkDescription(bool reserveMemory = true)
        {
             if (reserveMemory)
             {
                 // theSpeciesListCatalog.reserve( getPredictedSpeciesNetworkSize() );
                 //                 theCompleteReactionList.reserve( getPredictedSpeciesNetworkSize() * \
                 // static_cast<unsigned int>(getPredictedSpeciesReactionRatio() + 1) );
             }
        }

        static void setPredictedSpeciesNetworkSize(unsigned int maxSize)
        {
            ReactionNetworkDescription<SpeciesType, ReactionType>::estimatedTotalNumberSpecies = maxSize;
        }

        static void setPredictedSpeciesReactionRatio(float maxSize)
        {
            if (maxSize <= 0.0f) return
            ReactionNetworkDescription<SpeciesType, ReactionType>::estimatedSpeciesReactionRatio = maxSize;
        }

        static unsigned int getPredictedSpeciesNetworkSize()
        {
            return ReactionNetworkDescription<SpeciesType, ReactionType>::estimatedTotalNumberSpecies;
        }

        static float getPredictedSpeciesReactionRatio()
        {
            return ReactionNetworkDescription<SpeciesType, ReactionType>::estimatedSpeciesReactionRatio;
        }

        // -- The pointers to the strings in theSpeciesListCatalog ARE memory managed.
        // -- The pointers to the species and reactions ARE NOT memory managed here.

        SpeciesCatalog theSpeciesListCatalog;
        // SpeciesListCatalog theSpeciesListCatalog;
        ReactionList theCompleteReactionList;
        
        SpeciesList    theDeltaSpeciesList;
        ReactionList    theDeltaReactionList;

        // Static
        static unsigned int estimatedSpeciesReactionNetworkSize;
        static float estimatedSpeciesReactionRatio;
        
    };    

    template <typename speciesT, typename reactionT>
    unsigned int 
    ReactionNetworkDescription<speciesT, reactionT>::estimatedSpeciesReactionNetworkSize = 1e6;
    
    template <typename speciesT, typename reactionT>
    float
    ReactionNetworkDescription<speciesT, reactionT>::estimatedSpeciesReactionRatio = 4.0f;
    

    
}
#endif // RXNNETWORKCATALOG_HH
    
