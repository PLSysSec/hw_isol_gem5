#!/bin/bash

$@
EXIT_CODE=$?

if [ "$EXIT_CODE" -eq "0" ]; then
   echo "!!!!!!!!!Failure test unexpectedly passed!!!!!!!!!";
   exit 1;
fi

exit 0
