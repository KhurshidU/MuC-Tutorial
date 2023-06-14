#include "TemplatePackage/khurshid_works.hxx"

#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>

#include <marlin/AIDAProcessor.h>
#include <marlin/Global.h>
#include <cmath>
#include <IMPL/CalorimeterHitImpl.h>
#include <EVENT/LCCollection.h>
#include <IMPL/LCCollectionVec.h>

khurshid_works akhurshid_works ;

khurshid_works::khurshid_works()
  : Processor("khurshid_works") {
  // modify processor description
  _description = "Khurshid's Processor stores a collection of ECalorimeter hits and outputs a copy." ;
   



  // register steering parameters: name, description, class-variable, default value
 /* registerProcessorParameter("MinPt",
                             "Minimum particle pT.",
                              _minPt,
                             0.5);*/

  std::vector<std::string> ecalCollections; // 
  ecalCollections.push_back(std::string("EcalBarrelCollection"));
  
  registerInputCollection( LCIO::CALORIMETERHIT,
                            "ECALOutputCollection_kh_input" ,
                            "ECAL Collection of real Hits" ,
                            //_inputEcalCollections.at(0),
                         _inputEcalCollection,   std::string("ECALBarrel") );
  std::cout << "crush point_1" << std::endl;
  _outputEcalCollections.push_back(std::string("ECALBarrel"));      


  registerOutputCollection( LCIO::CALORIMETERHIT, // output - copy of ecal hits 
                            "ECALOutputCollection_kh_output" ,
                            "Khurshid's copy of ECAL Collection of real Hits" ,
                            _outputEcalCollections.at(0),
                            std::string("ECALBarrel_output") );   
std::cout << "crush point" << _outputEcalCollections.at(0)<< std::endl;

}


void khurshid_works::init() {
  // Print the initial parameters
 // printParameters() ;

  // Create useful histograms
 /* marlin::AIDAProcessor::histogramFactory(this);
  _h_pt = new TH1F("pt", "", 100, 0., 10);*/
}

void khurshid_works::processRunHeader( LCRunHeader* /*run*/) {
}

void khurshid_works::processEvent( LCEvent * evt ) {
  //
  // Get object required collections and create lists
  // to keep track of unsaved objects.

  // Loop over MCParticles
 // LCCollection* inputCol = evt->getCollection(_inputEcalCollections.at(0));
    LCCollection* inputCol = evt->getCollection(_inputEcalCollection);
  if( inputCol->getTypeName() != LCIO::CALORIMETERHIT ) {
    throw EVENT::Exception( "Invalid collection type: " + inputCol->getTypeName() ) ;
  }

  std::cout << "Found " << inputCol->getNumberOfElements() << " ECalorimeter hits." << std::endl;
  LCCollectionVec *ecalcol_kh = new LCCollectionVec(LCIO::CALORIMETERHIT);
  for(uint32_t i=0;i<inputCol->getNumberOfElements();i++) {
	 CalorimeterHitImpl *hit_copy=static_cast<CalorimeterHitImpl*>(inputCol->getElementAt(i));
         ecalcol_kh->addElement(hit_copy);
         std::cout << "Found " << hit_copy<< " ECalorimeter hits." << std::endl;
   }


  // LCCollectionVec *ecalcol = new LCCollectionVec(LCIO::CALORIMETERHIT);
 //  ecalcol_khurshid = myVector;
   //ecalcol->setFlag(_flag.getFlag());
   evt->addCollection(ecalcol_kh,_outputEcalCollections[0].c_str());
 // Create and add relation collection for ECAL/HCAL to event
  //    chschcol = calohitNav.createLCCollection();
 ///     evt->addCollection(chschcol,_outputRelCollection.c_str());




}

void khurshid_works::check( LCEvent * /*evt*/ )
{ }
void khurshid_works::end()
{ }
