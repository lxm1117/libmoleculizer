#include <string>

namespace mzr
{

  class PythonRulesManager
  {
  public:
    PythonRulesManager();

    std::string getXmlString() const;

    void addParameterStatement(const std::string& paramLine);
    void addModificationStatement(const std::string& modLine);
    void addMolsStatement(const std::string& molsLIne);
    void addAllostericPlexStatement(const std::string& alloPlexLine);
    void addAllostericOmniStatement(const std::string& alloOmniLine);
    void addDimerizationGenStatement(const std::string& dimerGenLine);
    void addOmniGenStatement(const std::string& omniGenLine);
    void addUniMolGenStatement(const std::string& uniMolGenLine);
    void addSpeciesStreamStatement(const std::string& speciesStreamLine);

  private:

    void DEBUG_doInterestingStuff();

  };


}