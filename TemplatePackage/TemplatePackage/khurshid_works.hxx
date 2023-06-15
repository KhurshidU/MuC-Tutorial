#pragma once

#include <TH1F.h>
#include <IMPL/LCFlagImpl.h>
#include <marlin/Processor.h>

//! A template processor that should be modified to do what you want.
/**
 * Implements a loop over the MCParticle collection and creates an output
 * colleciton passing certain criteria. This provides rovides examples of:
 *  - using parameters to configure a processor
 *  - opening collections
 *  - outputing histograms
 */
class khurshid_works  : public marlin::Processor
{
public:
  virtual Processor*  newProcessor() { return new khurshid_works ; }

  khurshid_works(const khurshid_works&) = delete ;
  khurshid_works& operator =(const khurshid_works&) = delete ;
  khurshid_works() ;

  /** Called at the begin of the job before anything is read.
   * Use to initialize the processor, e.g. book histograms.
   */
  virtual void init() ;

  /** Called for every run.
   */
  virtual void processRunHeader( LCRunHeader* run ) ;

  /** Called for every event - the working horse.
   */
  virtual void processEvent( LCEvent * evt ) ; 

  virtual void check( LCEvent * evt ) ; 

  /** Called after data processing for clean up.
   */
  virtual void end() ;  
  LCFlagImpl _flag{};
private:
  //! Input Collection
    std::vector<std::string> _inputEcalCollections{};
  //! Output Collection
 
  std::vector<std::string> _outputEcalCollections{};
  
  //! Minimum pT for particle filter
  float _minPt = 1;

  //! Output histogram
  TH1 *_h_pt = nullptr;
};
