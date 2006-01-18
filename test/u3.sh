#!/bin/bash

pushd $LOCAL_TMP_DIR

status=0
  
rm -f u3_infos.mlog u3_statistics.mlog  

cmsRun -p $LOCAL_TEST_DIR/u3.cfg
 
for file in u3_infos.mlog u3_statistics.mlog
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
