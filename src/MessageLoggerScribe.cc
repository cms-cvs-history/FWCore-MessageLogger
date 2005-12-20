// ----------------------------------------------------------------------
//
// MessageLoggerScribe.cc
//
// ----------------------------------------------------------------------


#include "FWCore/MessageLogger/interface/ELoutput.h"
#include "FWCore/MessageLogger/interface/ErrorObj.h"
#include "FWCore/MessageLogger/interface/MessageLoggerQ.h"
#include "FWCore/MessageLogger/interface/MessageLoggerScribe.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using namespace edm;


MessageLoggerScribe::MessageLoggerScribe()
: admin_p   ( ELadministrator::instance() )
, early_dest( admin_p->attach(ELoutput(std::cerr, false)) )
, errorlog_p( new ErrorLog() )
, file_ps   ( )
{ }


MessageLoggerScribe::~MessageLoggerScribe()
{
  delete errorlog_p;
  for( ;  not file_ps.empty();  file_ps.pop_back() )  {
    delete file_ps.back();
  }
}


void
  MessageLoggerScribe::run()
{
  MessageLoggerQ::OpCode  opcode;
  void *                  operand;

  bool  done = false;
  do  {
    MessageLoggerQ::consume(opcode, operand);  // grab next work item from Q
    switch(opcode)  {  // interpret the work item
      default:  {
        assert(false);  // can't happen (we certainly hope!)
        break;
      }
      case MessageLoggerQ::END_THREAD:  {
        assert( operand == 0 );
        done = true;
        break;
      }
      case MessageLoggerQ::LOG_A_MESSAGE:  {
        ErrorObj *  errorobj_p = static_cast<ErrorObj *>(operand);
        //std::cout << "MessageLoggerQ::LOG_A_MESSAGE " << errorobj_p << '\n';
        (*errorlog_p)( *errorobj_p );  // route the message text
        delete errorobj_p;  // dispose of the message text
        break;
      }
      case MessageLoggerQ::CONFIGURE:  {
        ParameterSet *  pset_p = static_cast<ParameterSet *>(operand);
        configure_errorlog( pset_p );
        delete pset_p;  // dispose of our (copy of the) ParameterSet
        break;
      }
      case MessageLoggerQ::EXTERN_DEST: {
        ELdestination *  dest_p = static_cast<ELdestination *>(operand);
        admin_p->attach( *dest_p );
        delete dest_p;  // dispose of our (copy of the) dest
        break;
	
      }
    }  // switch

  } while(! done);

}  // MessageLoggerScribe::run()

void
  MessageLoggerScribe::configure_errorlog( ParameterSet const * p )
{
  typedef std::string          String;
  typedef std::vector<String>  vString;
  typedef ParameterSet         PSet;

  vString  empty_vString;
  PSet     empty_PSet;
  String   empty_String;

  char *  severity_array[] = {"WARNING", "INFO", "ERROR", "DEBUG"};
  vString const  severities(severity_array+0, severity_array+4);

  // grab list of categories
  vString  categories
     = p->getUntrackedParameter<vString>("categories", empty_vString);

  // grab list of messageIDs -- these are a synonym for categories
  // Note -- the use of messageIDs is deprecated in favor of categories
  vString  messageIDs
     = p->getUntrackedParameter<vString>("messageIDs", empty_vString);

  // grab default limit/timespan common to all destinations/categories:
  PSet  default_pset
     = p->getUntrackedParameter<PSet>("default", empty_PSet);
  int  default_limit
    = default_pset.getUntrackedParameter<int>("limit", -1);
  int  default_timespan
    = default_pset.getUntrackedParameter<int>("timespan", -1);

  // grab list of destinations:
  vString  destinations
     = p->getUntrackedParameter<vString>("destinations", empty_vString);

  // dial down the early destination if other dest's are supplied:
  if( ! destinations.empty() )
    early_dest.setThreshold(ELhighestSeverity);

  // establish each destination:
  for( vString::const_iterator it = destinations.begin()
     ; it != destinations.end()
     ; ++it
     )
  {
    // attach the current destination, keeping a control handle to it:
    ELdestControl dest_ctrl;
    String filename = *it;
    if( filename == "cout" )  {
      dest_ctrl = admin_p->attach( ELoutput(std::cout) );
    }
    else if( filename == "cerr" )  {
      early_dest.setThreshold(ELzeroSeverity);  // or ELerror?
      dest_ctrl = early_dest;
    }
    else  {
      std::ofstream * os_p = new std::ofstream(filename.c_str());
      file_ps.push_back(os_p);
      dest_ctrl = admin_p->attach( ELoutput(*os_p) );
    }
    //(*errorlog_p)( ELinfo, "added_dest") << filename << endmsg;

    // grab all of this destination's parameters:
    PSet  dest_pset = p->getUntrackedParameter<PSet>(filename,empty_PSet);

    // grab this destination's default limit/timespan:
    PSet  dest_default_pset
       = dest_pset.getUntrackedParameter<PSet>("default", empty_PSet);
    int  dest_default_limit
      = dest_default_pset.getUntrackedParameter<int>("limit", default_limit);
    int  dest_default_timespan
      = dest_default_pset.getUntrackedParameter<int>("timespan", default_timespan);

    // establish this destination's limit/timespan for each of the categories:
    for( vString::const_iterator id_it = categories.begin()
       ; id_it != categories.end()
       ; ++id_it
       )
    {
      String  msgID = *id_it;
      PSet  messageIDpset
         = dest_pset.getUntrackedParameter<PSet>(msgID, empty_PSet);
      int  limit
        = messageIDpset.getUntrackedParameter<int>("limit", dest_default_limit);
      int  timespan
        = messageIDpset.getUntrackedParameter<int>("timespan", dest_default_timespan);
      if( limit    >= 0 )  dest_ctrl.setLimit(msgID, limit   );
      if( timespan >= 0 )  dest_ctrl.setLimit(msgID, timespan);
    }  // for

    // establish this destination's limit/timespan for each of the messageIDs:
    // Note -- the use of messageIDs is deprecated in favor of categories
    for( vString::const_iterator id_it = messageIDs.begin()
       ; id_it != messageIDs.end()
       ; ++id_it
       )
    {
      String  msgID = *id_it;
      PSet  messageIDpset
         = dest_pset.getUntrackedParameter<PSet>(msgID, empty_PSet);
      int  limit
        = messageIDpset.getUntrackedParameter<int>("limit", dest_default_limit);
      int  timespan
        = messageIDpset.getUntrackedParameter<int>("timespan", dest_default_timespan);
      if( limit    >= 0 )  dest_ctrl.setLimit(msgID, limit   );
      if( timespan >= 0 )  dest_ctrl.setLimit(msgID, timespan);
    }  // for

    // establish this destination's threshold:
    String  severity_name
      = dest_pset.getUntrackedParameter<String>("threshold", empty_String);
    ELseverityLevel  lev = ELseverityLevel(severity_name);
    if( lev != ELunspecified )
      dest_ctrl.setThreshold(lev);

    // establish this destination's limit for each severity:
    for( vString::const_iterator sev_it = severities.begin()
       ; sev_it != severities.end()
       ; ++sev_it
       )
    {
      String  sevID = *sev_it;
      ELseverityLevel  severity(sevID);
      PSet  sev_pset
         = dest_pset.getUntrackedParameter<PSet>(sevID, empty_PSet);
      int  limit
        = sev_pset.getUntrackedParameter<int>("limit", -1);
      int  timespan
        = sev_pset.getUntrackedParameter<int>("timespan", -1);
      if( limit    >= 0 )  dest_ctrl.setLimit(severity, limit   );
      if( timespan >= 0 )  dest_ctrl.setLimit(severity, timespan);
    }  // for

    // establish this destination's linebreak policy:
    bool noLineBreaks = 
            dest_pset.getUntrackedParameter<bool> ("noLineBreaks",false);
    if (noLineBreaks) {
      dest_ctrl.setLineLength(32000);
    } else {
      int  lenDef = 80; 
      int  lineLen = 
            dest_pset.getUntrackedParameter<int> ("lineLength",lenDef);
      if (lineLen != lenDef) {
        dest_ctrl.setLineLength(lineLen);
      }  
    }

  }  // for [it = destinations.begin() to end()]

}  // MessageLoggerScribe::configure()
