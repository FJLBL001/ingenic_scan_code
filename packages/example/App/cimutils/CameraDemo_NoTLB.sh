#
# this script only support x1000(x1500) platform
#
echo "1. take picture in usertrp model!!!!"
echo "2. take picture in mmap model!!!!"
echo "3. preview picture to fb in usertrp model!!!"
echo "4. preview picture to fb in mmap model!!!"
echo "please Select a label for operation......For example 1"

for i in `seq -w 5 -1 0`
do
	if read -t 1  answer
	then
		break
	else
		echo -ne "\rCount down:$i s";
		# echo "Enter default Module"
		answer=1
	fi
done

[ -e '/dev/video1' ] && cim_num=1 || cim_num=0

case $answer in
"1")
	cimutils -I $cim_num -C  -x 320 -y 240 -f RTF_ingenic_userptr.jpg -l 10 -v
	;;
"2")
	cimutils -I $cim_num -C  -x 320 -y 240 -f RTF_ingenic_mmap.jpg -l 10
	;;
"3")
	cimutils -I $cim_num -P -x 320 -y 240 -v
	;;
"4")
	cimutils -I $cim_num -P -x 320 -y 240
	;;
*)
	echo "please check your input"
	;;
esac
