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

#include "fnd/sensitivityList.hh"
#include "mzr/mzrReaction.hh"
#include "mzr/respondReaction.hh"
#include "mzr/mzrUnit.hh"
#include "mzr/volumeEvent.hh"

namespace mzr
{
volumeEvent::
volumeEvent(mzrUnit& refMzrUnit,
double targetVolume) :
rMzrUnit(refMzrUnit),
vol(targetVolume)
{}

fnd::eventResult
volumeEvent::
happen(moleculizer& rMolzer)
throw(std::exception)
{
fnd::sensitivityList<mzrReaction> affectedReactions;

// Reset the volume.
rMzrUnit.getMolarFactor().updateVolume(vol,
affectedReactions);

// Reschedule everybody.  This invocation stinks to high
// heaven of confused reference structure in this program.
// for_each(affectedReactions.begin(),
// 	     affectedReactions.end(),
// 	     respondReaction(rMolzer));

return fnd::go;
}
}
