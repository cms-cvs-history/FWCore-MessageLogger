#include "FWCore/MessageLogger/test/MessageLoggerClient.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <iostream>


namespace edmreftest
{


void
  MessageLoggerClient::analyze( edm::Event      const & e
                              , edm::EventSetup const & /*unused*/
                              )
{
  //std::cout << "Module reached\n";
  LogDebug("aTestMessage") << "LogDebug was used to send this message";
  edm::LogInfo("aTestMessage") << "LogInfo was used to send this message";
  edm::LogWarning("aTestMessage") << "LogWarning was used to send this message";
  edm::LogError("aTestMessage") << "LogError was used to send this message";

  edm::LogWarning("aboutToSend") << "about to send 100 warnings" << i;
  for( unsigned i = 0;  i != 100;  ++i )  {
    edm::LogWarning("unimportant") << "warning number " << i;
  }


}  // MessageLoggerClient::analyze()


}  // namespace edmreftest


using edmreftest::MessageLoggerClient;
DEFINE_FWK_MODULE(MessageLoggerClient)
