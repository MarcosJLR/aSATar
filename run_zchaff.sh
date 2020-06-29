rm ./files/zchaff.txt
rm ./files/zchaff_sudokus/*.png
echo "Ingrese el nombre del archivo con los sudokus a resolver:"
read fileaddress
cp $fileaddress ./files/Instances.txt
filesdir="$PWD/files"
make -C ./src/sudoku/
make -C ./src/zchaff64/
while read line; do
	if [[ $line = [0-9]* ]]; then	
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
	fi
done < $fileaddress
cd ./src/
python3 plots.py "$filesdir/zchaff.txt"
make clean -C ./zchaff64/
cd ..
