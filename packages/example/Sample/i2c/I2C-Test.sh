#!/bin/sh
echo "please input which platform you want test ."
echo "* 1. Newton2_plus pmu test."
echo "* 2. phoenix pmu test."

read answer
case $answer in
	"1")
		echo "*  read 8-bit data form register  * "
		echo ""
		i2c_sample -n 0 -s 0x47 -r 0x48
		echo ""
		echo "*  write 8-bit data form register  *"
		echo ""
		i2c_sample -n 0 -s 0x47 -w 0x48 -v 0x01
		echo ""
		echo "*  read 16-bit data form register  *"
		echo ""
		i2c_sample -n 0 -s 0x71 -R 0x0d
		echo ""
		echo "*  write 16-bit data form register  *"
		echo ""
		i2c_sample -n 0 -s 0x71 -W 0x0d -h 0x38 -l 0x8a
		echo ""
		;;
	"2")
		echo "*  read 8-bit data form register  * "
		echo ""
		i2c_sample -n 1 -s 0x32 -r 0x2c
		echo ""
		echo "*  write 8-bit data form register  *"
		i2c_sample -n 1 -s 0x32 -w 0x07 -v 0x01
		echo ""
esac
