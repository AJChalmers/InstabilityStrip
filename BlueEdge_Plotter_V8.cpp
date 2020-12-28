//V1: Currrently reading LINA data into arrays correctly, but if statement for outputting parameters at blue edge not working.
	//Note: if statement worked when using 2D arrays (reading all of LINA data) and inside of NumOfModes loop..however outputted 10 times..did not work outside of loop.
	//Note: From testing problem seems to be with outFile not the if statement? outFile worked when right before external loop test
	//Note: WORKAROUND - works when no header is added to outputfile. Why? Who knows.. Working program!
	
//V2: Goal is to add graphics to program so HR diagram of blue edge can be plotted.
	//Note: Graphics not working, Root isnt finding the points? Fixed this! Problem was in selectors 
	//Note: Root cant have x-axis go from highest to lowest? Maybe we dont use root?
	
//V3: Giving up on root graphics, this is V2 but without them.

//V4: Adding option to get all positive growth rate models so they can be plotted. 
	//Note: Reorganize program so the outside loop doesnt have to be done twice when getting positive growth and blue edge? Using more selectors inside instead of one outside?

//V5: Fixing problem with V4 taking too long to run using only one main loop.

//V6: Adding 1<P<4 function for logT = a + blogP + clogM fits. In future make program to find this fit?

//V7: Adding ablitiy to print out logT, logP and logM for fit. In future create functions for different uses?

//V8: Attempt to add FO and 2O arrays. Working program. 

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
int main(){
	ifstream inFile;
	
	//Controls//
	
	int NumOfModels = 20412; //20412 is total TypeII models
	int NumOfModes = 10; // Number of modes in the model
	
	char inputFileName[30] = "InputDataFULL.dat"; //File with the model parameters
	char outputFileName_ForBlueEdge[50] = "BlueEdge_SetB.dat"; //File with model parameters on the blue edge. Will have prefix for mode that is defined below.
	char outputFileName_ForPositiveGrowthModels[50] = "PostiveGrowth_SetB.dat"; //File with model parameters that have positve growth rates. Will have prefix for mode that is defined below.
	
	char log_directory_prefix[30] = "Raw_LINA_data/SetB/LOGS/LOGS_"; //Prefix to log_directory, suffix is model number. This is where LINA file should be
	char LINAFileName[30] = "LINA_period_growth.data"; //Normally should be "LINA_period_growth.data"
	
	char FU_OutputFilePrefix[10] = "FU_"; //Prefix to output files that contain data for FU mode
	char FO_OutputFilePrefix[10] = "FO_"; //Prefix to output files that contain data for FO mode
	char SO_OutputFilePrefix[10] = "2O_"; //Prefix to output files that contain data for 2O mode
	
	bool GetPositiveGrowthRateModels = true; // Positive Growth. When true, postive growth rate models will be outputted to a file.
	bool GetFirstNegativeGrowthRateModels = true; // Blue Edge. When true, the first negative growth rate models will be outputted to a file (For blue edge!)
	bool GetOnlyModelsWithDefinedPeriodRange = false; // Period Range. When true, gets only models within the defined period range.
	
	double Lower_Period_Boundry = 1; //Low bound to period range in days. Only is used when GetOnlyModelsWithDefinedPeriodRange is true 
	double Higher_Period_Boundry = 4; //High bound to period range in days. Only is used when GetOnlyModelsWithDefinedPeriodRange is true
	
	///////////////////////////////
	
	//Variables//
	
	//chars that store the FULL output file names.
	char FU_outputFileName_ForBlueEdge[50];
	char FO_outputFileName_ForBlueEdge[50];
	char SO_outputFileName_ForBlueEdge[50];
	
	char FU_outputFileName_ForPositiveGrowthModels[50];
	char FO_outputFileName_ForPositiveGrowthModels[50];
	char SO_outputFileName_ForPositiveGrowthModels[50];
	
	int BlueEdgei;
	int NumOfBlueEdgePoints;
	
	int NumOfFailedModels;
	
	int PosGrowthi;
	int NumOfPosGrowthPoints;
	
	//Arrays//
	
	const int nArray = NumOfModels + 1;
	const int nArray_Total = nArray*3; //Total possible model parameters to be outputted. Should be Number of models times modes being looked at
	
	double Mod[nArray];
	double z[nArray];
	double x[nArray];
	double M[nArray];
	double L[nArray];
	double T[nArray];
	
	double L_Blue[nArray];
	double T_Blue[nArray];
	
	double mode_FU[nArray];
	double period_FU[nArray];
	double growth_FU[nArray];
	
	double mode_FO[nArray];
	double period_FO[nArray];
	double growth_FO[nArray];
	
	double mode_2O[nArray];
	double period_2O[nArray];
	double growth_2O[nArray];
	
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
	
	///////////////////////////////////////////////////////////////
	
	//Main program//
		
	////////////////////////////////////////////////////////////
	
	//Getting models with any period//
	if(!GetOnlyModelsWithDefinedPeriodRange){
	
		if(GetFirstNegativeGrowthRateModels){
			cout<<"Creating output files for Blue Edge"<<endl;
			
			strcpy(FU_outputFileName_ForBlueEdge, FU_OutputFilePrefix);
			strcat(FU_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
			cout<<"Name of FU output file for blue edge data: "<<FU_outputFileName_ForBlueEdge<<endl;
			ofstream outFileBlueFU(FU_outputFileName_ForBlueEdge);
			outFileBlueFU<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
			strcpy(FO_outputFileName_ForBlueEdge, FO_OutputFilePrefix);
			strcat(FO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
			cout<<"Name of FO output file for blue edge data: "<<FO_outputFileName_ForBlueEdge<<endl;
			ofstream outFileBlueFO(FO_outputFileName_ForBlueEdge);
			outFileBlueFO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
			strcpy(SO_outputFileName_ForBlueEdge, SO_OutputFilePrefix);
			strcat(SO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
			cout<<"Name of 2O output file for blue edge data: "<<SO_outputFileName_ForBlueEdge<<endl;
			ofstream outFileBlue2O(SO_outputFileName_ForBlueEdge);
			outFileBlue2O<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
		}
		
		if(GetPositiveGrowthRateModels){
			cout<<"Creating output files for Positive Growth"<<endl;
			
			strcpy(FU_outputFileName_ForPositiveGrowthModels, FU_OutputFilePrefix);
			strcat(FU_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
			cout<<"Name of FU output file for positive growth data: "<<FU_outputFileName_ForPositiveGrowthModels<<endl;
			ofstream outFilePosFU(FU_outputFileName_ForPositiveGrowthModels);
			outFilePosFU<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
			strcpy(FO_outputFileName_ForPositiveGrowthModels, FO_OutputFilePrefix);
			strcat(FO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
			cout<<"Name of FO output file for positive growth data: "<<FO_outputFileName_ForPositiveGrowthModels<<endl;
			ofstream outFilePosFO(FO_outputFileName_ForPositiveGrowthModels);
			outFilePosFO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
			strcpy(SO_outputFileName_ForPositiveGrowthModels, SO_OutputFilePrefix);
			strcat(SO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
			cout<<"Name of 2O output file for positive growth data: "<<SO_outputFileName_ForPositiveGrowthModels<<endl;
			ofstream outFilePos2O(SO_outputFileName_ForPositiveGrowthModels);
			outFilePos2O<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
		}
		
		////////////////////////////////////
		
		//Variables to keep track of stuff//
		
		int LINAi = 0;
		
		if(GetFirstNegativeGrowthRateModels){
			BlueEdgei = 0;
			NumOfFailedModels = 0;
			NumOfBlueEdgePoints = 0;
		}
		
		if(GetPositiveGrowthRateModels){
			PosGrowthi = 0;
			NumOfFailedModels = 0;
			NumOfPosGrowthPoints = 0;
		}
		
		//LINA Loop//
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
			
				//reads LINA file into arrays
				getline(inFile,LINA_header);
				cout<<LINA_header<<endl;
				inFile>>mode_FU[LINAi]>>period_FU[LINAi]>>growth_FU[LINAi];
				cout<<setw(3)<<mode_FU[LINAi]<<setw(17)<<period_FU[LINAi]<<setw(17)<<growth_FU[LINAi]<<endl;
				inFile>>mode_FO[LINAi]>>period_FO[LINAi]>>growth_FO[LINAi];
				cout<<setw(3)<<mode_FO[LINAi]<<setw(17)<<period_FO[LINAi]<<setw(17)<<growth_FO[LINAi]<<endl;
				inFile>>mode_2O[LINAi]>>period_2O[LINAi]>>growth_2O[LINAi];
				cout<<setw(3)<<mode_2O[LINAi]<<setw(17)<<period_2O[LINAi]<<setw(17)<<growth_2O[LINAi]<<endl;
				
				
				
				if(GetFirstNegativeGrowthRateModels){
					//if f0 is negative and last was positive, then output model parameters and assign T and L to blue edge arrays
					if(LINAi != 0){
						
						//FU selector
						if(growth_FU[LINAi] < 0. && growth_FU[LINAi-1] > 0.){
							
							outFileBlueFU<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FU[LINAi]<<setw(20)<<growth_FU[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FU[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							BlueEdgei = BlueEdgei + 1;
							NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
						}
						//FO selector
						if(growth_FO[LINAi] < 0. && growth_FO[LINAi-1] > 0.){
							
							outFileBlueFO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FO[LINAi]<<setw(20)<<growth_FO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FO[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							BlueEdgei = BlueEdgei + 1;
							NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
						}
						//2O selector
						if(growth_2O[LINAi] < 0. && growth_2O[LINAi-1] > 0.){
							
							outFileBlue2O<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_2O[LINAi]<<setw(20)<<growth_2O[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_2O[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							BlueEdgei = BlueEdgei + 1;
							NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
						}
						
					}
				}
				
				if(GetPositiveGrowthRateModels){
					
					//if f0 is positive, add to file
					if(LINAi != 0){
						
						//FU selector
						if(growth_FU[LINAi] > 0){
							
							outFilePosFU<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FU[LINAi]<<setw(20)<<growth_FU[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FU[LINAi])<<setw(20)<<log(M[i])<<endl;
						
							PosGrowthi = PosGrowthi + 1;
							NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
						}	
						//FO selector
						if(growth_FO[LINAi] > 0){
							
							outFilePosFO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FO[LINAi]<<setw(20)<<growth_FO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FO[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							PosGrowthi = PosGrowthi + 1;
							NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
						}
						//2O selector
						if(growth_2O[LINAi] > 0){
							
							outFilePos2O<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_2O[LINAi]<<setw(20)<<growth_2O[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_2O[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							PosGrowthi = PosGrowthi + 1;
							NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
						}
						
					}
				}
				
				LINAi = LINAi + 1; //Incrementing LINAi
				
				inFile.close();
				
				if(!inFile.is_open()){
					cout<<"LINA File closed successfully"<<endl;
				}
			}
		}
	}

		
	///////////////////////////////////////////////
	
	//Getting models within defined period range//
	if(GetOnlyModelsWithDefinedPeriodRange){
	
		if(GetFirstNegativeGrowthRateModels){
			cout<<"Creating output files for Blue Edge"<<endl;
			
			strcpy(FU_outputFileName_ForBlueEdge, FU_OutputFilePrefix);
			strcat(FU_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
			cout<<"Name of FU output file for blue edge data: "<<FU_outputFileName_ForBlueEdge<<endl;
			ofstream outFileBlueFU(FU_outputFileName_ForBlueEdge);
			outFileBlueFU<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
			strcpy(FO_outputFileName_ForBlueEdge, FO_OutputFilePrefix);
			strcat(FO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
			cout<<"Name of FO output file for blue edge data: "<<FO_outputFileName_ForBlueEdge<<endl;
			ofstream outFileBlueFO(FO_outputFileName_ForBlueEdge);
			outFileBlueFO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
			strcpy(SO_outputFileName_ForBlueEdge, SO_OutputFilePrefix);
			strcat(SO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
			cout<<"Name of 2O output file for blue edge data: "<<SO_outputFileName_ForBlueEdge<<endl;
			ofstream outFileBlue2O(SO_outputFileName_ForBlueEdge);
			outFileBlue2O<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
		}
		
		if(GetPositiveGrowthRateModels){
			cout<<"Creating output files for Positive Growth"<<endl;
			
			strcpy(FU_outputFileName_ForPositiveGrowthModels, FU_OutputFilePrefix);
			strcat(FU_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
			cout<<"Name of FU output file for positive growth data: "<<FU_outputFileName_ForPositiveGrowthModels<<endl;
			ofstream outFilePosFU(FU_outputFileName_ForPositiveGrowthModels);
			outFilePosFU<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
			strcpy(FO_outputFileName_ForPositiveGrowthModels, FO_OutputFilePrefix);
			strcat(FO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
			cout<<"Name of FO output file for positive growth data: "<<FO_outputFileName_ForPositiveGrowthModels<<endl;
			ofstream outFilePosFO(FO_outputFileName_ForPositiveGrowthModels);
			outFilePosFO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
			strcpy(SO_outputFileName_ForPositiveGrowthModels, SO_OutputFilePrefix);
			strcat(SO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
			cout<<"Name of 2O output file for positive growth data: "<<SO_outputFileName_ForPositiveGrowthModels<<endl;
			ofstream outFilePos2O(SO_outputFileName_ForPositiveGrowthModels);
			outFilePos2O<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
			
		}
		
		////////////////////////////////////
		
		//Variables to keep track of stuff//
		
		int LINAi = 0;
		
		if(GetFirstNegativeGrowthRateModels){
			BlueEdgei = 0;
			NumOfFailedModels = 0;
			NumOfBlueEdgePoints = 0;
		}
		
		if(GetPositiveGrowthRateModels){
			PosGrowthi = 0;
			NumOfFailedModels = 0;
			NumOfPosGrowthPoints = 0;
		}
		
		//LINA Loop//
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
			
				//reads LINA file into arrays
				getline(inFile,LINA_header);
				cout<<LINA_header<<endl;
				inFile>>mode_FU[LINAi]>>period_FU[LINAi]>>growth_FU[LINAi];
				cout<<setw(3)<<mode_FU[LINAi]<<setw(17)<<period_FU[LINAi]<<setw(17)<<growth_FU[LINAi]<<endl;
				inFile>>mode_FO[LINAi]>>period_FO[LINAi]>>growth_FO[LINAi];
				cout<<setw(3)<<mode_FO[LINAi]<<setw(17)<<period_FO[LINAi]<<setw(17)<<growth_FO[LINAi]<<endl;
				inFile>>mode_2O[LINAi]>>period_2O[LINAi]>>growth_2O[LINAi];
				cout<<setw(3)<<mode_2O[LINAi]<<setw(17)<<period_2O[LINAi]<<setw(17)<<growth_2O[LINAi]<<endl;
				
				
				
				if(GetFirstNegativeGrowthRateModels){
					//if f0 is negative and last was positive, then output model parameters and assign T and L to blue edge arrays
					if(LINAi != 0){
						
						//FU selector
						if(growth_FU[LINAi] < 0. && growth_FU[LINAi-1] > 0. && period_FU[LINAi] < Higher_Period_Boundry && period_FU[LINAi] > Lower_Period_Boundry){
							
							outFileBlueFU<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FU[LINAi]<<setw(20)<<growth_FU[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FU[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							BlueEdgei = BlueEdgei + 1;
							NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
						}
						//FO selector
						if(growth_FO[LINAi] < 0. && growth_FO[LINAi-1] > 0. && period_FO[LINAi] < Higher_Period_Boundry && period_FO[LINAi] > Lower_Period_Boundry){
							
							outFileBlueFO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FO[LINAi]<<setw(20)<<growth_FO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FO[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							BlueEdgei = BlueEdgei + 1;
							NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
						}
						//2O selector
						if(growth_2O[LINAi] < 0. && growth_2O[LINAi-1] > 0. && period_2O[LINAi] < Higher_Period_Boundry && period_2O[LINAi] > Lower_Period_Boundry){
							
							outFileBlue2O<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_2O[LINAi]<<setw(20)<<growth_2O[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_2O[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							BlueEdgei = BlueEdgei + 1;
							NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
						}
						
					}
				}
				
				if(GetPositiveGrowthRateModels){
					
					//if f0 is positive, add to file
					if(LINAi != 0){
						
						//FU selector
						if(growth_FU[LINAi] > 0 && period_FU[LINAi] < Higher_Period_Boundry && period_FU[LINAi] > Lower_Period_Boundry){
							
							outFilePosFU<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FU[LINAi]<<setw(20)<<growth_FU[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FU[LINAi])<<setw(20)<<log(M[i])<<endl;
						
							PosGrowthi = PosGrowthi + 1;
							NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
						}	
						//FO selector
						if(growth_FO[LINAi] > 0 && period_FO[LINAi] < Higher_Period_Boundry && period_FO[LINAi] > Lower_Period_Boundry){
							
							outFilePosFO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FO[LINAi]<<setw(20)<<growth_FO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FO[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							PosGrowthi = PosGrowthi + 1;
							NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
						}
						//2O selector
						if(growth_2O[LINAi] > 0 && period_2O[LINAi] < Higher_Period_Boundry && period_2O[LINAi] > Lower_Period_Boundry){
							
							outFilePos2O<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_2O[LINAi]<<setw(20)<<growth_2O[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_2O[LINAi])<<setw(20)<<log(M[i])<<endl;
							
							PosGrowthi = PosGrowthi + 1;
							NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
						}
						
					}
				}
				
				LINAi = LINAi + 1; //Incrementing LINAi
				
				inFile.close();
				
				if(!inFile.is_open()){
					cout<<"LINA File closed successfully"<<endl;
				}
			}
		}
	}
	
	
	
	//////////////////////////////////////////////
	
	//Selectors for total number printouts//
	
	if(GetFirstNegativeGrowthRateModels){
		cout<<"NumOfBlueEdgePoints: "<<NumOfBlueEdgePoints<<endl;
	}
	
	
	if(GetPositiveGrowthRateModels){
		cout<<"NumOfPosGrowthPoints: "<<NumOfPosGrowthPoints<<endl;
	}
	
	////////////////////////////////////
	
	///Test loop for LINA data 
	// int LINAi_max = LINAi;
	// for(int l = 0; l < LINAi_max; l++){
		// cout<<setw(3)<<mode_FU[l]<<setw(17)<<period_FU[l]<<setw(17)<<growth_FU[l]<<endl;
		
		// if(l != 0){
			// if(growth_FU[l-1] > 0 && growth_FU[l] < 0){
				// outFile<<Mod[i]<<setw(15)<<z[i]<<setw(15)<<x[i]<<setw(15)<<M[i]<<setw(15)<<L[i]<<setw(15)<<T[i]<<endl;
				
				// L_Blue[i] = L[i];
				// T_Blue[i] = T[i];
			// }
		// }
	// }
	
	////////////////////////////////////////
	
	
	return 0;
}