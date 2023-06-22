#ifndef KHURSHID_SOFT_KILLER_H_
#define KHURSHID_SOFT_KILLER_H_

#include "/opt/ilcsoft/muonc/MarlinFastJet/v00-05-02/include/EClusterMode.h"

#include "marlin/Processor.h"
#include "marlin/VerbosityLevels.h"
#include "EVENT/LCCollection.h"
#include "EVENT/ReconstructedParticle.h"
#include "IMPL/LCCollectionVec.h"
#include "LCIOSTLTypes.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"

#include "fastjet/ClusterSequence.hh"
#include "/opt/ilcsoft/FastJet/3.2.1/fjcontrib-1.025/SoftKiller/SoftKiller.hh"

#include <vector>
#include <string>

//Forward declaration
class FastJetUtil;
typedef std::vector< fastjet::PseudoJet > PseudoJetList;

class khurshid_soft_killer : marlin::Processor {
public:
  khurshid_soft_killer();
  virtual ~khurshid_soft_killer();

  virtual Processor* newProcessor() {
    return new khurshid_soft_killer();
  }

  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init();

  /** Called for every run.
   */
  virtual void processRunHeader(LCRunHeader* ){};

  /** Called for every event - the working horse.
   */
  virtual void processEvent(LCEvent * evt);

  virtual void check(LCEvent * ) {};

  /** Called after data processing for clean up.
   */
  virtual void end();

  friend class FastJetUtil;

private:

  // the LC Collection names for input/output
  std::string   _lcParticleInName;
  std::string   _lcParticleOutName_kh;
  std::string   _lcJetOutName_kh;

  int  _statsFoundJets;
  int _statsNrEvents;
  int _statsNrSkippedEmptyEvents;
  int _statsNrSkippedFixedNrJets;
  int _statsNrSkippedMaxIterations;
  bool _storeParticlesInJets;

  FastJetUtil* _fju;

private:
  khurshid_soft_killer(const khurshid_soft_killer& rhs);
  khurshid_soft_killer& operator=(const khurshid_soft_killer&) { return *this; }

};


#endif /* KHURSHID_SOFT_KILLER_H_ */