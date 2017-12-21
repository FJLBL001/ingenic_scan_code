#!/bin/bash

# this test script need config kernel:
#	Machine selection -->
#		SOC type -->
#           auto suspend test
#
# loop test command: ./Awake-Test.sh loop


LOG_FILE=/tmp/RTF_awake_test.log

function Sleep_awake_TEST()
{
	echo "Sleep_awake_TEST --> begin"
	echo mem > /sys/power/state
	sleep 5
	dd if=/dev/urandom of=/testfile1 bs=$BS count=1 conv=fsync
	sync
	dd if=/testfile1 of=/testfile2 bs=$BS count=1 conv=fsync
	sync
	diff /testfile1 /testfile2
	if [ $? -ne 0 ];then
		echo "diff file err!!!"
		exit
	fi
	rm /testfile1 /testfile2
	echo "Sleep_awake_TEST ok"
}

#main
DATA_FREECAP=`busybox df | busybox awk '{if($6=="/") print $4}'`
if [ $DATA_FREECAP -gt 30 ];then
	BS=10
else
	echo "no space on devices!!! I need 30 byte ..."
	exit
fi
CNT=1
case $1 in
	"loop")
		echo "--------------begin loop awake-sleep test--------------" 2>&1 | tee -a $LOG_FILE
		while true
		do
			Sleep_awake_TEST
			if [ $? -eq 0 ]
			then
				echo "loop Sleep_awake_TEST $CNT ok!" 2>&1 |tee -a $LOG_FILE
				CNT=`expr $CNT + 1`
			else
				echo "loop Sleep_awake_TEST Failed!!" 2>&1 | tee -a $LOG_FILE
			fi
		done
		;;
	*)
		Sleep_awake_TEST
		;;
esac
