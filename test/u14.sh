#!/bin/bash

pushd $LOCAL_TMP_DIR

status=0
  
rm -f u14_errors.mlog u14_warnings.mlog u14_infos.mlog u14_debugs.mlog u14_default.mlog u14_job_report.mxml 

cmsRun -p $LOCAL_TEST_DIR/u14.cfg
 
for file in u14_errors.mlog u14_warnings.mlog u14_infos.mlog u14_debugs.mlog u14_default.mlog u14_job_report.mxml   
do
  diff $LOCAL_TEST_DIR/unit_test_outputs/$file $LOCAL_TMP_DIR/$file  
# fancy_diffs $LOCAL_TEST_DIR/unit_test_outputs/$file $LOCAL_TMP_DIR/$file $LOCAL_TEST_DIR/$file.diffs
  if [ $? -ne 0 ]  
  then
    echo The above discrepancies concern $file 
    status=1
  fi
done

popd

exit $status
