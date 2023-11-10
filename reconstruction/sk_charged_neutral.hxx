#ifndef SK_CHARGED_NEUTRAL_H_
#define SK_CHARGED_NEUTRAL_H_

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

class sk_charged_neutral : marlin::Processor {
public:
  sk_charged_neutral();
  virtual ~sk_charged_neutral();

  virtual Processor* newProcessor() {
    return new sk_charged_neutral();
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
  sk_charged_neutral(const sk_charged_neutral& rhs);
  sk_charged_neutral(& operator=(const sk_charged_neutral&) { return *this; }

};


#endif /* KHURSHID_SOFT_KILLER_H_ */