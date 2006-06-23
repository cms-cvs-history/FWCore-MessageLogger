/*----------------------------------------------------------------------

   This is a generic main that can be used with any plugin and a 
   PSet script. It shows the minimum machinery necessary to a
   "standalone" program to issue MessageLogger nessages.


----------------------------------------------------------------------*/  

#include <exception>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/ProblemTracker.h"
#include "FWCore/Utilities/interface/Presence.h"
#include "FWCore/Utilities/interface/PresenceFactory.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

// -----------------------------------------------

void grumble( )
{
// Issue several types of logger messages

  double d = 3.14159265357989;
  edm::LogWarning("cat_A")   << "Test of std::setprecision(p):"
  			     << " Pi with precision 12 is " 
  			     << std::setprecision(12) << d;

  for( int i=0; i<1000; ++i) {
    edm::LogInfo("cat_B")    << "\t\tEmit Info level message " << i+1;
    edm::LogWarning("cat_C") << "\t\tEmit Warning level message " << i+1;
  }
}  

int main(int argc, char* argv[]) {

  std::string const kProgramName = argv[0];

  int rc = 0;
  try {

//  We must initialize the plug-in manager first
    edm::AssertHandler ah;

    // Load the message service plug-in.  Forget this and bad things happen!
    boost::shared_ptr<edm::Presence> theMessageServicePresence;
    theMessageServicePresence = boost::shared_ptr<edm::Presence>(edm::PresenceFactory::get()->
      makePresence("MessageServicePresence").release());

//  Manufacture a configuration and set it
    std::string config =
      "process x = {"
	"service = MessageLogger {"
	  "untracked vstring destinations = {'infos','warnings'}"
	  "untracked PSet infos = {"
	    "untracked string threshold = 'INFO'"
	    "untracked PSet default = {untracked int32 limit = 1000000}"
	    "untracked PSet FwkJob = {untracked int32 limit = 0}"
	  "}"
	  "untracked PSet warnings = {"
	    "untracked string threshold = 'WARNING'"
	    "untracked PSet default = {untracked int32 limit = 1000000}"
	  "}"
	  "untracked vstring fwkJobReports = {'FrameworkJobReport.xml'}"
	  "untracked vstring categories = {'FwkJob'}"
	  "untracked PSet FrameworkJobReport.xml = {"
	    "untracked PSet default = {untracked int32 limit = 0}"
	    "untracked PSet FwkJob = {untracked int32 limit = 10000000}"
	  "}"
	"}"
	"service = JobReportService{}"
	"service = SiteLocalConfigService{}"
      "}";


    boost::shared_ptr<std::vector<edm::ParameterSet> > pServiceSets;
    boost::shared_ptr<edm::ParameterSet>          params_;
    edm::makeParameterSets(config, params_, pServiceSets);

//  create the services
    edm::ServiceToken tempToken(edm::ServiceRegistry::createSet(*pServiceSets.get()));

//  make the services available
    edm::ServiceRegistry::Operate operate(tempToken);

//  and generate a bunch of messages
    grumble( );
  }

//  Deal with exceptions
  catch (cms::Exception& e) {
    std::cout << "cms::Exception caught in "
                                << kProgramName
                                << "\n"
                                << e.explainSelf();
    rc = 1;
  }
  catch (seal::Error& e) {
    std::cout << "Exception caught in "
                                << kProgramName
                                << "\n"
                                << e.explainSelf();
    rc = 1;
  }
  catch (std::exception& e) {
    std::cout << "Standard library exception caught in "
                                << kProgramName
                                << "\n"
                                << e.what();
    rc = 1;
  }
  catch (...) {
    std::cout << "Unknown exception caught in "
                                << kProgramName;
    rc = 2;
  }

  return rc;
}
