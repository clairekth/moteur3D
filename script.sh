#!/bin/bash
for i in {0..359}
do
    ./main $i
    convert $i.tga out/$i.png
    rm $i.tga
done
echo "PNG files generated"