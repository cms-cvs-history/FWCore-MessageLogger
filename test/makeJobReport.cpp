
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>

#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/PluginManager/interface/standard.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/Presence.h"
#include "FWCore/MessageLogger/interface/ExceptionMessages.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageLogger/interface/MessageDrop.h"
#include "FWCore/PluginManager/interface/PresenceFactory.h"
#include "FWCore/MessageLogger/interface/JobReport.h"
#include "FWCore/ServiceRegistry/interface/ServiceRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLoggerQ.h"

/*
 * Test binary for job reports.
 *
 * Add a BuildFile looking like:
 *
 * <bin name=makeJobReport file=makeJobReport.cpp>
 * <use name=boost>
 * <use name=boost_program_options> 
 * <use name=FWCore/Framework>
 * <use name=FWCore/MessageLogger>
 * <use name=FWCore/PluginManager>
 * <use name=FWCore/ServiceRegistry>
 * <use name=FWCore/Utilities>
 *
 * </bin>
 *
 * To build this and then run it from a runtime environment
 *
 *
 *
 *
 *
 */

void work()
{

  // We must initialize the plug-in manager first
  try {
    edmplugin::PluginManager::configure(edmplugin::standard::config());
  } catch(cms::Exception& e) {
    std::cerr << e.what() << std::endl;
    return;
  }
  
  // Load the message service plug-in
  boost::shared_ptr<edm::Presence> theMessageServicePresence;
  try {
    theMessageServicePresence = boost::shared_ptr<edm::Presence>(edm::PresenceFactory::get()->makePresence("MessageServicePresence").release());
  } catch(cms::Exception& e) {
    std::cerr << e.what() << std::endl;
    return;
  }

  

  //
  // Make JobReport Service up front
  // 
  std::string jobReportFile = "FrameworkJobReport.xml";
  std::auto_ptr<edm::JobReport> jobRep(new edm::JobReport());  
  edm::ServiceToken jobReportToken = 
    edm::ServiceRegistry::createContaining(jobRep);
  
  std::string * jr_name_p = new std::string("FrameworkJobReport.xml");
  edm::MessageLoggerQ::MLqJOB( jr_name_p );
  
  
    
  std::string * jm_p = new std::string("");
  edm::MessageLoggerQ::MLqMOD( jm_p );

  edm::ParameterSet * params_p = new edm::ParameterSet();
  edm::MessageLoggerQ::MLqCFG(params_p);
  
  
  std::cout << "Testing JobReport" << std::endl;
  edm::JobReport * theReport = new edm::JobReport(); 
  
  

  std::vector<std::string> inputBranches;
  for (int i = 0; i < 10; i++){
    inputBranches.push_back("Some_Input_Branch");
  }
  
 std::size_t inpFile = theReport->inputFileOpened("InputPFN",
						  "InputLFN",
						  "InputCatalog",
						  "InputSource",
						  "InputLabel",
						  "InputGUID",
						  inputBranches);
  
 
  std::vector<std::string> outputBranches;
  for (int i=0; i < 10; i++){
    outputBranches.push_back("Some_Output_Branch_Probably_From_HLT");

  }

  
  std::size_t outFile = theReport->outputFileOpened("OutputPFN",
						    "OutputLFN",
						    "OutputCatalog",
						    "OutputModule",
						    "OutputModuleName",
						    "OutputGUID",
						    "DataType",
						    "ThisIsAHash",
						    outputBranches);

  
  for (int i=0; i < 1000; i++){
    theReport->eventReadFromFile(inpFile, 1000001, i);
    theReport->eventWrittenToFile(outFile, 1000001, i);
  }

  theReport->inputFileClosed(inpFile);
  theReport->outputFileClosed(outFile);
  theReport->reportPSetHash("ThisIsAPSetHash");
  

  std::map<std::string, std::string> timingInfo;
  timingInfo.insert( std::make_pair( "Time1", "1234" ) );
  timingInfo.insert( std::make_pair( "Time2", "4567" ) );
  timingInfo.insert( std::make_pair( "Time3", "7890" ) );
  
  theReport->reportPerformanceSummary("Timing", timingInfo);
  
  std::map<std::string, std::string> moduleTimingInfo;
  moduleTimingInfo.insert( std::make_pair( "Time1", "1234" ) );
  moduleTimingInfo.insert( std::make_pair( "Time2", "5678" ) );
  moduleTimingInfo.insert( std::make_pair( "Time3", "8901" ) );
  
  std::map<std::string, std::string> moduleMemInfo;
  moduleMemInfo.insert( std::make_pair( "Memory1", "1234" ) );
  moduleMemInfo.insert( std::make_pair( "Memory2", "5678" ) );
  moduleMemInfo.insert( std::make_pair( "Memory3", "8901" ) );


  std::map<std::string, std::string> moduleTrigInfo;
  moduleTrigInfo.insert( std::make_pair( "Trigger1", "Pass" ) );
  moduleTrigInfo.insert( std::make_pair( "Trigger2", "Fail" ) );
  moduleTrigInfo.insert( std::make_pair( "Trigger3", "Pass" ) );
  
  
  theReport->reportPerformanceForModule("Timing", "Module1", moduleTimingInfo);
  theReport->reportPerformanceForModule("Memory", "Module1", moduleMemInfo);
  theReport->reportPerformanceForModule("Trigger", "Module1", moduleTrigInfo);
  
  //edm::LogInfo("FwkJob") << "Is anybody out there?";



}

int main()
{
  int rc = -1;
  try {
      work();
      
      rc = 0;
  }
  
  catch ( ... ) {
      std::cerr << "Unknown exception caught\n";
      rc = 2;
  }
  return rc;
}
