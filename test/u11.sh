#!/bin/bash

pushd $LOCAL_TMP_DIR

status=0
  
rm -f u11_overall_unnamed.mlog u11_overall_specific.mlog u11_supercede_specific.mlog u11_non_supercede_common.mlog u11_specific.mlog

cmsRun -p $LOCAL_TEST_DIR/u11.cfg
 
for file in u11_overall_unnamed.mlog u11_overall_specific.mlog u11_supercede_specific.mlog u11_non_supercede_common.mlog u11_specific.mlog   
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
