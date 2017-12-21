#!/bin/sh

cur_dir=$(pwd)

alsa_test(){
	amixer cset numid=17,iface=MIXER,name='ADC Mux'  0 # you can change this parameter #
	amixer cset numid=4,iface=MIXER,name='Digital Capture Volume' 20  # you can change this parameter #
	amixer cset numid=6,iface=MIXER,name='Mic Volume' 3  # you can change this parameter #

	cd $cur_dir
	arecord -D hw:0,0 -c 2 -f S16_LE -r 44100 -d 5 /tmp/RTF_Amic_record1.wav # this commad will creat a 10s and 44100Hz Amic_record.wav file #
	if [ $? -eq 0 ];then
		echo "record has saved successful"
		echo "Amic_record1.wav will play"
		aplay /tmp/RTF_Amic_record1.wav
	else
		echo "Record Failed !!"
		exit
	fi
}

tinyalsa_test(){

	cd $cur_dir
	tinycap /tmp/RTF_Amic_record2.wav -D 0 -d 0 -c 2 -r 8000 -b 16 -t 5
	if [ $? -eq 0 ];then
		echo "record has saved successful"
		echo "Amic_record2.wav will play"
		tinyplay /tmp/RTF_Amic_record2.wav
	else
		echo "Record Failed!!"
		exit
	fi
}


cd /usr/bin
if test -e tinyplay
then
	tinyalsa_test
else
	alsa_test
fi
echo "AMIC test finish"
