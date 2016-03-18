#!/bin/bash
#create videos file
# $1 = dir $2 = name $3 = original file
TIME=4
TOTAL=15

cd $1

# for i in `seq 0 1`;
# do
# 	T=$(($i*$TIME))
# 	T2=$((($i+1)*$TIME))
# 	B=$(($i % 2))
# 	echo "$2 00:00:$T 00:00:$T2 $T $B" >> videos
# done

mkdir tmp

# split video
# while read vidtime to num bottom; do 
# # while read -r video vidtime to num bottom; do 
# 	B=$([ $bottom == 0 ] && echo '0' || echo 'ih/2')
# 	# file=tmp/$num.webm
# 	# file=$(printf '%q' "tmp/$num.webm")
# 	ffmpeg -loglevel 24 -ss $vidtime -i $2.webm -to $to -vf "crop=iw:ih/2:0:$B" tmp/$num.webm;

# done < videos.txt; 

ffmpeg -loglevel 24 -ss 00:00:00 -i $2.webm -to 00:00:03 -speed 8 -vf "crop=iw/2:ih/2:0:0" tmp/1.webm;
ffmpeg -loglevel 24 -ss 00:00:03 -i $2.webm -to 00:00:06 -speed 8 -vf "crop=iw/2:ih/2:0:ih/2" tmp/2.webm;
ffmpeg -loglevel 24 -ss 00:00:06 -i $2.webm -to 00:00:09 -speed 8 -vf "crop=iw/2:ih/2:0:0" tmp/3.webm;
ffmpeg -loglevel 24 -ss 00:00:09 -i $2.webm -to 00:00:12 -speed 8 -vf "crop=iw/2:ih/2:0:ih/2" tmp/4.webm;
# ffmpeg -loglevel 24 -ss 00:00:12 -i $2.webm -to 00:00:15 -speed 8 -vf "crop=iw:ih/2:0:0" tmp/5.webm;

# bring it all back 2gether
ffmpeg -loglevel 24 -f concat -i <(for f in tmp/*.webm; do echo "file '$PWD/$f'"; done) $2_cut.webm

echo $3

# add in audio
ffmpeg -loglevel 24 -i $2_cut.webm -i $3 -c copy -map 0:0 -map 1:1 -shortest $2_merged.webm

# # kill everything
# rm videos
rm -r tmp