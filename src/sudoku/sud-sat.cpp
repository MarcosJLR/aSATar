#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <tuple>
using namespace std;

string baseSudoku (int order,int length, int size, int nVars) {
	string sat = "";
	string completeness = "";
	int i,j,k,d,x,y,z;
	string s = to_string(nVars);
	int l = s.length();
	char line[10+2*l];
	for(i=0; i<nVars; i+=length){ 
		for(j=1; j<=length; j++){
			// Uniqueness
			for(k=j+1; k<=length; k++){
				sprintf(line, "-%i -%i 0\n",i+j,i+k);
				sat.append(line);
			}
			// Completeness
			completeness.append(to_string(i+j)).append(" ");
		}
		if(i!=nVars-length) completeness.append("0\n");
		else completeness.append("\n");
	}

	for(i=0;i<nVars;i+=size){
		for(j=0;j<size;j+=length){
			// Valid Row
			for(k=j+length;k<size;k+=length){
				for(d=1; d<=length; d++){
					sprintf(line, "-%i -%i 0\n",i+j+d,i+k+d);
					sat.append(line);
				}
			}
			// Valid Column
			for(k=i+size;k<nVars;k+=size){
				for(d=1; d<=length; d++){
					sprintf(line, "-%i -%i 0\n",i+j+d,j+k+d);
					sat.append(line);
				}
			}
		}
	}
	// Valid Square
	for(i=0;i<length;i++){
		x = length*(i%order)*order+size*(i/order)*order;
		for(j=0;j<length;j++){
			y = length*(j%order)+size*(j/order);
			for(k=j+1;k<length;k++){
				z = length*(k%order)+size*(k/order);
				for(d=1; d<=length; d++){
					sprintf(line, "-%i -%i 0\n",x+y+d,x+z+d);
					sat.append(line);
				}
			}
		}
	}
	sat.append(completeness);
	return sat;
}

tuple <int,string> filledPlaces (int length, int size, char* cells) {
	int i,nClauses,var;
	nClauses=0;
	string sat="";
	for(i=0;i<size;i++){
		if(cells[i]!='0'){
			if(cells[i]>'0' && cells[i]<='9'){
				var=i*length+cells[i]-'0';
			}
			else if(cells[i]>='A' && cells[i]<='Z'){
				var=i*length+cells[i]-'A'+10;
			}
			else if(cells[i]=='.'){
				var=i*length+36;
			}
			sat.append(to_string(var)).append(" 0\n");
			nClauses++;
		}
	}
	return {nClauses,sat};
}

void sudokuToSAT (string sudoku) {
	int order;
	sscanf(sudoku.c_str(),"%i",&order);
	int length = order*order; //N^2
	int size = length*length; //N^4
	int nVars = length*size; //N^6
	char cells[size];
	sscanf(sudoku.c_str(),"%*i %s",cells);
	string sat="p cnf ";
	sat.append(to_string(nVars)).append(" ");
	string baseSudSat = baseSudoku(order,length,size,nVars);
	auto [nClauses, filledVarsSat] = filledPlaces(length,size,cells);
	nClauses += size + 2*(length-1)*nVars;
	sat.append(to_string(nClauses)).append("\n");
	sat.append(filledVarsSat);
	sat.append(baseSudSat);
	ofstream file ("sat.txt");
	if(file.is_open()){
		file << sat;
		file.close();
	}
	else cout << "Unable to open file.\n";
}

int main (int argc, char *argv[]) {
	string sudoku = argv[1];
	sudoku.append(argv[2]);
	sudokuToSAT(sudoku);
	return 0;
}
