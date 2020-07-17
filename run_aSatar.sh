if [[ "$#" -eq 0 ]]; then
	echo "Error: No se recibieron argumentos."
	exit 1
fi
rm ./files/aSatar.txt
rm ./files/aSatar_sudokus/*.png
cp "$1" ./files/Instances.txt
filesdir="$PWD/files"
make -C ./src/sudoku/
make -C ./src/solver/
while read line; do
	if [[ $line = [0-9]* ]]; then
		cd ./src/sudoku 
		./sud_sat "$filesdir/sat.txt" "$line"
		cd ..
		cd ./solver
		if [[ "$#" -ne 1 ]]
		then
			./solver "$filesdir/sat.txt" $2
		else
			./solver "$filesdir/sat.txt"
		fi
		cp ./solution.txt $filesdir
		rm ./solution.txt
		cd ..
		cd ./sudoku
		./sat_sud "$filesdir/solution.txt" "$filesdir/aSatar.txt"
		cd ..
		cd ..
	fi
done < "$1" 
cd ./src/
python3 plots.py "$filesdir/aSatar.txt"
make clean -C ./solver/
cd ..
