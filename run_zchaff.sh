if [[ "$#" -eq 0 ]]; then
	echo "Error: No se recibieron argumentos."
	exit 1
fi
rm ./files/zchaff.txt
rm ./files/zchaff_sudokus/*.png
cp "$1" ./files/Instances.txt
filesdir="$PWD/files"
make -C ./src/sudoku/
make -C ./src/zchaff64/
while read line; do
	if [[ $line = [0-9]* ]]; then	
		cd ./src/sudoku 
		./sud_sat "$filesdir/sat.txt" "$line"
		cd ..
		cd ./zchaff64
		if [[ "$#" -ne 1 ]]
		then
			./zchaff "$filesdir/sat.txt" $2
		else
			./zchaff "$filesdir/sat.txt"
		fi
		cp ./solution.txt $filesdir
		rm ./solution.txt
		cd ..
		cd ./sudoku
		./sat_sud "$filesdir/solution.txt" "$filesdir/zchaff.txt"
		cd ..
		cd ..
	fi
done < "$1"
cd ./src/
python3 plots.py "$filesdir/zchaff.txt"
make clean -C ./zchaff64/
cd ..
