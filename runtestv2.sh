#/bin/bash

FILENAME="./path_list.txt"

SAVEDIR="/home/ubuntu/Pictures/predictions"
SPACE=" -out "
rm arguments_file.txt
touch arguments_file.txt
while read line
do
    input_file=$line
    subpath=${line##*prepared/}
    dir=${SAVEDIR}
    filename=${subpath##*/}
    output_file=${dir}"/"${filename} 
    echo $output_file
    echo ${input_file}${SPACE}${output_file%.*} >> arguments_file.txt
done < $FILENAME
cat  arguments_file.txt | xargs -n 3 ./darknet detector test cfg/voc.data cfg/tiny-yolo-voc.cfg tiny-yolo-voc.weights
