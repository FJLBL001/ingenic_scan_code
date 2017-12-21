#
# this script only supprot M200(M200s) platform
#
echo "1. take picture in usertrp model!!!!"
echo "2. preview picture to fb in usertrp model!!!"
echo "please Select a label for operation......For example 1"
default_dir="."

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
	echo "please input file path, if you want use default path (.), please input y"
	if read -t 5 dir
	then
		echo ""
	else
		dir="y"
	fi
	if [ "$dir" == "y" ]; then
		dir=$default_dir
	fi
	cimutils -I $cim_num -C  -x 1280 -y 720 -f $dir/RTF_ingenic_userptr.raw -l 10 -v -T
	uyuv_to_nv12 $dir/RTF_ingenic_userptr.raw $dir/RTF_ingenic_userptr_nv12.raw
	rm $dir/RTF_ingenic_userptr.raw
	ijpege $dir/RTF_ingenic_userptr_nv12.raw 1280 720 0 1 $dir/RTF_ingenic_userptr.jpg
	;;
"2")
	cimutils -I $cim_num -P -x 1280 -y 720 -v -T
	;;
*)
	echo "please check your input"
	;;
esac
