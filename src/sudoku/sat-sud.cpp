#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <tuple>
#include <cmath>
using namespace std;

void SATtoSudoku(string inputFileName, string outputFileName){
	string line;
	int sol=1;
	int vars=0;
	int i=-1;
	int n,var,num;
	string sudoku = "";
	ifstream inputFile(inputFileName);
	if(inputFile.is_open()){
		while(getline(inputFile,line) && sol==1 && i<vars){
			if(vars == 0 && line[0]=='s'){
				sscanf(line.c_str(),"s cnf %d %d",sol,vars);
				if(sol<1) sudoku.append(to_string(sol));
				else{
					i++;
					n = pow(vars,1/6);
					sudoku.append(to_string(n)).append(" ");
					n = n*n;
				}
			}
			else if(line[0]=='v'){
				sscanf(line.c_str(),"v %d",var);
				if(var>0){
					num = var % n;
					if(num==0) num=n;
					if(num<10) sudoku.append(to_string(num));
					else if(num>=10 && num<=35) sudoku.push_back(static_cast<char>('A'+num-10));
					else if(num==36) sudoku.push_back('.');
				}
			}
		}
		sudoku.append("\n");
	}

}

int main(){
	
}