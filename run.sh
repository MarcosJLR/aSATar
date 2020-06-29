cd ./files
rm *.png
rm *.txt
echo "Ingrese el nombre del archivo con los sudokus a resolver:"
read fileaddress
cd ..
cp $fileaddress ./files/Instances.txt
filesdir="$PWD/files"
make -C ./src/sudoku/
make -C ./src/zchaff64/
while read line; do
	cd ./src/sudoku 
	./sudsat "$filesdir/sat.txt" "$line"
	cd ..
	cd ./zchaff64
	./zchaff "$filesdir/sat.txt"
	cp ./solution.txt $filesdir
	rm ./solution.txt
	cd ..
	cd ./sudoku
	./satsud "$filesdir/solution.txt" "$filesdir/zchaff.txt"
	cd ..
	cd ..
done < $fileaddress
cd ./src/
python3 plots.py "$filesdir/zchaff.txt"
make clean -C ./zchaff64/
cd ..
