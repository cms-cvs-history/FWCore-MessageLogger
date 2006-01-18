#!/bin/bash

pushd $LOCAL_TMP_DIR

status=0
  
rm -f u4_errors.mlog u4_statistics.mlog u4_another.mlog 

cmsRun -p $LOCAL_TEST_DIR/u4.cfg
 
for file in u4_errors.mlog u4_statistics.mlog u4_another.mlog
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
