## Process this file with automake to produce Makefile.in.

noinst_LTLIBRARIES = libmoleculizer_cpx.la

VERSION = @INSTALLATION_VERSION@
MOLECULIZER_DIRNAME = libmoleculizer-$(VERSION)
libmoleculizer_cpxdir=$(includedir)/$(MOLECULIZER_DIRNAME)/libmoleculizer/cpx

AM_CXXFLAGS = -Wall 

libmoleculizer_cpx_la_SOURCES =\
basicBndSite.cc \
cpxXcpt.cc \
modMolMixin.cc \
modStateMixin.cc \
plexIso.cc \
plexMap.cc \
siteToShapeMap.cc

libmoleculizer_cpx_HEADERS=\
alloMol.hh \
alloMolImpl.hh \
basicBndSite.hh \
basicMol.hh \
basicMolImpl.hh \
basicPlex.hh \
basicPlexImpl.hh \
binding.hh \
cpxXcpt.hh \
cxBinding.hh \
cxBindingImpl.hh \
cxMol.hh \
cxMolImpl.hh \
cxOmni.hh \
cxOmniImpl.hh \
cxSite.hh \
cxSiteImpl.hh \
ftrSpec.hh \
hashMolRec.hh \
hashMolRecImpl.hh \
isoSearch.hh \
isoSearchImpl.hh \
knownBindings.hh \
modMixinQuery.hh \
modMol.hh \
modMolImpl.hh \
modMolMixin.hh \
modMolState.hh \
modMolStateQuery.hh \
modStateMixin.hh \
modification.hh \
molState.hh \
molStateQuery.hh \
omniPlex.hh \
omniPlexFeature.hh \
omniStructureQuery.hh \
omniStructureQueryImpl.hh \
plexFamily.hh \
plexFamilyImpl.hh \
plexIso.hh \
plexIsoImpl.hh \
plexMap.hh \
plexMapImpl.hh \
plexQuery.hh \
plexQueryImpl.hh \
plexSpcsMixin.hh \
plexSpcsMixinCanonicalNamingImpl.hh \
plexSpcsMixinImpl.hh \
prm.hh \
queryAlloList.hh \
queryAlloListImpl.hh \
recognizer.hh \
recognizerImpl.hh \
reportIsoSearch.hh \
siteShape.hh \
siteToShapeMap.hh \
siteToShapeMapImpl.hh \
smallMol.hh \
stateMol.hh \
structuralBinding.hh \
subPlexSpec.hh

