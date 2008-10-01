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

#ifndef UTL_FREXP10_H
#define UTL_FREXP10_H

namespace utl
{
    // For "roll your own" conversion of double to scientific notation, so that
    // it can be formatted in mathML.  The calling sequence is modeled on the
    // library routind frexp and its friends.
    //
    // The frexp routine does this exaclty right in base 2, and this function is
    // a wrapper for it.  The scientific notation generated by this is not
    // precisely correct, in that the fractional part may not of exactly the
    // right magnitude, but it should be numerically correct anyway.  (The
    // fractional part is derived from the base 2 fractional part without making
    // a detailed correction of its magnitude.)
    double
    frexp10 (double num,
             int& rExponent);
}

#endif // UTL_FREXP10_H
