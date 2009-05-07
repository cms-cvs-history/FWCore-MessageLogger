# timingFilteredMLexample -- 
#                  an example Configuration file for MessageLogger service:

# Illustrates how to limit the output of a category of message to one
# destination but not to another.
#
# This example sets up logging to a file and to cout, and establishes the
# timing service.  The idea is to deliver all messages created by LogWarning 
# and above to cout (as would happen in usual production jobs) EXCEPT to
# suppress the timing messages for cout (but not for the other log file).
#
# This sort of setup was requested by Florian Beaudette, Jean-Roch, Peter Elmer
# and for a time was rendered impossible because the timing service had used
# LogAbsolute for its output; that has since been modified.
#
# cmsRun timingFilteredMLexample_cfg.py outputs to cout, and also produces 
# timingFilteredMLexample.log.  The output to cout should not contain per-event
# timing information.

import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

import FWCore.Framework.test.cmsExceptionsFatal_cff
process.options = FWCore.Framework.test.cmsExceptionsFatal_cff.options

process.load("FWCore.MessageService.test.Services_cff")

# Here is where the timng service is enabled

process.Timing =  cms.Service("Timing")

# Here is the configuration of the MessgeLogger Service:

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring( 'cout' 
					, 'timingFilteredML'
					) ,
    categories   = cms.untracked.vstring( 'TimeEvent'
                                        , 'Time Module'
					, 'TimeReport' 
					, 'cat_B'
					) ,
    cout = cms.untracked.PSet(
        threshold  = cms.untracked.string('WARNING'),
        # We are about to impose a limit of 0 (meaning do no react to these)
        # on each of the message categories generated by the timing service.
        # Since this is inside the PSet for cout, these limits apply only to 
        # the cout destination.
        TimeEvent  = cms.untracked.PSet(
             limit = cms.untracked.int32(0)
        ),
        TimeModule = cms.untracked.PSet(
             limit = cms.untracked.int32(0)
        ),
        TimeReport = cms.untracked.PSet(
             limit = cms.untracked.int32(0)
        ),
        cat_B = cms.untracked.PSet(
             limit = cms.untracked.int32(0)
        )
    ),
    timingFilteredML = cms.untracked.PSet(
        threshold = cms.untracked.string('WARNING'),
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5)
)

process.source = cms.Source("EmptySource")

process.sendSomeMessages = cms.EDAnalyzer("MLexampleModule_1")

process.p = cms.Path(process.sendSomeMessages)
