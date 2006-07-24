#!/bin/bash

pushd $LOCAL_TMP_DIR

status=0
  
rm -f  u12_warnings.mlog u12_placeholder.mlog  

cmsRun -p $LOCAL_TEST_DIR/u12.cfg
 
for file in  u12_warnings.mlog    
do
  diff $LOCAL_TEST_DIR/unit_test_outputs/$file $LOCAL_TMP_DIR/$file  
  if [ $? -ne 0 ]  
  then
    echo The above discrepancies concern $file 
    status=1
  fi
done

for file in u12_placeholder.mlog
do
  if [ -f $file ]
  then
    echo A placeholder file was created when it should not be: $file
    status=1
  fi
done

popd

exit $status