###############################################################################
# BNGMZRConverter - A utility program for converting bngl input files to mzr
#		    input files.
# Copyright (C) 2007, 2008, 2009 The Molecular Sciences Institute
#
# Moleculizer is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# Moleculizer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Moleculizer; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# Original Author:
#   Nathan Addy, Scientific Programmer	Email: addy@molsci.org
#   The Molecular Sciences Institute    Email: addy@molsci.org  
#                     
#   
###############################################################################

from moleculizermol import MoleculizerMol, MoleculizerSmallMol, MoleculizerModMol, isSmallMol, isModMol

from util import DataUnifier

from xmlobject import XmlObject
import pdb


# This class parses a Mols Block into a list of small-mols and big mols.
# It also manages the list of modifications.

class MoleculeDictionary:
    class DuplicateMolDefinitionException(Exception): pass
    class BadMolDefinitionException(Exception): pass

    listOfNullModifications = ["none"]

    def __init__(self, moleculeBlock, paramDict):
        self.rawMoleculeDefinitions = moleculeBlock[:]

        self.paramDict = paramDict

        self.registeredMoleculesDictionary = {}
        self.smallMolsDictionary = {}
        self.modMolsDictionary = {}

        self.initialize()

    def initialize(self):
        self.rawMoleculeDefinitions = DataUnifier( self.rawMoleculeDefinitions )

        for line in self.rawMoleculeDefinitions:
            if isSmallMol(line):
                print "SM: %s" % line
                MoleculizerSmallMol(line)
            elif isModMol(line):
                print "MM: %s" % line
                MoleculizerModMol(line)
            else:
                print "'%s' is neither a ModMol nor a SmallMol, according to the isSmallMol and isModMol functions." % line
                raise "Hello"
        

    def parseMoleculeTypesLine(self, moleculeTypesLine):

        parsedMol = MoleculizerMol(moleculeTypesLine)
        parsedMolName = parsedMol.getName()

        if parsedMolName in self.registeredMoleculesDictionary.keys():
            raise DuplicateMolDefinitionException("Error, molecule %s already defined in the MoleculeInterpreter" % parsedMolName)

        self.registeredMoleculesDictionary[parsedMolName] = parsedMol



    def getUniversalModificationList(self):
        return MoleculizerMol.modificationStates[:]


    def addModifications(self, parentElmt):
        for modification in self.getUniversalModificationList():
            modificationTypeElmt = XmlObject("modification")
            modificationTypeElmt.addAttribute("name", modification)
            modificationTypeElmt.attachToParent(parentElmt)            

            weightElmt = XmlObject("weight-delta")
            weightElmt.attachToParent(modificationTypeElmt)

            if self.representsNullModification(modification):
                weightDelta = 0.0
            else:
                weightDelta = 1.0

            weightElmt.addAttribute("daltons", weightDelta)

    def addMols(self, parentElmt):
        for molName in self.registeredMoleculesDictionary.keys():
            self.addModMolElmtToMolsElmt(parentElmt, self.registeredMoleculesDictionary[molName])            

    def addModMolElmt(self, parentElmt):
        pass


    def addModMolElmtToMolsElmt(self, xmlObject, moleculizerMolObject):
        assert(isinstance(xmlObject, XmlObject))
        assert(isinstance(moleculizerMolObject, MoleculizerMol))

        newModMol = XmlObject("mod-mol")
        newModMol.addAttribute("name", moleculizerMolObject.getName())

        weightElement = XmlObject("weight")

        # Obviously this is one of the big deficiencies of this thing.  What shall
        # we set the (mandatory) weights to?  For now, let's just put in something
        # arbitratry.  But this is a big issue that ought to be fixed as soon as all
        # the basic facilities of the code have been built in.

        if moleculizerMolObject.getName() == "Pheromone":
            weightElement.addAttribute("daltons", 10.0)
        else:
            weightElement.addAttribute("daltons", 100.0)            

        newModMol.addSubElement(weightElement)

        for binding in moleculizerMolObject.bindingSites:
            self.addBindingSiteElmtToModMolElmt(binding, moleculizerMolObject, newModMol)

        for modification in moleculizerMolObject.modificationSites:
            modSite, defaultModState = modification
            modSiteElmt = XmlObject("mod-site")
            modSiteElmt.addAttribute("name", modSite)
            defModRefElmt = XmlObject("default-mod-ref")
            defModRefElmt.addAttribute("name", defaultModState)
            defModRefElmt.attachToParent(modSiteElmt).attachToParent(newModMol)

        xmlObject.addSubElement(newModMol)
        
        return

    def addBindingSiteElmtToModMolElmt(self, bindingName, moleculizerMol, xmlObject):

        newBindingElmt = XmlObject("binding-site")
        newBindingElmt.addAttribute("name", bindingName)
        
        defaultShape = XmlObject("default-shape-ref")
        defaultShape.addAttribute("name", "default")
        defaultShape.attachToParent(newBindingElmt)

        for shapeName in moleculizerMol.bindingSites[bindingName]:
            siteShapeElmt = XmlObject("site-shape")
            siteShapeElmt.addAttribute("name", shapeName)
            siteShapeElmt.attachToParent(newBindingElmt)

        xmlObject.addSubElement(newBindingElmt)
        return

    def representsNullModification(self, modificationType):
        return modificationType.lower() in MoleculeDictionary.listOfNullModifications
