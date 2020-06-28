cd ./files
rm Instances.txt
rm sat.txt
rm sud.txt
rm solution.txt
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
	./satsud "$filesdir/solution.txt" "$filesdir/sud.txt"
	cd ..
	cd ..
done < $fileaddress
