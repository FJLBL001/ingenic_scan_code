#!/bin/sh
cur_dir=$(pwd)

alsa_test(){
	cd $cur_dir
	arecord -D hw:0,2 -c 2 -f S16_LE -r 8000 -d 5 /tmp/RTF_Dmic_record1.wav
	if [ $? -eq 0 ];then
		echo "record has saved successful"
		echo "Dmic_record1.wav will play"
		aplay /tmp/RTF_Dmic_record1.wav
	else
		echo "Record Failed !!"
		exit
	fi
}

tinyalsa_test(){
	cd $cur_dir
	tinycap /tmp/RTF_Dmic_record2.wav -D 0 -d 2 -c 2 -r 8000 -b 16 -t 5
	if [ $? -eq 0 ];then
		echo "record has saved successful"
		echo "Dmic_record1.wav will play"
		tinyplay /tmp/RTF_Dmic_record2.wav
	else
		echo "Record Failed !!"
		exit
	fi
}

echo "DMIC test"
cd /usr/bin
if test -e tinyplay
then
	tinyalsa_test
else
	alsa_test
fi
echo "DMIC test finish"
