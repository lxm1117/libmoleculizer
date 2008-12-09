//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2008
//
// Modifing Authors:
//
//


#ifndef RXNNETWORKCATALOG_HH
#define RXNNETWORKCATALOG_HH

#include "utl/defs.hh"

// Depreciate this file.
#include "fnd/utility.hh"

#include "fnd/fndXcpt.hh"
#include "fnd/basicReaction.hh"
#include "fnd/basicSpecies.hh"
#include <boost/function.hpp>

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
    DECLARE_TYPE( speciesT, SpeciesType );
    DECLARE_TYPE( reactionT, ReactionType );

    // Some ReactionNetowrkDescription;
    DECLARE_TYPE( String, SpeciesName );
    DECLARE_TYPE( String, SpeciesID );

    typedef const SpeciesID* SpeciesHandle;

    typedef std::map<SpeciesHandle, SpeciesTypePtr, aux::compareByPtrValue<SpeciesID> > SpeciesCatalog;
    typedef typename SpeciesCatalog::iterator SpeciesCatalogIter;
    typedef typename SpeciesCatalog::const_iterator SpeciesCatalogCIter;

    typedef std::list<SpeciesTypePtr> SpeciesList;
    typedef std::list<ReactionTypePtr> ReactionList;

    typedef std::multimap<SpeciesTypePtr, ReactionTypePtr> ParticipatingSpeciesRxnMap;

    typedef std::list< std::pair<std::string*, SpeciesTypePtr> > SpeciesListCatalog;

    typedef typename SpeciesListCatalog::iterator SpeciesListCatalogIter;
    typedef typename SpeciesListCatalog::const_iterator SpeciesListCatalogCIter;

    typedef typename SpeciesList::iterator SpeciesListIter;
    typedef typename SpeciesList::const_iterator SpeciesListCIter;

    typedef typename ReactionList::iterator ReactionListIter;
    typedef typename ReactionList::const_iterator ReactionListCIter;

public:

    SpeciesTypePtr
    findSpecies( const std::string& name ) throw( fnd::NoSuchSpeciesXcpt )
    {
        SpeciesCatalogIter theIter = theSpeciesListCatalog.find( &name );
        if ( theIter != theSpeciesListCatalog.end() )
        {
            return theIter->second;
        }
        else
        {
            throw fnd::NoSuchSpeciesXcpt( name );
        }

    }

    SpeciesTypeCptr
    findSpecies( const std::string& name ) const throw( fnd::NoSuchSpeciesXcpt )
    {
        SpeciesCatalogCIter theIter = theSpeciesListCatalog.find( &name );

        if ( theIter != theSpeciesListCatalog.end() )
        {
            return theIter->second;
        }
        else
        {
            throw fnd::NoSuchSpeciesXcpt( name );
        }
    }


    bool
    findReactionWithSubstrates( SpeciesTypeCptr A,
                                std::vector<ReactionTypeCptr>& reactionVector )
    {

        // This is bad.  It feels semantically correct, but probably means something
        // should be refactored.
        const_cast<SpeciesTypePtr>(A)->expandReactionNetwork();



        reactionVector.clear();
        typename ParticipatingSpeciesRxnMap::const_iterator iter = singleSubstrateRxns.find( const_cast<SpeciesTypePtr>(A) );


        while ( iter->first == A )

        {
            reactionVector.push_back( iter->second );
            ++iter;
        }

        return ( !reactionVector.empty() );
    }

    bool
    findReactionWithSubstrates( SpeciesTypeCptr A,
                                SpeciesTypeCptr B,
                                std::vector<ReactionTypeCptr>& reactionVector )
    {
        // This feels wrong (although semantically, so right), and probably means things 
        // should be refactored.
        const_cast<SpeciesTypePtr>(A)->expandReactionNetwork();
        const_cast<SpeciesTypePtr>(B)->expandReactionNetwork();

        reactionVector.clear();

        if ( A == B )
        {
            BOOST_FOREACH( ReactionTypePtr ptrRxn, theCompleteReactionList )
            {
                if ( ptrRxn->getReactantStochiometry( A ) == 2 )
                {
                    reactionVector.push_back( ptrRxn );
                }
            }
        }
        else
        {
            BOOST_FOREACH( ReactionTypePtr ptrRxn, theCompleteReactionList )
            {
                if ( ptrRxn->hasReactant( A ) && ptrRxn->hasReactant( B ) )
                {
                    reactionVector.push_back( ptrRxn );
                }
            }
        }

        return ( !reactionVector.empty() );
    }

    bool
    checkSpeciesIsKnown( const std::string& speciesName ) const
    {
        return !( theSpeciesListCatalog.find( &speciesName ) == theSpeciesListCatalog.end() );

    }

    SpeciesCatalog&
    getSpeciesCatalog()
    {
        return theSpeciesListCatalog;
    }


    const SpeciesCatalog&
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
    recordSpecies( SpeciesTypePtr pSpecies )
    {
        SpeciesHandle speciesHandle( new SpeciesID( pSpecies->getName() ) );

        if ( theSpeciesListCatalog.find( speciesHandle ) == theSpeciesListCatalog.end() )
        {
            theSpeciesListCatalog.insert( std::make_pair( speciesHandle, pSpecies ) );
            theDeltaSpeciesList.push_back( pSpecies );

            // It's a new species, so call all the new species callbacks on this.  
            BOOST_FOREACH( NewSpeciesCallback cb, newSpeciesCallbacks)
            {
                cb( pSpecies );
            }

            return true;
        }
        else
        {
            return false;
        }

    }

    bool
    recordSpecies( SpeciesTypePtr pSpecies, SpeciesID& name )
    {
        SpeciesHandle speciesHandle( new SpeciesID( pSpecies->getName() ) );

        // Put the speciesID into the name.
        name = *speciesHandle;

        if ( theSpeciesListCatalog.find( speciesHandle ) == theSpeciesListCatalog.end() )
        {
            theSpeciesListCatalog.insert( std::make_pair( speciesHandle, pSpecies ) );
            theDeltaSpeciesList.push_back( pSpecies );

            // It's a new species, so call all the new species callbacks on this.  
            BOOST_FOREACH( NewSpeciesCallback cb, newSpeciesCallbacks)
            {
                cb( pSpecies );
            }

            return true;
        }
        else
        {
            return false;
        }

    }

    bool
    recordReaction( ReactionTypePtr pRxn )
    {
        if ( !pRxn->isStandardReaction() )
        {
            throw utl::FatalXcpt("Error in reactionNetworkCatalog::recordReaction.  Reaction passed in is non-standard");
        }

        theCompleteReactionList.push_back( pRxn );
        theDeltaReactionList.push_back( pRxn );

        switch ( pRxn->getNumberOfReactants() )
        {
            SpeciesTypePtr theSpeciesPtr;

        case 0:
            // Register in the map of creation reactions
            zeroSubstrateRxns.push_back( pRxn );
            break;

        case 1:
            // Register in a multi map of 1->? reactions
            theSpeciesPtr = pRxn->getReactants().begin()->first;
            singleSubstrateRxns.insert( std::make_pair( theSpeciesPtr, pRxn ) );
            break;
        case 2:
            BOOST_FOREACH( typename fnd::basicReaction<SpeciesType>::multMap::value_type vt,
                           pRxn->getReactants() )
            {
                theSpeciesPtr = vt.first;
                doubleSubstrateRxns.insert( std::make_pair( theSpeciesPtr, pRxn ) );
            }
            break;
        default:
            throw utl::FatalXcpt("Error in reactionNetworkCatalog::recordReaction.  Reaction is non-standard. (jfjhnnnnnfhd348)"  );
            break;
        }

        // Call each of the registered callbacks on the newly installed reaction.
        BOOST_FOREACH( NewReactionCallback cb, newReactionCallbacks )
        {
            cb( pRxn );
        }

        return true;
    }


    int
    getPopulation( const std::string& rName ) const
    {
        return findSpecies( rName ).getPop();
    }

    // These are not used by anyone at the moment.  It might be better to use them instead
    // of the plain old recordSpecies/recordReaction functions, but who knows.
    void mustRecordSpecies( SpeciesTypePtr pSpecies ) throw( utl::xcpt )
    {
        if ( !recordSpecies( pSpecies ) )
        {
            throw DuplicatedCatalogEntryXcpt( pSpecies->getName() );
        }
    }

    void
    mustRecordReaction( ReactionTypePtr pRxn ) throw( utl::xcpt )
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

    void incrementNetworkBySpeciesName( const SpeciesID& rName ) throw( utl::xcpt )
    {
        SpeciesCatalogCIter iter = theSpeciesListCatalog.find( &rName );

        if ( iter != theSpeciesListCatalog.end() )
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

        BOOST_FOREACH( typename SpeciesCatalog::value_type i, theSpeciesListCatalog )
        {
            delete i.first;
        }
    }

    // -- The pointers to the strings in theSpeciesListCatalog ARE memory managed.
    // -- The pointers to the species and reactions ARE NOT memory managed here.


    SpeciesCatalog theSpeciesListCatalog;
    ReactionList theCompleteReactionList;
    ReactionList unaryReactionList;
    ReactionList binaryReactionList;

    SpeciesList    theDeltaSpeciesList;
    ReactionList    theDeltaReactionList;

    // 0->1, 1->0, 1->1, 1->2, 2->1
    ReactionList zeroSubstrateRxns;
    ParticipatingSpeciesRxnMap singleSubstrateRxns;
    ParticipatingSpeciesRxnMap doubleSubstrateRxns;

    /////////////////////////
    // NEWCODE
    //
    typedef boost::function<void( const ReactionType* )> NewReactionCallback;
    typedef boost::function<void( const SpeciesType*)> NewSpeciesCallback;

    std::vector<NewReactionCallback> newReactionCallbacks;
    std::vector<NewSpeciesCallback> newSpeciesCallbacks;

public:
    void addNewReactionCallback( NewReactionCallback cb )
    {
        newReactionCallbacks.push_back( cb );
    }

    void addNewSpeciesCallback( NewSpeciesCallback cb )
    {
        newSpeciesCallbacks.push_back( cb );
    }

    //
    /////////////////////////

};



}
#endif // RXNNETWORKCATALOG_HH
