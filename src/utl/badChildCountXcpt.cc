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

#include <sstream>
#include "utl/badChildCountXcpt.hh"

namespace utl
{
    namespace dom
    {
        std::string
        badChildCountXcpt::
        mkGeneralMsg (const xmlpp::Node* pParentNode,
                      const std::string& rChildName,
                      int requiredCount,
                      int actualCount)
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg (pParentNode)
            << "Expected "
            << requiredCount
            << " "
            << rChildName
            << " elements in "
            << pParentNode->get_name()
            << " element; got "
            << actualCount
            << ".";
            return msgStream.str();
        }

        std::string
        badChildCountXcpt::
        mkChoiceMsg (const xmlpp::Node* pParentNode,
                     int actualCount)
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg (pParentNode)
            << "Expected one child of "
            << pParentNode->get_name()
            << " node among alternatives "
            << "offered by choice, got "
            << actualCount
            << ".";
            return msgStream.str();
        }

        std::string
        badChildCountXcpt::
        mkOneOrMoreMsg (const xmlpp::Node* pParentNode)
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg (pParentNode)
            << "Expected one or more children of "
            << pParentNode->get_name()
            << " node among alternatives, got none.";
            return msgStream.str();
        }

// In support of RNG's "optional" construct.
        std::string
        badChildCountXcpt::
        mkZeroOrOneMsg (const xmlpp::Node* pParentNode,
                        const std::string& rChildName,
                        int actualCount)
        {
            std::ostringstream msgStream;
            msgStream << xcpt::mkMsg (pParentNode)
            << "Expected zero or one "
            << rChildName
            << " elements in "
            << pParentNode->get_name()
            << " element; got "
            << actualCount
            << ".";
            return msgStream.str();
        }

        badChildCountXcpt::
        badChildCountXcpt (const std::string& rMsg) :
                xcpt (rMsg)
        {}

        badChildCountXcpt
        badChildCountXcpt::
        general (const xmlpp::Node* pParentNode,
                 const std::string& rChildName,
                 int requiredCount,
                 int actualCount)
        throw()
        {
            return badChildCountXcpt (mkGeneralMsg (pParentNode,
                                                    rChildName,
                                                    requiredCount,
                                                    actualCount) );
        }

        badChildCountXcpt
        badChildCountXcpt::
        choice (const xmlpp::Node* pParentNode,
                int actualCount)
        throw()
        {
            return badChildCountXcpt (mkChoiceMsg (pParentNode,
                                                   actualCount) );
        }

        badChildCountXcpt
        badChildCountXcpt::
        oneOrMore (const xmlpp::Node* pParentNode)
        throw()
        {
            return badChildCountXcpt (mkOneOrMoreMsg (pParentNode) );
        }

        badChildCountXcpt
        badChildCountXcpt::
        zeroOrOne (const xmlpp::Node* pParentNode,
                   const std::string& rChildName,
                   int actualCount)
        throw()
        {
            return badChildCountXcpt (mkZeroOrOneMsg (pParentNode,
                                      rChildName,
                                      actualCount) );
        }
    }
}
