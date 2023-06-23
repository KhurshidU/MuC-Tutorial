

#include "TemplatePackage/khurshid_soft_killer.hxx"

#include <EVENT/LCCollection.h>

#include <marlin/AIDAProcessor.h>
#include <marlin/Global.h>
#include <cmath>
#include <IMPL/CalorimeterHitImpl.h>
#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>
#include <iomanip> 

#include </opt/ilcsoft/muonc/MarlinFastJet/v00-05-02/include/FastJetUtil.h>

#include <IMPL/ReconstructedParticleImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/ParticleIDImpl.h>
#include <IMPL/VertexImpl.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/MCParticle.h>

khurshid_soft_killer akhurshid_soft_killer ;

khurshid_soft_killer::khurshid_soft_killer():
                                        Processor("khurshid_soft_killer"),
                                       _lcParticleInName(""),
                                       _lcParticleOutName_kh(""),
                                       _lcJetOutName_kh(""),
                                       _statsFoundJets(0),
                                       _statsNrEvents(0),
                                       _statsNrSkippedEmptyEvents(0),
                                       _statsNrSkippedFixedNrJets(0),
                                       _statsNrSkippedMaxIterations(0),
                                       _storeParticlesInJets(false),
                                       _fju(new FastJetUtil())
 
 
 
 { // modify processor description
  _description = "Khurshid's Processor stores a collection of reconstructed particles, converts them into pseudo jets and applies softkiller" ;
   
      
registerInputCollection(LCIO::RECONSTRUCTEDPARTICLE, "recParticleIn", "a list of all reconstructed particles we are searching for jets in.", _lcParticleInName, "MCParticle");
registerOutputCollection(LCIO::RECONSTRUCTEDPARTICLE, "jetOut_kh", "The identified jets by khurshid", _lcJetOutName_kh, "JetOut_kh");

registerOutputCollection(LCIO::RECONSTRUCTEDPARTICLE, "recParticleOut_kh", "a list of all reconstructed particles used to make jets. If no value specified collection is not created", _lcParticleOutName_kh, "");

registerProcessorParameter(
                             "storeParticlesInJets",
                             "Store the list of particles that were clustered into jets in the recParticleOut collection",
                             _storeParticlesInJets,
                             false);

 _fju->registerFastJetParameters( this );

std::cout << "crush point_2" << std::endl;

}

khurshid_soft_killer::khurshid_soft_killer(const khurshid_soft_killer& rhs):
  Processor( rhs.name() ),
  _lcParticleInName(rhs._lcParticleInName),
  _lcParticleOutName_kh(rhs._lcParticleOutName_kh),
  _lcJetOutName_kh(rhs._lcJetOutName_kh),
  _statsFoundJets(rhs._statsFoundJets),
  _statsNrEvents(rhs._statsNrEvents),
  _statsNrSkippedEmptyEvents(rhs._statsNrSkippedEmptyEvents),
  _statsNrSkippedFixedNrJets(rhs._statsNrSkippedFixedNrJets),
  _statsNrSkippedMaxIterations(rhs._statsNrSkippedMaxIterations),
  _storeParticlesInJets(rhs._storeParticlesInJets),
  _fju( new FastJetUtil(*rhs._fju) )
 {}
//      FastJetProcessor& operator=(const FastJetProcessor&) {}


khurshid_soft_killer::~khurshid_soft_killer() {
  delete _fju;
}


void khurshid_soft_killer::init() {
  // Print the initial parameters
 // printParameters() ;

  printParameters();

  // parse the given steering parameters
  _fju->init();
  streamlog_out(MESSAGE) << "Jet Algorithm Khurshid's Soft Killer: " << _fju->_jetAlgo->description() << std::endl << std::endl;

}


void khurshid_soft_killer::processEvent( LCEvent * evt ) {
 
 /*  if( inputCol->getTypeName() != LCIO::CALORIMETERHIT ) {
    throw EVENT::Exception( "Invalid collection type: " + inputCol->getTypeName() ) ;
  }*/
  LCCollection* particleIn(NULL);
  try
    {
      // get the input collection if existent
      particleIn = evt->getCollection(_lcParticleInName);
      if (particleIn->getNumberOfElements() < 1) {
        _statsNrSkippedEmptyEvents++;
        throw DataNotAvailableException("Collection is there, but its empty!");
      }

    } catch (DataNotAvailableException e) {
    streamlog_out(WARNING) << e.what() << std::endl << "Skipping" << std::endl;

    //create dummy empty collection only in case there are processor that need the presence of them in later stages

    // create output collection and save every jet with its particles in it
    IMPL::LCCollectionVec* lccJetsOut_kh = new IMPL::LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
    // create output collection and save every particle which contributes to a jet

    IMPL::LCCollectionVec* lccParticlesOut_kh(NULL);
    if (_storeParticlesInJets){
      lccParticlesOut_kh= new IMPL::LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
      lccParticlesOut_kh->setSubset(true);
    }

    evt->addCollection(lccJetsOut_kh, _lcJetOutName_kh);
    if (_storeParticlesInJets) evt->addCollection(lccParticlesOut_kh, _lcParticleOutName_kh);


    return ;
  }

// convert to pseudojet list
  PseudoJetList pjList = _fju->convertFromRecParticle(particleIn);

// softkiller will be here 
  //std::cout<<"Pseudo Jet list"<<pjList<<std::endl;
  double grid_size = 0.2;
  double rapmax = 2.5; //5 units of rapidity
  fastjet::contrib::SoftKiller soft_killer(rapmax, grid_size);

  //contrib::SoftKiller soft_killer(-0.0, 4.0, 0.5, 0.3);
   std::cout << "# of pfos in PseudoJet before soft killer   " << pjList.size()<<std::endl;
for (unsigned int i=0; i<pjList.size(); i++){
    const fastjet::PseudoJet &jet = pjList[i];
    std::cout << "pt = " << jet.pt()
	 << ", rap = " << jet.rap()
   << ", phi = " <<jet.phi()
	 << ", mass = " << jet.m() << std::endl;
  }



  
  double pt_threshold;
  std::vector<fastjet::PseudoJet>  pjList_softly_killed;
  soft_killer.apply(pjList, pjList_softly_killed, pt_threshold);
// alternative, more compact invocation
  //std::vector<fastjet::PseudoJet> pjList_softly_killed = soft_killer(pjList);
  std::cout << "# Ran the following soft killer: " << soft_killer.description() << std::endl;

  //std::cout << setprecision(4);

  std::cout << std::setprecision(4);
  std::cout << "Soft Killer applied a pt threshold of  " << pt_threshold << std::endl;

  

std::cout << "# of pfos in PseudoJet after soft killer  "<<pjList_softly_killed.size() << std::endl;
for (unsigned int i=0; i<pjList_softly_killed.size(); i++){
    const fastjet::PseudoJet &jet = pjList_softly_killed[i];
    std::cout << "pt = " << jet.pt()
	 << ", rap = " << jet.rap()
   << ", phi = " <<jet.phi()
	 << ", mass = " << jet.m() << std::endl;
  }



  PseudoJetList jetsk;
  try {
    jetsk = _fju->clusterJets(pjList_softly_killed, particleIn); //pass soft kill event instead of pjList
  } catch( SkippedFixedNrJetException& e ) {
    _statsNrSkippedFixedNrJets++;
  } catch( SkippedMaxIterationException& e ) {
    jetsk = e._jets;
    _statsNrSkippedMaxIterations++;
  }


  //std::vector<fastjet::PseudoJet> kill_jets = sel_jets(jetsk.inclusive_jets());
// Sanity check -- I don't trust myself 

/*std::cout << "# original full jets" << std::endl;
  for (unsigned int i=0; i<full_jets.size(); i++){
    const fastjet::PseudoJet &jet = full_jets[i];
    std::cout << "pt = " << jet.pt()
	 << ", rap = " << jet.rap()
	 << ", mass = " << jet.m() << std::endl;
  }
 std::cout << std::endl;*/
  std::cout << "check point 1  " << std::endl;
  std::cout << "# jets after clustering " << std::endl;

  for (unsigned int i=0; i<jetsk.size(); i++){
    const fastjet::PseudoJet &jet = jetsk[i];
    std::cout << "pt = " << jet.pt()
	 << ", rap = " << jet.rap()
   << ", phi = " <<jet.phi()
	 << ", mass = " << jet.m() << std::endl;
  }
  std::cout << std::endl;
  std::cout << "check point 2  " << std::endl;

  _statsNrEvents++;
  _statsFoundJets += jetsk.size();
  const unsigned nrJets = jetsk.size();

  // create output collection and save every jet with its particles in it
  IMPL::LCCollectionVec* lccJetsOut_kh = new IMPL::LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
  // create output collection and save every particle which contributes to a jet
  IMPL::LCCollectionVec* lccParticlesOut_kh(NULL);
  if (_storeParticlesInJets){
    lccParticlesOut_kh= new IMPL::LCCollectionVec(LCIO::RECONSTRUCTEDPARTICLE);
    lccParticlesOut_kh->setSubset(true);
  }

 PseudoJetList::iterator it;

  for (it=jetsk.begin(); it != jetsk.end(); it++) {
    // create a reconstructed particle for this jet, and add all the containing particles to it
    ReconstructedParticle* rec = _fju->convertFromPseudoJet( (*it), _fju->_cs->constituents(*it), particleIn);
    lccJetsOut_kh->addElement( rec );

    if (_storeParticlesInJets) {
      for (unsigned int n = 0; n < _fju->_cs->constituents(*it).size(); ++n) {
        ReconstructedParticle* p =
          static_cast<ReconstructedParticle*>(particleIn->getElementAt((_fju->_cs->constituents(*it))[n].user_index()));
        lccParticlesOut_kh->addElement( p );
      }
    }
  }

  evt->addCollection(lccJetsOut_kh, _lcJetOutName_kh);
  if (_storeParticlesInJets) evt->addCollection(lccParticlesOut_kh, _lcParticleOutName_kh);


  // special case
 // special case for the exclusive jet mode: we can save the transition y_cut value
  if (_fju->_clusterMode == FJ_exclusive_nJets && jetsk.size() == _fju->_requestedNumberOfJets) {
    // save the dcut value for this algorithm (although it might not be meaningful)
    LCParametersImpl &lccJetParams((LCParametersImpl &)lccJetsOut_kh->parameters());

    lccJetParams.setValue(std::string("d_{n-1,n}"), (float)_fju->_cs->exclusive_dmerge(nrJets-1));
    lccJetParams.setValue(std::string("d_{n,n+1}"), (float)_fju->_cs->exclusive_dmerge(nrJets));
    lccJetParams.setValue(std::string("y_{n-1,n}"), (float)_fju->_cs->exclusive_ymerge(nrJets-1));
    lccJetParams.setValue(std::string("y_{n,n+1}"), (float)_fju->_cs->exclusive_ymerge(nrJets));
  }






}


void khurshid_soft_killer::end()
{ streamlog_out(MESSAGE)
    << "Found jets (after soft killer was applied): " << _statsFoundJets
    << " (" << (double)_statsFoundJets/_statsNrEvents << " per event) "
    << " - Skipped Empty events:" << _statsNrSkippedEmptyEvents
    << " - Skipped Events after max nr of iterations reached: " << _statsNrSkippedMaxIterations
    << " - Skipped Search for Fixed Nr Jets (due to insufficient nr of particles):" << _statsNrSkippedFixedNrJets
    << std::endl;
    
    }
