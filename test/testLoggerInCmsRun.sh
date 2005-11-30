#!/bin/sh

# Pass in name and status
function die { echo $1: status $2 ;  exit $2; }

cmsRun --parameter-set ${LOCAL_TEST_DIR}/messageLogger.cfg

