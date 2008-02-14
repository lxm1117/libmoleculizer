/////////////////////////////////////////////////////////////////////////////
// libComplexSpecies - a library for canonically naming species of protein 
//                     complexes.
// Copyright (C) 2007  Nathan Addy
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
// Contact information:
//   Nathan Addy, Research Associate     Voice: 510-981-8748
//   The Molecular Sciences Institute    Email: addy@molsci.org  
//   2168 Shattuck Ave.                  
//   Berkeley, CA 94704
/////////////////////////////////////////////////////////////////////////////

#include <iterator>
#include <set>
#include <vector>

namespace complexspecies
{

    namespace detail
    {

        template <class molT>
        ComplexSpeciesOutputMinimizer<molT>::ComplexSpeciesOutputMinimizer()
        {
            ; // do nothing
        }

        template <class molT>
        ComplexOutputState ComplexSpeciesOutputMinimizer<molT>::getMinimalOutputState(complexspecies::ComplexSpecies<molT> aComplexSpecies)
        {
            this->theUnnamedComplex = aComplexSpecies;

            setupDataStructuresForCalculation();
            Permutation theUnnamedComplexMinimalOutputStatePermutation = calculateMinimizingPermutationForComplex(aComplexSpecies);
      
            theUnnamedComplex.applyPermutationToComplex(theUnnamedComplexMinimalOutputStatePermutation);
            ComplexOutputState aComplexSpeciesCanonicalState;
            theUnnamedComplex.constructOutputState(aComplexSpeciesCanonicalState);
            return aComplexSpeciesCanonicalState;
        }


        template <class molT>
        Permutation ComplexSpeciesOutputMinimizer<molT>::calculateMinimizingPermutationForComplex(ComplexSpecies<molT> aComplexSpecies)
        {
   
            if( !checkPlexIsSorted(theUnnamedComplex) )
            {
                throw complexspecies::CSXcpt("ComplexSpeciesOutputMinimizer<molT>::calculateMinimizingPermutationForComplex()", "ComplexSpecies theComplexSpecies has not been sorted.");
            }
  
            std::set<Permutation> setOfPossibleSolutions; // This is a set of partial permutations, which represent, all possible permutations (each incomplete pp represents a particular coset of permutations)
            std::vector<PermutationName<molT> > theNextIterationsPartialPermutations; //to be used for holding permutations which will be in the next partialPermiter set in the next round.      

            setOfPossibleSolutions.insert( Permutation( theUnnamedComplex.getNumberOfMolsInComplex()) );

            //Basic idea of this algorithm as follows:
            //
            //1.  For each permutation in setOfPossibleSolutions, figure out the least int not in that permutation.
            //2.  That int will be a associated with the Mol that will ultimatly map to it.  Use this information to find all the indexes that can map to that least value.
            //3.  Now prune the index list above (we are trying to find the smallest set of indexes that can extend the permutation in 1.) by only keeping indexes which correspond to a mol  
            //3.  that has the lowest binding amonst all the corresponding mols to the cumulative indexes.
            //3.5 Now generate all the permutations that come from combining the permutation in question with each one of it's possible continuations.
            //3.7 For each one of these new permutations, extend it maximally.
            //4.  Now generate the name corresponding to each permutation.  Find the least and throw out any permutation whose name isn't one of the least.  
            //5.  Repeat this process until there is no permutation in the set of permutations that has any negative entry.



            // 5. Repeat this process until there is no permutation in the set of permutations that has any negative entry.
            while( checkExistsIncompletePermutations(setOfPossibleSolutions) ) 
            {

                theNextIterationsPartialPermutations.clear();

                //For each partial permutation, figure out how to automatically extend it into the next round (see NOTES for information on moleculizer-specific
	  

                for(ConstPermSetIter partialPermIter = setOfPossibleSolutions.begin();
                    partialPermIter != setOfPossibleSolutions.end();
                    ++partialPermIter)
                {   
                    //For each permutation in setOfPossibleSolutions, determine if it is complete or not.
                    //If it is not, find all continuations, construct and extend each one, and add it to theNextIterationsPartialPermutations;
                    //If it is, just add it as is to the partials of this round.  

                    if ( !partialPermIter->getIsComplete() )
                    {
                        //1. Find the most significant int that hasn't been fixed already.
                        int leastIntNotInPartial = partialPermIter->getLeastValueNotInPermutation();
		
                        //2. Find all the free mols of the associated type and put them in validIndexes
                        std::string molTypeToBeAdded = indexToMolMap[leastIntNotInPartial];
                        Range molRange = molToIndexRangeMap[molTypeToBeAdded];

                        std::vector<int> validIndexes;  
                        for(int i = molRange.first; i != molRange.second; ++i)
                        {
                            if ( partialPermIter->getValueAtPositionXcpt(i) == UNDEF )
                            {
                                validIndexes.push_back(i);
                            }
                        }

                        //3.5
                        for(std::vector<int>::iterator i = validIndexes.begin();
                            i!=validIndexes.end();
                            ++i)
                        {
                            PermutationName<molT> tmpPn;
                            Permutation tmpPermutation(*partialPermIter, *i, leastIntNotInPartial);
                            tmpPn.thePermutation = tmpPermutation;

                            //3.7
                            maximallyExtendPermutation(tmpPn.thePermutation);
                            theNextIterationsPartialPermutations.push_back(tmpPn);

                        }
                    }
                    else
                    {
                        PermutationName<molT> tmp;
                        tmp.thePermutation = *partialPermIter;
                        theNextIterationsPartialPermutations.push_back(tmp);
                    }

                } //ends for partialPermIter loop, The creation of new partials for the round is complete


                //We begin the processing of the new partials by clearing setOfPossibleSolutions
                setOfPossibleSolutions.clear();
      
                //Generate the name for each Permutation
                for(typename std::vector<PermutationName<molT> >::iterator iter = theNextIterationsPartialPermutations.begin();
                    iter != theNextIterationsPartialPermutations.end();
                    ++iter)
                {
                    iter->theCorrespondingPartialTokenList = calculatePartialTokenListForPermutation(theUnnamedComplex, iter->thePermutation);
                }

                std::sort(theNextIterationsPartialPermutations.begin(),
                          theNextIterationsPartialPermutations.end());

                PartialTokenList<molT> leastPartial = theNextIterationsPartialPermutations.begin()->theCorrespondingPartialTokenList;
                for(typename std::vector<PermutationName<molT> >::iterator i = theNextIterationsPartialPermutations.begin();
                    i != theNextIterationsPartialPermutations.end();
                    ++i)
                {
                    if (i->theCorrespondingPartialTokenList == leastPartial)
                    {
                        setOfPossibleSolutions.insert( i->thePermutation );
                    }
                    else
                    {
                        break;
                    }
                }
            } //end while loop 

            //So now all permutations are complete and all produce equal names.  So we can just return the first one.
            return *(setOfPossibleSolutions.begin());

        }


        template <class molT>
        void ComplexSpeciesOutputMinimizer<molT>::setupDataStructuresForCalculation()
        {
            detail::Permutation aMolSortingPermutation = getPlexSortingPermutation(theUnnamedComplex);
            theUnnamedComplex.applyPermutationToComplex(aMolSortingPermutation);

            indexToMolMap.clear();
            molToIndexRangeMap.clear();

            //Initialize the indexToMolMap.
            std::vector<std::string> sortedMolNames;
            typename ComplexSpecies<molT>::MolList aMolList=theUnnamedComplex.getMolList();

            for(unsigned int i = 0; 
                i!=aMolList.size(); 
                ++i)
            {
                sortedMolNames.push_back( aMolList[i].getMolType() );
                indexToMolMap.insert(std::make_pair(i, aMolList[i].getMolType()));
            }


            for(std::vector<std::string>::iterator i = sortedMolNames.begin(); 
                i != sortedMolNames.end(); 
                ++i)
            {
                std::string name=*i;
                int first=0;
                int last;
 
                //Properly set the first index.
                while( !((first==0 && (sortedMolNames[first]==name)) || (first!=0 && sortedMolNames[first-1]!=name && sortedMolNames[first]==name)))
                {
                    first++;
                }

                //Now set the last index.
                last=first+1;
                while(!(last==static_cast<int>(sortedMolNames.size()) || (sortedMolNames[last]!=sortedMolNames[last-1])))
                {
                    last++;
                }
      
                molToIndexRangeMap.insert( std::make_pair( name, std::make_pair(first,last)));
            }

        }


        template <class molT>
        void ComplexSpeciesOutputMinimizer<molT>::maximallyExtendPermutation(Permutation& refPermToExtend)
        {
            //TODO I think this might not be doing what it ought to be doing, so for now it does nothing.
            //TODO Fix this as the first order of buisiness.

            //     Permutation inversePerm = refPermToExtend.invert();

            //     for(int indexToExtend = 0;
            // 	indexToExtend != static_cast<int>(refPermToExtend.size());
            // 	++indexToExtend)
            //       {
            // 	//If indexToExtend is mapped somewhere, it will have a preimage of inversePerm[indexToExtend]

            // 	int preimage = inversePerm[indexToExtend];

            // 	if(preimage == -1)
            // 	  {
            // 	    // This means that no Mol is currently mapped by refPermToExtend to indexToExtend
            // 	    break;
            // 	  }
            // 	else
            // 	  {
            // 	    // Mol with index preimage is mapped to indexToExtend by refPermToExtend

            // 	    //So now find all indexes involved in binding to preimage.  
            // 	    std::vector<Binding> allBindingsWithPreimage;
            // 	    for(std::vector<Binding>::iterator i = theUnnamedComplex.theBindings.begin();
            // 		i != theUnnamedComplex.theBindings.end();
            // 		++i)
            // 	      {
            // 		if ((*i).first.first == preimage || (*i).second.first == preimage)
            // 		  {
            // 		    allBindingsWithPreimage.push_back(*i);
            // 		  }
            // 	      }
	  
            // 	    //sort vector<Binding> by the binding number of the binding which has number
            // 	    //preimage
            // 	    //for each in order, check it out and if the other half hasn't been assigned to
            // 	    //assign it the best position possible.  
            // 	    sort(allBindingsWithPreimage.begin(),
            // 		 allBindingsWithPreimage.end(),
            // 		 namerBindingCmp(preimage));

            // 	    //for each binding in order, find the index that isn't the preimage.  Find out if that 
            // 	    //index is 
	  
            // 	    for(std::vector<Binding>::iterator i = allBindingsWithPreimage.begin();
            // 		i != allBindingsWithPreimage.end();
            // 		++i)
            // 	      {
            // 		//get the index which isn't the preimage
            // 		int otherIndex;
            // 		if (i->first.first == preimage)
            // 		  {
            // 		    otherIndex = i->second.first;
            // 		  }
            // 		else 
            // 		  {
            // 		    otherIndex = i->first.first;
            // 		  }

            // 		int otherIndexUnderMapping = refPermToExtend[otherIndex];

            // 		if (otherIndexUnderMapping == -1)
            // 		  {
            // 		    //in this case, we can assign this value by calculation, done here
            // 		    //Do this by finding the molType of otherIndex, finding the range of molType
            // 		    //and then finding the smallest member of that range that is not mapped to 
            // 		    //(such that inversePerm[x]==-1)
            // 		    //Then we should add otherIndex->x as the proper continuation.  

            // 		    Range molRange = molToIndexRangeMap[indexToMolMap[otherIndex]];
            // 		    for(int x = molRange.first; x != molRange.second; ++x)
            // 		      {
            // 			if (inversePerm[x] == -1)
            // 			  {
            // 			    refPermToExtend.setValueAtPosition(otherIndex, x);
            // 			    inversePerm[x] = otherIndex;
            // 			    break;
            // 			  }
            // 		      }
		  
            // 		  }
            // 	      }
            // 	  }

            //       }
  
            return;
        }



        template <class molT>
        PartialTokenList<molT> ComplexSpeciesOutputMinimizer<molT>::calculatePartialTokenListForPermutation(ComplexSpecies<molT>& aComplexSpecies, Permutation& aPerm)
        {

            PartialTokenList<molT> aComplexSpeciesPartialTokenList;
            aComplexSpeciesPartialTokenList.isComplete = true;

            // The mols are already sorted in an abstractPlex, so we can just copy those into aComplexSpeciesPartialTokenList.theMols.
            typename complexspecies::ComplexSpecies<molT>::MolList aComplexSpeciesMolList = aComplexSpecies.getMolList();

            copy(aComplexSpeciesMolList.begin(),
                 aComplexSpeciesMolList.end(),
                 back_inserter(aComplexSpeciesPartialTokenList.theMols));


            std::vector<Binding> aComplexSpeciesPartialTokenListBindings;

            typedef int MolNdx;
            typedef std::string BindingSite;
            typedef int BndNdx;
            typedef std::pair<std::pair<MolNdx, BndNdx>, std::pair<MolNdx, BndNdx> > Binding;
            typedef std::vector<Binding> BindingList;
      
            const BindingList& theBindings= aComplexSpecies.getBindingList();

            for(BindingList::const_iterator i = theBindings.begin();
                i != theBindings.end();
                ++i)
            {
                int f_index, f_bndNum, s_index, s_bndNum;
                std::string f_bndState, s_bndState;
      
                f_index = (*i).first.first;
                f_bndNum = (*i).first.second;
      
                s_index = (*i).second.first;
                s_bndNum = (*i).second.second;


                f_index = aPerm.getValueAtPositionXcpt(f_index);
                s_index = aPerm.getValueAtPositionXcpt(s_index);
                if (!(f_index == -1) && !(s_index == -1))
                {
                    //then this entire binding is valid.
                    Binding tmpBinding;
                    if(f_index < s_index)
                    {
                        tmpBinding.first.first = f_index;
                        tmpBinding.first.second = f_bndNum;


                        tmpBinding.second.first = s_index;
                        tmpBinding.second.second = s_bndNum;

                    }
                    else
                    {
                        tmpBinding.first.first = s_index;
                        tmpBinding.first.second = s_bndNum;

                        tmpBinding.second.first = f_index;
                        tmpBinding.second.second = f_bndNum;

                    }

                    aComplexSpeciesPartialTokenListBindings.push_back(tmpBinding);
                }
                else 
                {
                    aComplexSpeciesPartialTokenList.isComplete = false; 
                }

            }

            std::sort(aComplexSpeciesPartialTokenListBindings.begin(),
                      aComplexSpeciesPartialTokenListBindings.end());
  
            std::copy(aComplexSpeciesPartialTokenListBindings.begin(),
                      aComplexSpeciesPartialTokenListBindings.end(),
                      std::back_inserter(aComplexSpeciesPartialTokenList.theBindings));

            if (!aComplexSpeciesPartialTokenList.isComplete) 
            {
                return aComplexSpeciesPartialTokenList;
            }

            // I'm  not sure that this is stricly necessary, but at this moment, I'm worrying that it might be possible for an incomplete permutation to perfectly 
            // describe all the bindings and yet not describe all the modifications.  So, playing it safe, this may be more complicated than it needs to be.

    
            std::vector<std::pair<int, std::pair<std::string, std::string> > > tmpModList;
            detail::Permutation inversePerm = aPerm.invertPermutation();



            const typename ComplexSpecies<molT>::MolList& theMols = theUnnamedComplex.getMolList();
            for(unsigned int molNdx = 0; 
                molNdx != theMols.size(); 
                ++molNdx)
            {
                typename Mol::ModificationList currentModNdxMolList = theMols[molNdx].getModificationList();

                for(typename Mol::ModificationList::iterator i = currentModNdxMolList.begin();
                    i != currentModNdxMolList.end();
                    ++i)
                {
                    std::pair<int, std::pair<std::string, std::string> > unambiguousModification;
                    unambiguousModification.first = inversePerm[molNdx];
                    unambiguousModification.second = *i;
                    tmpModList.push_back(unambiguousModification);
                }
	    
            }

            std::sort(tmpModList.begin(),
                      tmpModList.end());
            aComplexSpeciesPartialTokenList.theModifications.swap(tmpModList);

            return aComplexSpeciesPartialTokenList;
        }








































































        template <class molT>
        Permutation ComplexSpeciesOutputMinimizer<molT>::getPlexSortingPermutation(ComplexSpecies<molT> aComplexSpecies)
        {
            //initialize a vector with entries 0...size-1
            std::vector<int> permutationVect;
            for(int i=0; i != aComplexSpecies.getNumberOfMolsInComplex(); ++i)
            {
                permutationVect.push_back(i);
            }

            //Sort permutation using a function which compares the theMols belonging at that index.
            std::sort(permutationVect.begin(),
                      permutationVect.end(),
                      MolIndexLessThanCmp(aComplexSpecies));

            Permutation inversePerm = Permutation(permutationVect);
            Permutation forwardPerm = inversePerm.invertPermutation();
            return forwardPerm;
        }

        template <class molT>
        bool ComplexSpeciesOutputMinimizer<molT>::checkExistsIncompletePermutations( std::set<Permutation>& setOfPPs)
        {
            //Loop over each element of the set.  If any member is Incomplete, return true; otherwise, return false.  
            std::set<Permutation>::const_iterator i;
            for(i = setOfPPs.begin(); i != setOfPPs.end(); ++i)
            {
                if (i->getIsIncomplete())
                {
                    return true;
                }
            }
            return false;
        }




        template <class molT>
        bool ComplexSpeciesOutputMinimizer<molT>::checkPlexIsSorted(const ComplexSpecies<molT>& aComplexSpecies) const
        {

            if (theUnnamedComplex.getNumberOfMolsInComplex() ==0)
            {
                std::cout<<"Error, unnamed plex has not been initialized"<<std::endl;
            }

            const std::vector<molT>& aMolList = aComplexSpecies.getMolList();

            typename std::vector<molT>::const_iterator iter, iterPlusOne;
      
            if (aMolList.size()==1)
            {
                return true;
            }



            for(iter = aMolList.begin(), iterPlusOne = iter+1;
                iterPlusOne != aMolList.end();
                ++iter, ++iterPlusOne)
            {
                if ((iter->getMolType())>(iterPlusOne->getMolType()))
                {
                    return false;
                }
            }
            return true;
        }

    }
}

