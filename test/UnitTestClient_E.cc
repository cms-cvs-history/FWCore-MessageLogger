#include "FWCore/MessageLogger/test/UnitTestClient_E.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <iostream>
#include <string>

namespace edmtest
{


void
  UnitTestClient_E::analyze( edm::Event      const & e
                           , edm::EventSetup const & /*unused*/
                              )
{
  for (int i=1; i<=50; ++i) {
    edm::LogInfo   ("notListed1")   << 
  	"message with overall default limit of 5";
    edm::LogInfo   ("lim3bydefault")   << 
  	"message with specific overall default limit of 3";
   }
   
}  // MessageLoggerClient::analyze()


}  // namespace edmtest


using edmtest::UnitTestClient_E;
DEFINE_FWK_MODULE(UnitTestClient_E)
