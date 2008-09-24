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
//   Larry Lok, Research Fellow, Molecular Sciences Institute, 2001
//
// Modifing Authors:
//
//

#ifndef BASICREACTION_H
#define BASICREACTION_H

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <boost/foreach.hpp>

namespace fnd
{
template<class speciesType>
class basicReaction
{
public:

typedef typename std::map<speciesType*, int> multMap;

bool
hasReactant(speciesType* species)
{
return (reactants.find(species) != reactants.end() );
}

bool
hasProduct(speciesType* species)
{
return (products.find(species) != products.end());
}


int
getReactantStochiometry(speciesType* species)
{
if (!hasReactant(species)) return 0;
else
{
reactants[species];
}
}

const multMap&
getReactants() const
{
return reactants;
}

const multMap&
getProducts() const
{
return products;
}

bool
isStandardReaction() const
{
// This may be totally inappropriate here, but I anticipate there will only
// be a few reaction types

unsigned int reactantsSize = getReactants().size();
unsigned int productsSize = getProducts().size();

if (reactantsSize == 0)
{
// 0->1 is acceptable, everything else is not.
return (productsSize == 1);
}
else if (reactantsSize == 1)
{
// 1-> 0, 1, or 2
return (productsSize >= 0 && productsSize <= 2);
}
else if (reactantsSize == 2)
{
// 2->1 || 2->2
return (productsSize == 1 || productsSize == 2);
}
else
{
return false;
}
}

//         bool
//         isDecompositionReaction() const
//         {
//             return (getReactants().size() == 1 &&
//                     getProducts().size() == 2);
//         }

//         bool
//         isDimerizationReaction() const
//         {
//             return (getReactants().size() == 2 &&
//                     getProducts().size() == 1);
//         }


protected:

multMap reactants;
multMap products;
multMap deltas;

int arity;

double rate;

public:
int getNumberOfReactants() const
{
int sum = 0;

BOOST_FOREACH( typename multMap::value_type tt, reactants)
{
sum += tt.second;

}

return sum;
}

public:
basicReaction(double reactionRate = 0.0) :
arity(0),
rate(reactionRate),
nameUnclean( true ),
theName( "" )
{}

virtual ~basicReaction()
{}

void
addReactant(speciesType* pSpecies,
int multiplicity);

void
addProduct(speciesType* pSpecies,
int multiplicity);

double
getRate(void) const
{
return rate;
}

void
setRate(double newRate)
{
rate = newRate;
}

int
getArity(void) const
{
return arity;
}


std::string
getCanonicalName() const
{
if (nameUnclean) generateCanonicalName();
return theName;
}

void
generateCanonicalName() const;

virtual
std::string
getName() const
{
return getCanonicalName();
}


private:
// This is all naming stuff.  Students of history may find it notable that
// this is also the first time I've used the "mutable" keyword.

mutable bool nameUnclean;
mutable std::string theName;

};

// Note that this does nothing with regard to sensitization.  Sensitization
// of the reaction to the new substrate must be done in descendant reaction
// classes themselves, since those are the classes to which the species
// are sensitive.
template<class speciesType>
void
basicReaction<speciesType>::
addReactant(speciesType* pSpecies,
int multiplicity)
{
// Try to insert the new reactant species and its multiplicity into the
// reactant multiplicity map, under the assumption that the species is not
// already a reactant.
std::pair<typename multMap::iterator, bool> insertResult
= reactants.insert(std::pair<speciesType*, int>(pSpecies,
multiplicity));

// The insertion will fail if the species is already a reactant.
// If this is the case, then add to the reactant's multiplicity
// in the existing entry.
if(! insertResult.second)
{
insertResult.first->second += multiplicity;
}

// Try to insert the new reactant species and its (negative) delta
// in the delta multiplicity map, under the assumption that the species
// is neither a reactant nor a product.
insertResult
= deltas.insert(std::pair<speciesType*, int>(pSpecies,
- multiplicity));

// The insertion will fail if the species is already a reactant or a
// product.  If this is the case, then adjust the multiplicity in its
// existing entry.
if(! insertResult.second)
{
insertResult.first->second -= multiplicity;
}

// Add the reactant multiplicity to the arity.
arity += multiplicity;

// Adding a reactant means we will need to regenerate the name if anyone
// requests it.
nameUnclean = true;
}

template<class speciesType>
void
basicReaction<speciesType>::
addProduct(speciesType* pSpecies,
int multiplicity)
{
// Try to insert the new product species and its multiplicity into the
// product multiplicity map, under the assumption that the species is not
// already a product.
std::pair<typename multMap::iterator, bool> insertResult
= products.insert(std::pair<speciesType*, int>(pSpecies,
multiplicity));

// The insertion will fail if the species is already a product.
// If this is the case, then add to the product's multiplicity
// in the existing entry.
if(! insertResult.second)
{
insertResult.first->second += multiplicity;
}

// Try to insert the new product species and its (positive) delta
// in the delta multiplicity map, under the assumption that the species
// is neither a reactant nor a product.
insertResult
= deltas.insert(std::pair<speciesType*, int>(pSpecies,
multiplicity));

// The insertion will fail if the species is already a reactant or a
// product.  If this is the case, then adjust the multiplicity in its
// existing entry.
if(! insertResult.second)
{
insertResult.first->second += multiplicity;
}

// Adding a reactant means we will need to regenerate the name if anyone
// requests it.
nameUnclean = true;
}

template <typename speciesType>
class CompareSpeciesEntries
{
public:
int operator()( const std::pair<speciesType*, int>& numOne, const std::pair<speciesType*, int>& numTwo)
{
return numOne.first->getName() < numTwo.first->getName();
}
};

template <class speciesType>
void basicReaction<speciesType>::
generateCanonicalName() const
{

std::vector< std::pair<speciesType*, int> > theReactants( reactants.begin(),
reactants.end() );

std::vector< std::pair<speciesType*, int> > theProducts( products.begin(),
products.end() );

std::sort( theReactants.begin(),
theReactants.end(),
CompareSpeciesEntries<speciesType>()
);


std::sort( theProducts.begin(),
theProducts.end(),
CompareSpeciesEntries<speciesType>()
);

std::ostringstream reactionName;

reactionName << "(" << reactants.size() << ", " << products.size() <<  ") -- ";

// This is sort of goofy because of the following
// vector = first| a, b, c| last
// " a + b + c" => add a " + " iff ndx of what we've just added

for(unsigned int ii = 0;
ii != theReactants.size();
++ii)
{

if (theReactants[ii].second > 1)
{

reactionName << theReactants[ii].second
<< " "
<< theReactants[ii].first->getName();
}
else
{
reactionName << theReactants[ii].first->getName();
}

// I don't like this condition as it seems error-prone.
// However
// if size == 4 and
// NDX: 0  1  2  3
//      a  b  c  d
// We want to add pluses for everything up to and including 2
if (ii != theReactants.size() - 1)
{
reactionName << " + ";
}

}

reactionName << " -> ";

for(unsigned int ii = 0;
ii != theProducts.size();
++ii)
{
if (theProducts[ii].second > 1)
{
reactionName << theProducts[ii].second
<< " "
<< theProducts[ii].first->getName();
}
else
{
reactionName << theProducts[ii].first->getName();
}

if (ii != (theProducts.size() - 1) )
{
reactionName << " + ";
}

}

nameUnclean = false;
theName = reactionName.str();

}

}

#endif // BASICREACTION_H
