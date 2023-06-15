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

  //Loading the collection created by DDigiCalo_BIB, 3 collections since DDigiCalo_BIB outputs 3 

  std::vector<std::string> ecalCollections; // 
  ecalCollections.push_back(std::string("ECALBarrel"));
  ecalCollections.push_back(std::string("ECALEndcap"));
  ecalCollections.push_back(std::string("ECALOther"));
  
 registerInputCollections( LCIO::CALORIMETERHIT,
                            "ECALOutputCollection_kh_input" ,
                            "ECAL Collection of real Hits" ,
                            _inputEcalCollections,    
                            ecalCollections) ;
                                       
/*registerInputCollection( LCIO::CALORIMETERHIT,
                            "ECALOutputCollection_kh_input" ,
                            "ECAL Collection of real Hits" ,
                            //_inputEcalCollections.at(0),
                            _inputEcalCollectionion,  
                             std::string("ECALBarrel") );

  registerInputCollection( LCIO::CALORIMETERHIT,
                            "ECALOutputCollection1" ,
                            "ECAL Collection of real Hits" ,
                            _inputEcalCollection.at(1),
                            std::string("ECALEndcap") );

 registerInputCollection( LCIO::CALORIMETERHIT,
                            "ECALOutputCollection2" ,
                            "ECAL Collection of real Hits" ,
                            _inputEcalCollection.at(2),
                            std::string("ECALOther") ) ;*/


   
  std::cout << "crush point_1" << std::endl;

  _outputEcalCollections.push_back(std::string("ECALBarrel"));
  _outputEcalCollections.push_back(std::string("ECALEndcap"));
  _outputEcalCollections.push_back(std::string("ECALOther"));      


  registerOutputCollection( LCIO::CALORIMETERHIT, // output - copy of ecal hits 
                            "ECALBarrel_kh" ,
                            "Khurshid's copy of ECAL Barrel Collection of real Hits" ,
                            _outputEcalCollections.at(0),
                            std::string("ECALBarrel_output") );
   
 registerOutputCollection( LCIO::CALORIMETERHIT,
                            "ECALEndcaps_kh" ,
                           "Khurshid's copy of ECAL Endcaps Collection of real Hits " ,
                            _outputEcalCollections.at(1),
                            std::string("ECALEndcap_output") );

  registerOutputCollection( LCIO::CALORIMETERHIT,
                            "ECALOther_kh" ,
                            "Khurshid's copy of ECAL Others Collection of real Hits" ,
                            _outputEcalCollections.at(2),
                            std::string("ECALOther_output") ) ;
   

  std::cout << "crush point_2" << _outputEcalCollections.at(0)<< std::endl;

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

   LCCollection* inputCol = evt->getCollection(_inputEcalCollections.at(0));
   //LCCollection* inputCol_Endcap = evt->getCollection(_inputEcalCollections.at(1));
 //  LCCollection* inputCol_Other = evt->getCollection(_inputEcalCollections.at(2));
   //LCCollection* inputCol = evt->getCollection(_inputEcalCollection);
 
   if( inputCol->getTypeName() != LCIO::CALORIMETERHIT ) {
    throw EVENT::Exception( "Invalid collection type: " + inputCol->getTypeName() ) ;
  }

  /*if( inputCol_Other->getTypeName() != LCIO::CALORIMETERHIT ) {
    throw EVENT::Exception( "Invalid collection type: " + inputCol_Other->getTypeName() ) ;
  }*/



  std::cout << "Found " << inputCol->getNumberOfElements() << " ECalorimeter hits." << std::endl;
  LCCollectionVec *ecalcol_kh = new LCCollectionVec(LCIO::CALORIMETERHIT);
  _flag.setBit(LCIO::CHBIT_LONG);
  _flag.setBit(LCIO::RCHBIT_TIME);
  ecalcol_kh->setFlag(_flag.getFlag());
  
  for(uint32_t i=0;i<inputCol->getNumberOfElements();i++) {
	// CalorimeterHitImpl *hit_copy=static_cast<CalorimeterHitImpl*>(inputCol->getElementAt(i));
         CalorimeterHitImpl *hit_copy=new CalorimeterHitImpl(*static_cast<CalorimeterHitImpl*>(inputCol->getElementAt(i)));
         if ( hit_copy->getEnergy() > 0.5 ) {
            ecalcol_kh->addElement(hit_copy); 
 	    std::cout << "Found " << hit_copy<< " ECalorimeter hits above 0.5 GeV." << std::endl;
         }
          
   }
  

 // std::string colName =  _inputEcalCollection;
 // LCCollection * col = evt->getCollection( colName.c_str() ) ;
  std::string initString = inputCol->getParameters().getStringVal(LCIO::CellIDEncoding);
  std::cout << "crush point 3 " << std::endl;
  
  ecalcol_kh->parameters().setValue(LCIO::CellIDEncoding,initString);
  evt->addCollection(ecalcol_kh,_outputEcalCollections[0].c_str());
  std::cout << "crush point 4  " << std::endl;



}

void khurshid_works::check( LCEvent * /*evt*/ )
{ }
void khurshid_works::end()
{ }
