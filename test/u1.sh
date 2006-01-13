#!/bin/bash

pushd $LOCAL_TMP_DIR

status=0
  
rm -f u1_errors.mlog u1_warnings.mlog u1_infos.mlog u1_debugs.mlog u1_default.mlog u1_job_report.mxml 

cmsRun -p $LOCAL_TEST_DIR/u1.cfg
 
for file in u1_errors.mlog u1_warnings.mlog u1_infos.mlog u1_debugs.mlog u1_default.mlog u1_job_report.mxml   
do
  diff $LOCAL_TEST_DIR/unit_test_outputs/$file $LOCAL_TMP_DIR/$file  
  if [ $? -ne 0 ]  
  then
    echo The above discrepancies concern $file 
    status=1
  fi
done

popd

exit $status
