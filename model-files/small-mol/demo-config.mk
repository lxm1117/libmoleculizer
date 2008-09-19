###############################################################################
# Moleculizer - a stochastic simulator for cellular chemistry.
# Copyright (C) 2001  Walter Lawrence (Larry) Lok.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#    
# Contact information:
#   Larry Lok, Research Fellow          Voice: 510-981-8740
#   The Molecular Sciences Institute      Fax: 510-647-0699
#   2168 Shattuck Ave.                  Email: lok@molsci.org
#   Berkeley, CA 94704
###############################################################################

DEMO_NAME := small-mol

DEMO_DIR := $(DEMO)/$(DEMO_NAME)

INPUT_FILE := $(DEMO_DIR)/$(DEMO_NAME).xml

OUTPUT_DIR := $(DEMO_DIR)/$(DEMO_NAME).out

# Run the simulation, convert the generated state dump into sbml, and
# pretty-print (indent) the sbml.
$(DEMO_DIR)/target : $(OUTPUT_DIR)/state-dump.sbml \
	$(OUTPUT_DIR)/state-dump-lines.sbml \

$(OUTPUT_DIR) :
	mkdir $@

# Run the moleculizer simulation, producing a state dump.
$(OUTPUT_DIR)/simulation-done : $(INPUT_FILE) | $(OUTPUT_DIR)
	echo "Started at" `date` > $@
	cp $< $(@D)
	cd $(@D) \
	&& moleculizer < $(<F) \
	&& plot-dmp-files
	echo "Finished at" `date` >> $@

# Translate the state dump into sbml.
$(OUTPUT_DIR)/state-dump.sbml : $(OUTPUT_DIR)/simulation-done
	cd $(@D) \
	&& state2sbml state-dump.xml $(@F)

# Pretty-print (indent) the sbml translation of the state dump.
$(OUTPUT_DIR)/state-dump-lines.sbml : $(OUTPUT_DIR)/state-dump.sbml
	cd $(@D) \
	&& xmlpretty $(<F) $(@F)

# For running the demo repeatedly.
$(DEMO_DIR)/clean : OD := $(OUTPUT_DIR)
$(DEMO_DIR)/clean :
	rm -rf $(OD)

CLEAN_LIST += $(OUTPUT_DIR)

PREEN_LIST += $(DEMO_DIR)/*~ $(DEMO_DIR)/*.bak
