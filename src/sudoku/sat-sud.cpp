#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <tuple>
using namespace std;

int power(int base, int exp){
	if(exp==0) return 1;
	else if(exp==1) return base;
	else if(exp & 1){
		int hpower = power(base,exp/2);
		return base*hpower*hpower;
	}
	else{
		int hpower = power(base,exp/2);
		return hpower*hpower;	
	}
}

void SATtoSudoku(string& inputFileName, string& outputFileName){
	string line;
	int sol;
	int vars;
	int n,var,num;
	float time=-1.0;
	string sudoku = "";
	ifstream inputFile(inputFileName);
	if(inputFile.is_open()){
		while(getline(inputFile,line)){
			if(line[0]=='s'){
				sscanf(line.c_str(),"s cnf %i %i",&sol,&vars);
				if(sol<1) sudoku.append(to_string(sol));
				else{
					n=0;
					for(int i=1;i<6 && n==0;i++){
						if(power(i,6)==vars){
							n = i;
							sudoku.append(to_string(n)).append(" ");
							n = n*n;
						}
					}
				}
			}
			else if(line[0]=='v'){
				sscanf(line.c_str(),"v %d",&var);
				if(var>0){
					num = var % n;
					if(num==0) num=n;
					if(num<10) sudoku.append(to_string(num));
					else if(num>=10 && num<=35) sudoku.push_back(static_cast<char>('A'+num-10));
					else if(num==36) sudoku.push_back('.');
				}
			}
			else if(line[0]=='c'){
				sscanf(line.c_str(),"c rt %f",&time);
			}
		}
		if(time!=-1.0) sudoku.append(" ").append(to_string(time));
		sudoku.append("\n");
		inputFile.close();
		ofstream outputFile(outputFileName, ios::app);
		if(outputFile.is_open()){
			outputFile << sudoku;
			outputFile.close();
		}
		else cout << "Couldn't open output file.\n";
	}
	else cout << "Couldn't open input file.\n";

}

int main(int argc, char *argv[]){
	string iFN = argv[1];
	string oFN = argv[2];
	SATtoSudoku(iFN,oFN);
}