#include "FWCore/MessageLogger/test/UnitTestClient_A.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <iostream>
#include <string>

namespace edmtest
{


void
  UnitTestClient_A::analyze( edm::Event      const & e
                           , edm::EventSetup const & /*unused*/
                              )
{
  std::string empty_;
  std::string file_ = "nameOfFile";
  edm::LogError  ("cat_A")   << "LogError was used to send this message";
  edm::LogError  ("cat_B")   << "LogError was used to send this other message";
  edm::LogWarning("cat_A")   << "LogWarning was used to send this message";
  edm::LogWarning("cat_B")   << "LogWarning was used to send this other message";
  edm::LogInfo   ("cat_A")   << "LogInfo was used to send this message";
  edm::LogInfo   ("cat_B")   << "LogInfo was used to send this other message";
       LogDebug  ("cat_A")   << "LogDebug was used to send this message";
       LogDebug  ("cat_B")   << "LogDebug was used to send this other message";
}  // MessageLoggerClient::analyze()


}  // namespace edmtest


using edmtest::UnitTestClient_A;
DEFINE_FWK_MODULE(UnitTestClient_A)
