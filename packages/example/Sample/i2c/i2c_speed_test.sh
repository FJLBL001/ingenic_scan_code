#bin/sh


TEST_LOG=test.log


output_env()
{
	echo
	echo "*******************************************************" 2>&1 |tee -a $TEST_LOG
	echo "TEST DATE: $(date)" 2>&1| tee -a $TEST_LOG
	echo "***************** TEST ENVIRONMENT ********************" 2>&1 |tee -a $TEST_LOG
	cat /proc/jz/clock/clocks |grep cclk 2>&1 |tee -a $TEST_LOG
	cat /proc/jz/clock/clocks |grep l2clk 2>&1 |tee -a $TEST_LOG
	cat /proc/jz/clock/clocks |grep h0clk 2>&1 |tee -a $TEST_LOG
	cat /proc/jz/clock/clocks |grep h2clk 2>&1 |tee -a $TEST_LOG
	cat /proc/jz/clock/clocks |grep pclk | head -1 2>&1 |tee -a $TEST_LOG
	cat /proc/jz/clock/clocks |grep cgu_msc0 2>&1 |tee -a $TEST_LOG
	echo "*******************************************************" 2>&1 |tee -a $TEST_LOG
}

output_env
sleep 1
# use eeprom_speed_test read 16kbit data form eeprom
SPEED_TIME=$( eeprom_speed_test -n 1 -a 0x50 -b 2048  |  awk 'NR==7{print $1}')
echo $SPEED_TIME
SPEED_TIME=${SPEED_TIME#*:}
SPEED=$(awk 'BEGIN{printf "%0.3f\n",'16'/('$SPEED_TIME'/'1000000000')}')

echo "I2C chansfer speed is : $SPEED Kbit/s" | tee -a $TEST_LOG
