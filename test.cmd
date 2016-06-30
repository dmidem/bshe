md /Test
md /Dir1
md /Dir1/Dir2
md /Dir1/Dir2/NewDir
md /Dir2
md /Dir2/Dir3
mf /Dir2/Dir3/file.txt
md /Dir2/Dir4
mf /Dir2/Dir4/file.jpg
rm /Dir2/Dir4
mf /Dir2/Dir3/new_file.txt
cp /Dir2/Dir3 /Dir1_new
cp /Dir2/Dir3/file.txt /Dir2
cp /Dir2/Dir3/file.txt /Dir1/newfile.txt
