//V1: Currrently reading LINA data into arrays correctly, but if statement for outputting parameters at blue edge not working.
	//Note: if statement worked when using 2D arrays (reading all of LINA data) and inside of NumOfModes loop..however outputted 10 times..did not work outside of loop.
	//Note: From testing problem seems to be with outFile not the if statement? outFile worked when right before external loop test
	//Note: WORKAROUND - works when no header is added to outputfile. Why? Who knows.. Working program!

# include <iostream>
# include <fstream>
# include <math.h>
# include <iomanip>
# include <cmath>
# include <stdlib.h>
# include <cstdlib>
//# include <fstream.h>
# include <string.h>
# include <string>
//# include <dos.h> //For Sleep() 


using namespace std;
int main() {
	ifstream inFile;
	
	//Controls//
	
	int NumOfModels = 162; //20412 is total TypeII models
	int NumOfModes = 10;
	int NumOfSpaces = 1; // Number of spaces between each models output 
	
	char inputFileName[30] = "InputDataFULL.dat"; //File with the model parameters
	char outputFileName[30] = "BlueEdgeTest.dat"; //File with organized output
	char LINAFileName[30] = "LINA_period_growth.data"; //Normally should be "LINA_period_growth.data"
	char log_directory_prefix[30] = "Raw_LINA_data/SetA/LOGS_"; //Prefix to log_directory, suffix is model number. This is where LINA file should be
	
	///////////////////////////////
	
	//Arrays//
	
	const int nArray = NumOfModels + 1;
	const int nArray_modes = NumOfModes + 1;
	
	double Mod[nArray];
	double z[nArray];
	double x[nArray];
	double M[nArray];
	double L[nArray];
	double T[nArray];
	
	double L_Blue[nArray];
	double T_Blue[nArray];
	
	double mode[nArray];
	double period[nArray];
	double growth[nArray];
	
	/////////////////////////////////////////
	
	//Reading input parameters from file into arrays//
	
	inFile.open(inputFileName,ios::in);
	
	// Warning if file cant be opened
	if(!inFile.is_open()){ 
		cout << "Error opening file. \n";
		//cout << "Giving Retry... \n";
	}
	if(inFile.is_open()){
		cout<<"Input File was opened successfully"<<endl;
	}
	if(inFile.good()){
		cout<<"Input File is ready for reading"<<endl;
	}
	
	
	//reading file
	if(inFile.is_open()){
	
		// Putting cursor at start of file
		inFile.clear();
		
		//Reading first line - This is needed outside loop otherwise array won't fill.
		inFile >> Mod[0] >> z[0] >> x[0] >> M[0] >> L[0] >> T[0];
		cout<<"Model"<<setw(10)<<"z"<<setw(15)<<"x"<<setw(15)<<"M"<<setw(15)<<"L"<<setw(15)<<"T"<<endl;
		cout<<Mod[0]<<setw(15)<<z[0]<<setw(15)<<x[0]<<setw(15)<<M[0]<<setw(15)<<L[0]<<setw(15)<<T[0]<<endl;
		for (int a = 1; a < NumOfModels; a++){
			
			// Reading rest of file
			inFile >> Mod[a] >> z[a] >> x[a] >> M[a] >> L[a] >> T[a];
			//cout<<Mod[a]<<setw(15)<<z[a]<<setw(15)<<x[a]<<setw(15)<<M[a]<<setw(15)<<L[a]<<setw(15)<<T[a]<<endl;
				
		}
		
		//To show last and first index only, have this line uncommmented and the cout line in loop commented
		cout<<Mod[NumOfModels-1]<<setw(15)<<z[NumOfModels-1]<<setw(15)<<x[NumOfModels-1]<<setw(15)<<M[NumOfModels-1]<<setw(15)<<L[NumOfModels-1]<<setw(15)<<T[NumOfModels-1]<<endl;
		
		// Close the file.
		inFile.close();
	}
			
			
	if(!inFile.is_open()){
		cout<<"Input File closed successfully"<<endl;
	}
	
	////////////////////////////////////////
	
	cout<<"Creating output file"<<endl;
	ofstream outFile(outputFileName);
	//outFile<<"Model"<<setw(10)<<"z"<<setw(15)<<"x"<<setw(15)<<"M"<<setw(15)<<"L"<<setw(15)<<"T"<<endl; //Header
	//outFile<<""<<endl;
	
	////////////////////////////////////
	
	
	//Start of outside loop, steping through input data
	int LINAi = 0;
	int NumOfFailedModels = 0;
	for(int i = 0; i < NumOfModels; i++){
		
		/////Reading LINA file////////
		
		//Getting model number into string
		stringstream stream;
			stream<<Mod[i];
			string Model = stream.str();
			//cout<<"Current Model in string form: "<<Model<<endl;
		
		string LINA_header; //Varible for header of LINA file
		string readout; //String thats viewing each line
		
		char LINAFilePath[50]; //log_directory_prefix + Model + "/" + LINAFileName;
		strcpy(LINAFilePath, log_directory_prefix);
		strcat(LINAFilePath, Model.c_str());
		strcat(LINAFilePath, "/");
		strcat(LINAFilePath, LINAFileName);
		cout<<"Path to LINA file: "<<LINAFilePath<<endl;
		
		inFile.open(LINAFilePath,ios::in);
	
		// Warning if file cant be opened
		if(!inFile.is_open()){ 
			cout << "Error opening LINA file. Model did not run. \n";
			NumOfFailedModels = NumOfFailedModels + 1;
		}
		if(inFile.is_open()){
			cout<<"LINA File was opened successfully"<<endl;
		}
		
		
		if(inFile.good()){
			cout<<"LINA File is being read..."<<endl;
		
			//loop that reads LINA file into temp arrays, saving L and T of input file if Blue Edge 
			getline(inFile,LINA_header);
			cout<<LINA_header<<endl;
			inFile>>mode[LINAi]>>period[LINAi]>>growth[LINAi];
			cout<<setw(3)<<mode[LINAi]<<setw(17)<<period[LINAi]<<setw(17)<<growth[LINAi]<<endl;
			
			
			// for(int l = 1; l < NumOfModes; l++){
				
				// inFile>>mode[i]>>period[i]>>growth[i];
				
				// cout<<setw(3)<<mode[i]<<setw(17)<<period[i]<<setw(17)<<growth[i]<<endl;
			
				
			// }
			
			

			//if f0 is negative and last was positive, then output model parameters and assign T and L to blue edge arrays
			if(LINAi != 0){
				if(growth[LINAi] < 0. && growth[LINAi-1] > 0.){
					outFile<<Mod[i]<<setw(15)<<z[i]<<setw(15)<<x[i]<<setw(15)<<M[i]<<setw(15)<<L[i]<<setw(15)<<T[i]<<endl;
					
					L_Blue[i] = L[i];
					T_Blue[i] = T[i];
				}
			}
			
			LINAi = LINAi + 1; //Incrementing LINAi
			
			//Space loop - puts space between each model
			// for(int s = 0; s < NumOfSpaces; s++){
				// outFile<<""<<endl;
			// }
			
			inFile.close();
			
			if(!inFile.is_open()){
			cout<<"LINA File closed successfully"<<endl;
			}
	
		}
		
	}
	
	////////////////////////////////////
	
	// cout<<"Creating output file"<<endl;
	// ofstream outFile(outputFileName);
	// outFile<<"Model"<<setw(10)<<"z"<<setw(15)<<"x"<<setw(15)<<"M"<<setw(15)<<"L"<<setw(15)<<"T"<<endl; //Header
	// outFile<<""<<endl;
	
	////////////////////////////////////
	
	///Test loop for LINA data 
	// int LINAi_max = LINAi;
	// for(int l = 0; l < LINAi_max; l++){
		// cout<<setw(3)<<mode[l]<<setw(17)<<period[l]<<setw(17)<<growth[l]<<endl;
		
		// if(l != 0){
			// if(growth[l-1] > 0 && growth[l] < 0){
				// outFile<<Mod[i]<<setw(15)<<z[i]<<setw(15)<<x[i]<<setw(15)<<M[i]<<setw(15)<<L[i]<<setw(15)<<T[i]<<endl;
				
				// L_Blue[i] = L[i];
				// T_Blue[i] = T[i];
			// }
		// }
	// }
	
	
	return 0;
}