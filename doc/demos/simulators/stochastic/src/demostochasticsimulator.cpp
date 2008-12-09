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
//   Nathan Addy, Scientific Programmer, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//


#include <iostream>
#include "demostochasticsimulator.hpp"
#include <algorithm>

SimpleStochasticSimulator::SimpleStochasticSimulator( std::string rulesfile, std::string modelfile )
        :
        SimpleSimulator()
{
    loadRules(rulesfile);
    loadModel(modelfile);

    std::cout << "Prior to initialization there are " 
              << getNumSpecies() << " species and " << getNumRxns() << " reactions" << std::endl;

    recordNewReactions();

    initialize();
    
    std::cout << "After initialization there are " 
              << getNumSpecies() << " species and " << getNumRxns() << " reactions" << std::endl;
}

void SimpleStochasticSimulator::recordNewReactions()
{
    std::copy( ptrSpeciesReactionGenerator->theDeltaReactionList.begin(),
               ptrSpeciesReactionGenerator->theDeltaReactionList.end(),
               std::back_inserter( reactions ) );

    if ( ptrSpeciesReactionGenerator->theDeltaReactionList.size() > 0 )
    {
        std::cout << "Adding to StochasticSimulator: " << std::endl;
    }

    BOOST_FOREACH( mzr::mzrReaction* rxn, ptrSpeciesReactionGenerator->theDeltaReactionList )
    {
        printRxn( rxn );
    }

    assert( reactions.size() == ptrSpeciesReactionGenerator->theCompleteReactionList.size() );

//     assert( reactions.size() == ptrSpeciesReactionGenerator->zeroSubstrateRxns.size() + \
//             ptrSpeciesReactionGenerator->singleSubstrateRxns.size() +    \
//             ptrSpeciesReactionGenerator->doubleSubstrateRxns.size() );

    ptrSpeciesReactionGenerator->resetCurrentState();
}

mzr::mzrReaction* SimpleStochasticSimulator::calculateReactionToFire()
{

    // Get a list of potential reactions, in this case a list of reactions
    // that have all positive numbers of substrates.

    std::vector<mzr::moleculizer::ReactionTypePtr> potentialReactions;
    getReactionsWithPositivePropensity( potentialReactions );

    if ( potentialReactions.size() == 0 )
    {
        std::cout << "There are no reactions with positive propensity..." << std::endl;
        return NULL;
    }


    // Pick a random reaction.
    int randomIndex = rand() % potentialReactions.size();
    mzr::moleculizer::ReactionTypePtr randomReactionPtr = potentialReactions[ randomIndex ];

    return randomReactionPtr;
}


void SimpleStochasticSimulator::getReactionsWithPositivePropensity( std::vector<mzr::mzrReaction*>& okReactions )
{
    okReactions.clear();

    BOOST_FOREACH( mzr::mzrReaction* rxnptr, reactions )
    {
        if ( reactionHasPositiveSubstrates( rxnptr ) )
        {
            okReactions.push_back( rxnptr );
        }
    }
}

bool SimpleStochasticSimulator::reactionHasPositiveSubstrates( const mzr::mzrReaction* rxnPtr )
{
    typedef std::pair<mzr::mzrSpecies*, int> PairType;
    BOOST_FOREACH( const PairType& pr, rxnPtr->getReactants() )
    {
        std::string substrateName( pr.first->getName() );

        if ( theModel.find( substrateName ) == theModel.end() )
        {
            return false;
        }

        if ( theModel[ substrateName ] < pr.second ) return false;
    }

    return true;

}

void SimpleStochasticSimulator::singleStep()
{
    mzr::mzrReaction* reaction = calculateReactionToFire();

    if ( reaction )
    {
        executeReaction( reaction );
    }

    recordNewReactions();

    return;
}

