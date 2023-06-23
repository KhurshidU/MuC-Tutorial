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

   
// std::cout << "crush point_1" << std::endl;

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
   
 //HCAL Collection
    
  std::vector<std::string> hcalCollections;
  hcalCollections.push_back(std::string("HCALBarrel"));
  hcalCollections.push_back(std::string("HCALEndcap "));
  hcalCollections.push_back(std::string("HCALOther"));

  registerInputCollections( LCIO::CALORIMETERHIT,
			    "HCALOutputCollection_kh_input",
                            "HCAL Collection of real Hits", 
                             _inputHcalCollections,	
		             hcalCollections);

  _outputHcalCollections.push_back(std::string("HCALBarrel"));
  _outputHcalCollections.push_back(std::string("HCALEndcap"));
  _outputHcalCollections.push_back(std::string("HCALOther"));
 
  registerOutputCollection( LCIO::CALORIMETERHIT,
                            "HCALBarrel_kh", 
                            "Khurshid's copy of HCAL Barrel Collection of real Hits",
                            _outputHcalCollections.at(0),
                            std::string("HCALBarrel_output") );
  
  registerOutputCollection( LCIO::CALORIMETERHIT,
                            "HCALEndcap_kh",
                            "Khurshid's copy of HCAL Endcaps Collection of real Hits",
                            _outputHcalCollections.at(1),
                            std::string("HCALOther_output") ) ;
  
  registerOutputCollection( LCIO::CALORIMETERHIT,
                            "HCALOther_kh",
                            "Khurshid's copy of HCAL Others Collection of real Hits",
                            _outputHcalCollections.at(2),
                            std::string("HCALOther_output") );
// std::cout << "crush point_2" << _outputEcalCollections.at(0)<< std::endl;

}

bool applyDifferentialThresholdEcalBIB(CalorimeterHitImpl * calhit, bool useCLIC, bool useCrilin) {
  bool pass = false;

  if (useCLIC==false && useCrilin==false) pass = true;

  if (useCLIC==true){
	  
    float x = calhit->getPosition()[0];
    float y = calhit->getPosition()[1];
    float z = calhit->getPosition()[2];
    float d = sqrt(x*x+y*y+z*z);
    float R = sqrt(x*x+y*y);
    float theta = acos(z/d);

    float theta_bins[3+1] = {1.57-1.02,1.57-0.27,1.57+0.27,1.57+1.02};
    float R_bins[5+1] = {1500,1570,1600,1650,1710,1770};

    float th_barrel[5][3] = {18.2731,  17.1456,  18.4469,
                    24.7357,  21.9555,  24.6477,
                    33.8206,  30.3771,  33.8971,
                    43.646,   42.9506,  43.3244,
                    45.6661,  47.0891,  46.0237};

    float std_barrel[5][3] = {32.9455,  31.5414,  34.2989,
                        55.6836,  48.5732,  55.6375,
                        77.1442,  71.8559,  76.0541,
                        93.3592,  94.8317,  91.6914,
                        96.7721,  96.9367,  94.6892};

    for (int i_theta=0; i_theta<3; i_theta++){
      for (int i_R=0; i_R<5; i_R++){
        if (theta>theta_bins[i_theta] && theta<theta_bins[i_theta+1] && R>R_bins[i_R] && R<R_bins[i_R+1]) {
          if (calhit->getEnergy()>(th_barrel[i_R][i_theta]+2*std_barrel[i_R][i_theta])*0.001) {pass = true;
                                                                  calhit->setEnergy(calhit->getEnergy()-th_barrel[i_theta][i_R]*0.001);
                                                                 }
        }
      }
    } 


    }

    if (useCrilin==true){

    float x = calhit->getPosition()[0];
    float y = calhit->getPosition()[1];
    float z = calhit->getPosition()[2];

     //Rotation of the dodecaedra
     
         float phi = acos(x/sqrt(x*x+y*y));
    
         if (y<0) phi = -phi;
    
         float xphi = phi + 3.14159/12.;
    
         if (xphi<0) xphi = 2*3.14159+xphi;
     
         int nphi = xphi*6./3.14159;
   
         float delta_phi = 3.14159/2.-2*3.14159/12.*nphi;
  
         //float xprime = x*cos(delta_phi)-y*sin(delta_phi);
         float yprime = x*sin(delta_phi)+y*cos(delta_phi);
    
         float m_energyc[5] = {14.1607,17.6719,23.0927,24.9074,28.0001};
         float m_energyf[5] = {20.3787,18.6048,19.045,20.4087,23.8197};
         float s_energyc[5] = {18.0425,24.4413,36.4643,33.8902,36.6783};
         float s_energyf[5] = {20.9336,20.1998,22.8853,26.9295,35.3377};
     //
         float tenergyc[5];
         float tenergyf[5];
    
         for (int k=0; k<5; k++){
           tenergyc[k]=m_energyc[k]+2*s_energyc[k];
           tenergyf[k]=m_energyf[k]+2*s_energyf[k];
           }	    
                                                                          
         if (z>-500 && z<500){ //Central region
         if (yprime<1550) if (calhit->getEnergy()>tenergyc[0]*0.001) pass = true; 
         if (yprime>1550 && yprime<1600 ) if (calhit->getEnergy()>tenergyc[1]*0.001) pass = true;
         if (yprime>1600 && yprime<1630) if (calhit->getEnergy()>tenergyc[2]*0.001) pass = true;
         if (yprime>1630 && yprime<1660) if (calhit->getEnergy()>tenergyc[3]*0.001) pass = true;
         if (yprime>1660) if (calhit->getEnergy()>tenergyc[4]*0.001) pass = true;
                                                                                  }
  
         if (z<-500 || z>500){ //Forward region
         if (yprime<1550) if (calhit->getEnergy()>tenergyf[0]*0.001) pass = true;
         if (yprime>1550 && yprime<1600 ) if (calhit->getEnergy()>tenergyf[1]*0.001) pass = true;
         if (yprime>1600 && yprime<1630) if (calhit->getEnergy()>tenergyf[2]*0.001) pass = true;
         if (yprime>1630 && yprime<1660) if (calhit->getEnergy()>tenergyf[3]*0.001) pass = true;
         if (yprime>1660) if (calhit->getEnergy()>tenergyf[4]*0.001) pass = true;
          }	  
         
        }
     
        return pass;
     
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
 
 /*  if( inputCol->getTypeName() != LCIO::CALORIMETERHIT ) {
    throw EVENT::Exception( "Invalid collection type: " + inputCol->getTypeName() ) ;
  }*/
  
   for (int i = 0; i < _inputEcalCollections.size(); i++) {
    std::string colName =  _inputEcalCollections[i] ;

    streamlog_out ( DEBUG ) << "looking for collection: " << colName << std::endl;
	 try{ 
  		LCCollection* inputCol = evt->getCollection(colName);
    


   		 if ( colName.find("dummy")!=std::string::npos ) {
      			streamlog_out ( DEBUG ) << "ignoring input ECAL collection name (looks like dummy name)" << colName <<std::endl;
     			 continue;
   			 }
    
   		 LCCollectionVec *ecalcol_kh = new LCCollectionVec(LCIO::CALORIMETERHIT);
      		 ecalcol_kh->setFlag(_flag.getFlag());
   
 		 for(uint32_t i=0;i<inputCol->getNumberOfElements();i++) {
		// CalorimeterHitImpl *hit_copy=static_cast<CalorimeterHitImpl*>(inputCol->getElementAt(i));
        	 CalorimeterHitImpl *hit_copy=new CalorimeterHitImpl(*static_cast<CalorimeterHitImpl*>(inputCol->getElementAt(i)));
       		 
                	 if ( hit_copy->getEnergy() > 0.5 ) {
            			ecalcol_kh->addElement(hit_copy); 
 	   		 	//std::cout << "Found " << hit_copy<< " ECalorimeter hits above 0.5 GeV." << std::endl;
         			}
    
   			}

  		std::string initString = inputCol->getParameters().getStringVal(LCIO::CellIDEncoding);
  

  		ecalcol_kh->parameters().setValue(LCIO::CellIDEncoding,initString);
  		evt->addCollection(ecalcol_kh,_outputEcalCollections[i].c_str());
  //		std::cout << "crush point 4  " << std::endl;

   		}

  	 catch (DataNotAvailableException &e){
      		streamlog_out(DEBUG) << "could not find the input ECAL collection " << colName << std::endl;
      		}   

   }	
  


}


void khurshid_works::check( LCEvent * /*evt*/ )
{ }
void khurshid_works::end()
{ }
