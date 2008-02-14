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

#ifndef __ABSTRACTMOL_HH
#define __ABSTRACTMOL_HH

#include <string>
#include <vector>
#include <list>
#include <utility>

#include "csException.hh"

namespace complexspecies
{

    class AbstractMol
    {

    public:

        typedef std::string MolType;
        typedef std::string BindingSite;
        typedef std::string ModificationSite;
        typedef std::string ModificationValue;
        typedef std::vector<ModificationValue> ListOfModificationValues;

    public:

        AbstractMol(MolType aMolType) 
            : 
            theMolType(aMolType) 
        {
            ; // do nothing
        }
        virtual ~AbstractMol()
        {
            ; // do nothing
        }

        MolType getMolType() const
        { 
            return theMolType; 
        }

        virtual bool checkIfBindingSiteExists(BindingSite aBindingSite) const =0;
        virtual bool checkIfModificationSiteExists(ModificationSite aModificationSite) const =0;

        virtual bool checkIfBindingSiteIsBound(BindingSite aBindingSite) const =0;
        virtual ModificationValue getModificationValueAtModificationSite(ModificationSite aModificationSite) const  =0;

        virtual void bindAtBindingSite(BindingSite aBindingSite) =0;
        virtual void unbindAtBindingSite(BindingSite aBindingSite) =0;
        virtual void updateModificationState(ModificationSite aModificationSite,
                                             ModificationValue aModificationValue) =0;

        virtual int getBindingSiteInteger(BindingSite aBindingSite) const =0;
        virtual int getModificationSiteInteger(ModificationSite aModificationSite) const =0;

        virtual bool operator<(const AbstractMol& rhsAbstractMol) const
        {
            return (this->getMolType() < rhsAbstractMol.getMolType());
        }

    protected:

        MolType theMolType;
    
    };

}

#endif 
