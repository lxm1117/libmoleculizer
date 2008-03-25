/////////////////////////////////////////////////////////////////////////////
// Moleculizer - a stochastic simulator for cellular chemistry.
// Copyright (C) 2001, 2008  Walter Lawrence (Larry) Lok.
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
//   Larry Lok, Research Fellow          Voice: 510-981-8740
//   The Molecular Sciences Institute      Fax: 510-647-0699
//   2168 Shattuck Ave.                  Email: lok@molsci.org
//   Berkeley, CA 94704
/////////////////////////////////////////////////////////////////////////////

#ifndef CPX_ALLOMOLIMPL_H
#define CPX_ALLOMOLIMPL_H

#include <sstream>

namespace cpx
{
  template<class stateMolT>
  const typename alloMol<stateMolT>::stateType*
  alloMol<stateMolT>::
  internState(const typename alloMol<stateMolT>::stateType& rStateToIntern)
  {
    // Has this state previously been interned?
    alloIterator iEntry = alloMap.find(rStateToIntern);

    // If not, intern it with default site shapes.
    if(alloMap.end() == iEntry)
      {
	// Insert the state into the alloMap.
	iEntry = alloMap.insert(alloMapValue(rStateToIntern,
					     this->getDefaultSiteParams())).first;
      }

    return &(iEntry->first);
  }
  
  class badMolParamClassXcpt : 
    public utl::xcpt
  {
    static std::string
    mkMsg(const std::string& rMolName)
  {
    std::ostringstream msgStream;
    msgStream << utl::xcpt::mkMsg()
	      << "State pointer was of the wrong class for mol "
	      << rMolName
	      << ".";
    return msgStream.str();
  }
  public:
    badMolParamClassXcpt(const std::string& rMolName) :
      utl::xcpt(mkMsg(rMolName))
    {}
  };

  class uninternedStateXcpt :
    public utl::xcpt
  {
    static std::string
    mkMsg(const std::string& rMolName)
    {
      std::ostringstream msgStream;
      msgStream << utl::xcpt::mkMsg()
		<< "State was not previously interned in mol "
		<< rMolName
		<< ".";
      return msgStream.str();
    }
  public:
    uninternedStateXcpt(const std::string& rMolName) :
      utl::xcpt(mkMsg(rMolName))
      {}
  };

  template<class stateMolT>
  typename alloMol<stateMolT>::constAlloIterator
  alloMol<stateMolT>::
  externalize(const molState* pStateToExternalize) const
    throw(typename utl::xcpt)
  {
    const typename alloMol::stateType* pOurState 
      = dynamic_cast<const typename alloMol::stateType*>(pStateToExternalize);

    // Check that the generic state pointer was really a pointer to
    // our state type.
    if(! pOurState) 
      throw badMolParamClassXcpt(this->getName());

    // Check that the state is in the allostery map, verifying that 
    // pStateToExtern was produced by internState.
    typename alloMol::constAlloIterator iEntry = alloMap.find(*pOurState);
    if((alloMap.end() == iEntry)
       || pOurState != &(iEntry->first))
      throw uninternedStateXcpt(this->getName());

    return iEntry;
  }

  template<class stateMolT>
  typename alloMol<stateMolT>::alloIterator
  alloMol<stateMolT>::
  externalize(const molState* pStateToExternalize)
    throw(typename utl::xcpt)
  {
    const typename alloMol::stateType* pOurState 
      = dynamic_cast<const typename alloMol::stateType*>(pStateToExternalize);

    // Check that the generic state pointer was really a pointer to
    // our state type.
    if(! pOurState) 
      throw badMolParamClassXcpt(this->getName());

    // Check that the state is in the allostery map, verifying that 
    // pStateToExtern was produced by internState.
    typename alloMol::alloIterator iEntry = alloMap.find(*pOurState);
    if((alloMap.end() == iEntry)
       || pOurState != &(iEntry->first))
      throw uninternedStateXcpt(this->getName());

    return iEntry;
  }
}

#endif // CPX_ALLOMOLIMPL_H
