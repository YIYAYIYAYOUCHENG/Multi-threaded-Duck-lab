################################################################################
#  duck-lab script: base_types generation (from DataInt16)
#
#  author(s): Christian Nastasi (c.nastasi@sssup.it, nastasichr@gmail.com) 
#             Claudio Salvadori (c.salvadori@sssup.it)
#  version:   1.0 
#  date:      February 2012
################################################################################
#!/bin/sh

if [ -e $1 ]; then 
	echo "You must give a name parameter"
	exit
fi

if [ -e $2 ]; then 
	echo "You must give a type parameter"
	exit
fi

if [ -e $3 ]; then 
	echo "You must give a type_name parameter"
	exit
fi

name=$1
type=$2
type_name=$3
new_h="Data$1.h"
new_cpp="Data$1.cpp"
name_up=`echo $name | tr '[:lower:]' '[:upper:]'`

echo TEST $name, $name_up,  $type, $type_name, $new_cpp, $new_h

cp DataInt16.cpp $new_cpp
cp DataInt16.h $new_h

sed "s/Int16/$name/g" $new_h > tmp
mv tmp $new_h
sed "s/Int16/$name/g" $new_cpp > tmp
mv tmp $new_cpp

sed "s/INT16/$name_up/g" $new_h > tmp
mv tmp $new_h
sed "s/INT16/$name_up/g" $new_cpp > tmp
mv tmp $new_cpp

sed "s/int16_t/$type/g" $new_h > tmp
mv tmp $new_h
sed "s/int16_t/$type/g" $new_cpp > tmp
mv tmp $new_cpp

sed "s/int16\([^_t]\)/$type_name\1/g" $new_h > tmp
mv tmp $new_h

