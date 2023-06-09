#include "TemplatePackage/TemplateProcessor.hxx"

#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>

#include <marlin/AIDAProcessor.h>

#include <cmath>

TemplateProcessor aTemplateProcessor ;

TemplateProcessor::TemplateProcessor()
  : Processor("TemplateProcessor") {
  // modify processor description
  _description = "TemplateProcessor provides a skeleton processor." ;

  // register steering parameters: name, description, class-variable, default value
  registerProcessorParameter("MinPt",
                             "Minimum particle pT.",
                             _minPt,
                             _minPt);

  registerInputCollection( LCIO::MCPARTICLE,
			   "InputCollectionName" , 
			   "Name of an input collection.",
			   _inputCollectionName,
			   _inputCollectionName
			   );

  registerOutputCollection( LCIO::MCPARTICLE,
			    "OutputCollectionName" ,
			   "Name of the output collection" ,
			   _outputCollectionName,
			   _outputCollectionName
			   );
}

void TemplateProcessor::init() {
  // Print the initial parameters
  printParameters() ;

  // Create useful histograms
  marlin::AIDAProcessor::histogramFactory(this);
  _h_pt = new TH1F("pt", "", 100, 0., 10);
}

void TemplateProcessor::processRunHeader( LCRunHeader* /*run*/) {
}

void TemplateProcessor::processEvent( LCEvent * evt ) {
  //
  // Get object required collections and create lists
  // to keep track of unsaved objects.

  // Loop over MCParticles
  LCCollection* inputCol = evt->getCollection(_inputCollectionName);

  if( inputCol->getTypeName() != lcio::LCIO::MCPARTICLE ) {
    throw EVENT::Exception( "Invalid collection type: " + inputCol->getTypeName() ) ;
  }

  std::cout << "Found " << inputCol->getNumberOfElements() << " particles." << std::endl;

  for(uint32_t i=0;i<inputCol->getNumberOfElements();i++) {
    const EVENT::MCParticle *mcp=static_cast<const EVENT::MCParticle*>(inputCol->getElementAt(i));
    
      // Check the particle pT
      const double* mom=mcp->getMomentum();
      double pt=std::sqrt(std::pow(mom[0],2)+std::pow(mom[1],2));
      if(pt<_minPt) {
	continue; 
      }

      _h_pt->Fill(pt);
  }
}

void TemplateProcessor::check( LCEvent * /*evt*/ )
{ }

void TemplateProcessor::end()
{ }
