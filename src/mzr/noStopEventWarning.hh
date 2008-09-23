/////////////////////////////////////////////////////////////////////////////
// Moleculizer - a stochastic simulator for cellular chemistry.
// Copyright (C) 2001, 2008 The Molecular Sciences Institute.
//
// Moleculizer is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// Moleculizer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//    
// Contact information:
//   Larry Lok, Research Fellow          Voice: 510-981-8740
//   The Molecular Sciences Institute      Fax: 510-647-0699
//   2168 Shattuck Ave.                  Email: lok@molsci.org
//   Berkeley, CA 94704
/////////////////////////////////////////////////////////////////////////////

#ifndef MZR_NOSTOPEVENTWARNING_H
#define MZR_NOSTOPEVENTWARNING_H

#include "utl/dom.hh"

namespace mzr
{
  // Decided not to use the special utl::warning class; rather enhanced
  // utl::xcpt to issue its message as a warning, if desired.
  class noStopEventWarning : 
    public utl::xcpt
  {
    static std::string
    mkMsg(xmlpp::Node* pOffendingNode = 0);
    
  public:
    noStopEventWarning(xmlpp::Node* pEventsElement = 0) :
      utl::xcpt(mkMsg(pEventsElement))
    {}
  };
}

#endif // MZR_NOSTOPEVENTWARNING_H
