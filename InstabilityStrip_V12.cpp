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

//V9: Attempt to add all modes up to 10. Also changed name of program to better align with its current function, was perviously BlueEdge_Plotter.

//V10: Fixing compliation problems

//V11: Adding output for overall period range, cureenty only for FU mode

//V12: Adding bool for minimizing output to screen

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
	int NumOfModes = 10; // Number of modes in the model. Max is 10 currently
	
	char inputFileName[30] = "InputDataFULL.dat"; //File with the model parameters
	char outputFileName_ForBlueEdge[50] = "BlueEdge_SetD.dat"; //File with model parameters on the blue edge. Will have prefix for mode that is defined below.
	char outputFileName_ForPositiveGrowthModels[50] = "PostiveGrowth_SetD.dat"; //File with model parameters that have positve growth rates. Will have prefix for mode that is defined below.
	
	char log_directory_prefix[30] = "Raw_LINA_data/SetC/LOGS_"; //Prefix to log_directory, suffix is model number. This is where LINA file should be
	char LINAFileName[30] = "LINA_period_growth.data"; //Normally should be "LINA_period_growth.data"
	
	char FU_OutputFilePrefix[10] = "FU_"; //Prefix to output files that contain data for FU mode
	char FO_OutputFilePrefix[10] = "FO_"; //Prefix to output files that contain data for FO mode
	char SO_OutputFilePrefix[10] = "2O_"; //Prefix to output files that contain data for 2O mode
	char ThirdO_OutputFilePrefix[10] = "ThirdO_"; //Prefix to output files that contain data for ThirdO mode
	char ForthO_OutputFilePrefix[10] = "ForthO_"; //Prefix to output files that contain data for ForthO mode
	char FifthO_OutputFilePrefix[10] = "FifthO_"; //Prefix to output files that contain data for FifthO mode
	char SixthO_OutputFilePrefix[10] = "SixthO_"; //Prefix to output files that contain data for SixthO mode
	char SeventhO_OutputFilePrefix[10] = "SeventhO_"; //Prefix to output files that contain data for SeventhO mode
	char EighthO_OutputFilePrefix[10] = "EighthO_"; //Prefix to output files that contain data for EighthO mode
	char NinethO_OutputFilePrefix[10] = "NinethO_"; //Prefix to output files that contain data for NinethO mode
	
	bool GetPositiveGrowthRateModels = true; // Positive Growth. When true, postive growth rate models will be outputted to a file.
	bool GetFirstNegativeGrowthRateModels = false; // Blue Edge. When true, the first negative growth rate models will be outputted to a file (For blue edge!)
	bool GetOnlyModelsWithDefinedPeriodRange = false; // Period Range. When true, gets only models within the defined period range.
	bool MinimizeOutputToScreen = true; // Minimizes output to screen, not showing LINA data. To make the program run faster.
	
	double Lower_Period_Boundry = 1; //Low bound to period range in days. Only is used when GetOnlyModelsWithDefinedPeriodRange is true 
	double Higher_Period_Boundry = 4; //High bound to period range in days. Only is used when GetOnlyModelsWithDefinedPeriodRange is true
	
	///////////////////////////////
	
	//Variables//
	
	//chars that store the FULL output file names.
	char FU_outputFileName_ForBlueEdge[50];
	char FO_outputFileName_ForBlueEdge[50];
	char SO_outputFileName_ForBlueEdge[50];
	char ThirdO_outputFileName_ForBlueEdge[50];
	char ForthO_outputFileName_ForBlueEdge[50];
	char FifthO_outputFileName_ForBlueEdge[50];
	char SixthO_outputFileName_ForBlueEdge[50];
	char SeventhO_outputFileName_ForBlueEdge[50];
	char EighthO_outputFileName_ForBlueEdge[50];
	char NinethO_outputFileName_ForBlueEdge[50];
	
	char FU_outputFileName_ForPositiveGrowthModels[50];
	char FO_outputFileName_ForPositiveGrowthModels[50];
	char SO_outputFileName_ForPositiveGrowthModels[50];
	char ThirdO_outputFileName_ForPositiveGrowthModels[50];
	char ForthO_outputFileName_ForPositiveGrowthModels[50];
	char FifthO_outputFileName_ForPositiveGrowthModels[50];
	char SixthO_outputFileName_ForPositiveGrowthModels[50];
	char SeventhO_outputFileName_ForPositiveGrowthModels[50];
	char EighthO_outputFileName_ForPositiveGrowthModels[50];
	char NinethO_outputFileName_ForPositiveGrowthModels[50];
	
	char LINAFilePath[50];
	
	int BlueEdgei;
	int NumOfBlueEdgePoints;
	
	int NumOfFailedModels;
	int LINAi;
	
	int PosGrowthi;
	int NumOfPosGrowthPoints;
	
	double LowestPeriodFU = 10000;
	double HighestPeriodFU = 0;
	
	ofstream outFileBlueFU;
	ofstream outFileBlueFO;
	ofstream outFileBlue2O;
	ofstream outFileBlueThirdO;
	ofstream outFileBlueForthO;
	ofstream outFileBlueFifthO;
	ofstream outFileBlueSixthO;
	ofstream outFileBlueSeventhO;
	ofstream outFileBlueEighthO;
	ofstream outFileBlueNinethO;
	
	ofstream outFilePosFU;
	ofstream outFilePosFO;
	ofstream outFilePos2O;
	ofstream outFilePosThirdO;
	ofstream outFilePosForthO;
	ofstream outFilePosFifthO;
	ofstream outFilePosSixthO;
	ofstream outFilePosSeventhO;
	ofstream outFilePosEighthO;
	ofstream outFilePosNinethO;
	
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
	
	double mode_ThirdO[nArray];
	double period_ThirdO[nArray];
	double growth_ThirdO[nArray];
	
	double mode_ForthO[nArray];
	double period_ForthO[nArray];
	double growth_ForthO[nArray];
	
	double mode_FifthO[nArray];
	double period_FifthO[nArray];
	double growth_FifthO[nArray];
	
	double mode_SixthO[nArray];
	double period_SixthO[nArray];
	double growth_SixthO[nArray];
	
	double mode_SeventhO[nArray];
	double period_SeventhO[nArray];
	double growth_SeventhO[nArray];
	
	double mode_EighthO[nArray];
	double period_EighthO[nArray];
	double growth_EighthO[nArray];
	
	double mode_NinethO[nArray];
	double period_NinethO[nArray];
	double growth_NinethO[nArray];
	
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
			
			//Loop for number of modes on output files. Each iteration looks at a different mode.
			for(int m1 = 0; m1 < NumOfModes; m1++){
				
				if(m1 == 0){
					strcpy(FU_outputFileName_ForBlueEdge, FU_OutputFilePrefix);
					strcat(FU_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of FU output file for blue edge data: "<<FU_outputFileName_ForBlueEdge<<endl;
					outFileBlueFU.open(FU_outputFileName_ForBlueEdge, ios::out);
					outFileBlueFU<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 1){
					strcpy(FO_outputFileName_ForBlueEdge, FO_OutputFilePrefix);
					strcat(FO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of FO output file for blue edge data: "<<FO_outputFileName_ForBlueEdge<<endl;
					outFileBlueFO.open(FO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueFO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 2){
					strcpy(SO_outputFileName_ForBlueEdge, SO_OutputFilePrefix);
					strcat(SO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of 2O output file for blue edge data: "<<SO_outputFileName_ForBlueEdge<<endl;
					outFileBlue2O.open(SO_outputFileName_ForBlueEdge, ios::out);
					outFileBlue2O<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}	
				
				if(m1 == 3){
					strcpy(ThirdO_outputFileName_ForBlueEdge, ThirdO_OutputFilePrefix);
					strcat(ThirdO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of ThirdO output file for blue edge data: "<<ThirdO_outputFileName_ForBlueEdge<<endl;
					outFileBlueThirdO.open(ThirdO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueThirdO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}

				if(m1 == 4){
					strcpy(ForthO_outputFileName_ForBlueEdge, ForthO_OutputFilePrefix);
					strcat(ForthO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of ForthO output file for blue edge data: "<<ForthO_outputFileName_ForBlueEdge<<endl;
					outFileBlueForthO.open(ForthO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueForthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 5){
					strcpy(FifthO_outputFileName_ForBlueEdge, FifthO_OutputFilePrefix);
					strcat(FifthO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of FifthO output file for blue edge data: "<<FifthO_outputFileName_ForBlueEdge<<endl;
					outFileBlueFifthO.open(FifthO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueFifthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 6){
					strcpy(SixthO_outputFileName_ForBlueEdge, SixthO_OutputFilePrefix);
					strcat(SixthO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of SixthO output file for blue edge data: "<<SixthO_outputFileName_ForBlueEdge<<endl;
					outFileBlueSixthO.open(SixthO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueSixthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 7){
					strcpy(SeventhO_outputFileName_ForBlueEdge, SeventhO_OutputFilePrefix);
					strcat(SeventhO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of SeventhO output file for blue edge data: "<<SeventhO_outputFileName_ForBlueEdge<<endl;
					outFileBlueSeventhO.open(SeventhO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueSeventhO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 8){
					strcpy(EighthO_outputFileName_ForBlueEdge, EighthO_OutputFilePrefix);
					strcat(EighthO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of EighthO output file for blue edge data: "<<EighthO_outputFileName_ForBlueEdge<<endl;
					outFileBlueEighthO.open(EighthO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueEighthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 9){
					strcpy(NinethO_outputFileName_ForBlueEdge, NinethO_OutputFilePrefix);
					strcat(NinethO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of NinethO output file for blue edge data: "<<NinethO_outputFileName_ForBlueEdge<<endl;
					outFileBlueNinethO.open(NinethO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueNinethO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}	
			}
		}
		
		if(GetPositiveGrowthRateModels){
			cout<<"Creating output files for Positive Growth"<<endl;
			
			//Loop for number of modes on output files. Each iteration looks at a different mode.
			for(int m2 = 0; m2 < NumOfModes; m2++){
				
				if(m2 == 0){
					strcpy(FU_outputFileName_ForPositiveGrowthModels, FU_OutputFilePrefix);
					strcat(FU_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of FU output file for positive growth data: "<<FU_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosFU.open(FU_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosFU<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 1){
					strcpy(FO_outputFileName_ForPositiveGrowthModels, FO_OutputFilePrefix);
					strcat(FO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of FO output file for positive growth data: "<<FO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosFO.open(FO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosFO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 2){
					strcpy(SO_outputFileName_ForPositiveGrowthModels, SO_OutputFilePrefix);
					strcat(SO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of 2O output file for positive growth data: "<<SO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePos2O.open(SO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePos2O<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}	
				
				if(m2 == 3){
					strcpy(ThirdO_outputFileName_ForPositiveGrowthModels, ThirdO_OutputFilePrefix);
					strcat(ThirdO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of ThirdO output file for positive growth data: "<<ThirdO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosThirdO.open(ThirdO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosThirdO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}

				if(m2 == 4){
					strcpy(ForthO_outputFileName_ForPositiveGrowthModels, ForthO_OutputFilePrefix);
					strcat(ForthO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of ForthO output file for positive growth data: "<<ForthO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosForthO.open(ForthO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosForthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 5){
					strcpy(FifthO_outputFileName_ForPositiveGrowthModels, FifthO_OutputFilePrefix);
					strcat(FifthO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of FifthO output file for positive growth data: "<<FifthO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosFifthO.open(FifthO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosFifthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 6){
					strcpy(SixthO_outputFileName_ForPositiveGrowthModels, SixthO_OutputFilePrefix);
					strcat(SixthO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of SixthO output file for positive growth data: "<<SixthO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosSixthO.open(SixthO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosSixthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 7){
					strcpy(SeventhO_outputFileName_ForPositiveGrowthModels, SeventhO_OutputFilePrefix);
					strcat(SeventhO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of SeventhO output file for positive growth data: "<<SeventhO_outputFileName_ForPositiveGrowthModels<<endl;
					ofstream outFilePosSeventhO(SeventhO_outputFileName_ForPositiveGrowthModels);
					outFilePosSeventhO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 8){
					strcpy(EighthO_outputFileName_ForPositiveGrowthModels, EighthO_OutputFilePrefix);
					strcat(EighthO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of EighthO output file for positive growth data: "<<EighthO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosEighthO.open(EighthO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosEighthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 9){
					strcpy(NinethO_outputFileName_ForPositiveGrowthModels, NinethO_OutputFilePrefix);
					strcat(NinethO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of NinethO output file for positive growth data: "<<NinethO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosNinethO.open(NinethO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosNinethO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
			}
		}
		
		////////////////////////////////////
		
		//Variables to keep track of stuff//
		
		LINAi = 0;
		
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
			if(inFile.is_open() && !MinimizeOutputToScreen){
				cout<<"LINA File was opened successfully"<<endl;
			}
			
			
			if(inFile.good()){

				if(!MinimizeOutputToScreen){
					
					cout<<"LINA File is being read..."<<endl;
					
					//reads LINA file into arrays
					getline(inFile,LINA_header);
					cout<<LINA_header<<endl;
					
					//Loop for number of modes on LINA data. Each iteration looks at a different mode and fills array to be used.
					for(int m3 = 0; m3 < NumOfModes; m3++){
						
						if(m3 == 0){
							inFile>>mode_FU[LINAi]>>period_FU[LINAi]>>growth_FU[LINAi];
							cout<<setw(3)<<mode_FU[LINAi]<<setw(17)<<period_FU[LINAi]<<setw(17)<<growth_FU[LINAi]<<endl;
							
							if(period_FU[LINAi] < LowestPeriodFU){
								LowestPeriodFU = period_FU[LINAi];
							}
							if(period_FU[LINAi] > HighestPeriodFU){
								HighestPeriodFU = period_FU[LINAi];
							}
						}
						
						if(m3 == 1){
							inFile>>mode_FO[LINAi]>>period_FO[LINAi]>>growth_FO[LINAi];
							cout<<setw(3)<<mode_FO[LINAi]<<setw(17)<<period_FO[LINAi]<<setw(17)<<growth_FO[LINAi]<<endl;
						}
						
						if(m3 == 2){
							inFile>>mode_2O[LINAi]>>period_2O[LINAi]>>growth_2O[LINAi];
							cout<<setw(3)<<mode_2O[LINAi]<<setw(17)<<period_2O[LINAi]<<setw(17)<<growth_2O[LINAi]<<endl;
						}
						
						if(m3 == 3){
							inFile>>mode_ThirdO[LINAi]>>period_ThirdO[LINAi]>>growth_ThirdO[LINAi];
							cout<<setw(3)<<mode_ThirdO[LINAi]<<setw(17)<<period_ThirdO[LINAi]<<setw(17)<<growth_ThirdO[LINAi]<<endl;
						}
						
						if(m3 == 4){
							inFile>>mode_ForthO[LINAi]>>period_ForthO[LINAi]>>growth_ForthO[LINAi];
							cout<<setw(3)<<mode_ForthO[LINAi]<<setw(17)<<period_ForthO[LINAi]<<setw(17)<<growth_ForthO[LINAi]<<endl;
						}
						
						if(m3 == 5){
							inFile>>mode_FifthO[LINAi]>>period_FifthO[LINAi]>>growth_FifthO[LINAi];
							cout<<setw(3)<<mode_FifthO[LINAi]<<setw(17)<<period_FifthO[LINAi]<<setw(17)<<growth_FifthO[LINAi]<<endl;
						}
						
						if(m3 == 6){
							inFile>>mode_SixthO[LINAi]>>period_SixthO[LINAi]>>growth_SixthO[LINAi];
							cout<<setw(3)<<mode_SixthO[LINAi]<<setw(17)<<period_SixthO[LINAi]<<setw(17)<<growth_SixthO[LINAi]<<endl;
						}
						
						if(m3 == 7){
							inFile>>mode_SeventhO[LINAi]>>period_SeventhO[LINAi]>>growth_SeventhO[LINAi];
							cout<<setw(3)<<mode_SeventhO[LINAi]<<setw(17)<<period_SeventhO[LINAi]<<setw(17)<<growth_SeventhO[LINAi]<<endl;
						}
						
						if(m3 == 8){
							inFile>>mode_EighthO[LINAi]>>period_EighthO[LINAi]>>growth_EighthO[LINAi];
							cout<<setw(3)<<mode_EighthO[LINAi]<<setw(17)<<period_EighthO[LINAi]<<setw(17)<<growth_EighthO[LINAi]<<endl;
						}
						
						if(m3 == 9){
							inFile>>mode_NinethO[LINAi]>>period_NinethO[LINAi]>>growth_NinethO[LINAi];
							cout<<setw(3)<<mode_NinethO[LINAi]<<setw(17)<<period_NinethO[LINAi]<<setw(17)<<growth_NinethO[LINAi]<<endl;
						}
					}
				}
				
				if(MinimizeOutputToScreen){
					//reads LINA file into arrays
					getline(inFile,LINA_header);
					//cout<<LINA_header<<endl;
					
					//Loop for number of modes on LINA data. Each iteration looks at a different mode and fills array to be used.
					for(int m3 = 0; m3 < NumOfModes; m3++){
						
						if(m3 == 0){
							inFile>>mode_FU[LINAi]>>period_FU[LINAi]>>growth_FU[LINAi];
							//cout<<setw(3)<<mode_FU[LINAi]<<setw(17)<<period_FU[LINAi]<<setw(17)<<growth_FU[LINAi]<<endl;
							
							if(period_FU[LINAi] < LowestPeriodFU){
								LowestPeriodFU = period_FU[LINAi];
							}
							if(period_FU[LINAi] > HighestPeriodFU){
								HighestPeriodFU = period_FU[LINAi];
							}
						}
						
						if(m3 == 1){
							inFile>>mode_FO[LINAi]>>period_FO[LINAi]>>growth_FO[LINAi];
							//cout<<setw(3)<<mode_FO[LINAi]<<setw(17)<<period_FO[LINAi]<<setw(17)<<growth_FO[LINAi]<<endl;
						}
						
						if(m3 == 2){
							inFile>>mode_2O[LINAi]>>period_2O[LINAi]>>growth_2O[LINAi];
							//cout<<setw(3)<<mode_2O[LINAi]<<setw(17)<<period_2O[LINAi]<<setw(17)<<growth_2O[LINAi]<<endl;
						}
						
						if(m3 == 3){
							inFile>>mode_ThirdO[LINAi]>>period_ThirdO[LINAi]>>growth_ThirdO[LINAi];
							//cout<<setw(3)<<mode_ThirdO[LINAi]<<setw(17)<<period_ThirdO[LINAi]<<setw(17)<<growth_ThirdO[LINAi]<<endl;
						}
						
						if(m3 == 4){
							inFile>>mode_ForthO[LINAi]>>period_ForthO[LINAi]>>growth_ForthO[LINAi];
							//cout<<setw(3)<<mode_ForthO[LINAi]<<setw(17)<<period_ForthO[LINAi]<<setw(17)<<growth_ForthO[LINAi]<<endl;
						}
						
						if(m3 == 5){
							inFile>>mode_FifthO[LINAi]>>period_FifthO[LINAi]>>growth_FifthO[LINAi];
							//cout<<setw(3)<<mode_FifthO[LINAi]<<setw(17)<<period_FifthO[LINAi]<<setw(17)<<growth_FifthO[LINAi]<<endl;
						}
						
						if(m3 == 6){
							inFile>>mode_SixthO[LINAi]>>period_SixthO[LINAi]>>growth_SixthO[LINAi];
							//cout<<setw(3)<<mode_SixthO[LINAi]<<setw(17)<<period_SixthO[LINAi]<<setw(17)<<growth_SixthO[LINAi]<<endl;
						}
						
						if(m3 == 7){
							inFile>>mode_SeventhO[LINAi]>>period_SeventhO[LINAi]>>growth_SeventhO[LINAi];
							//cout<<setw(3)<<mode_SeventhO[LINAi]<<setw(17)<<period_SeventhO[LINAi]<<setw(17)<<growth_SeventhO[LINAi]<<endl;
						}
						
						if(m3 == 8){
							inFile>>mode_EighthO[LINAi]>>period_EighthO[LINAi]>>growth_EighthO[LINAi];
							//cout<<setw(3)<<mode_EighthO[LINAi]<<setw(17)<<period_EighthO[LINAi]<<setw(17)<<growth_EighthO[LINAi]<<endl;
						}
						
						if(m3 == 9){
							inFile>>mode_NinethO[LINAi]>>period_NinethO[LINAi]>>growth_NinethO[LINAi];
							//cout<<setw(3)<<mode_NinethO[LINAi]<<setw(17)<<period_NinethO[LINAi]<<setw(17)<<growth_NinethO[LINAi]<<endl;
						}
					}
				}
				
				
				if(GetFirstNegativeGrowthRateModels){
					//if f0 is negative and last was positive, then output model parameters and assign T and L to blue edge arrays
					if(LINAi != 0){
						
						//loop for number of modes on blue edge data. Each interation looks at a different mode.
						for(int m4 = 0; m4 < NumOfModes; m4++){
							
							if(m4 == 0){
								if(growth_FU[LINAi] < 0. && growth_FU[LINAi-1] > 0.){
									
									outFileBlueFU<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FU[LINAi]<<setw(20)<<growth_FU[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FU[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 1){	
								if(growth_FO[LINAi] < 0. && growth_FO[LINAi-1] > 0.){
									
									outFileBlueFO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FO[LINAi]<<setw(20)<<growth_FO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}	
							
							if(m4 == 2){
								if(growth_2O[LINAi] < 0. && growth_2O[LINAi-1] > 0.){
									
									outFileBlue2O<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_2O[LINAi]<<setw(20)<<growth_2O[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_2O[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 3){
								if(growth_ThirdO[LINAi] < 0. && growth_ThirdO[LINAi-1] > 0.){
									
									outFileBlueThirdO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_ThirdO[LINAi]<<setw(20)<<growth_ThirdO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_ThirdO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 4){
								if(growth_ForthO[LINAi] < 0. && growth_ForthO[LINAi-1] > 0.){
									
									outFileBlueForthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_ForthO[LINAi]<<setw(20)<<growth_ForthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_ForthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 5){
								if(growth_FifthO[LINAi] < 0. && growth_FifthO[LINAi-1] > 0.){
									
									outFileBlueFifthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FifthO[LINAi]<<setw(20)<<growth_FifthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FifthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 6){
								if(growth_SixthO[LINAi] < 0. && growth_SixthO[LINAi-1] > 0.){
									
									outFileBlueSixthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_SixthO[LINAi]<<setw(20)<<growth_SixthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_SixthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 7){
								if(growth_SeventhO[LINAi] < 0. && growth_SeventhO[LINAi-1] > 0.){
									
									outFileBlueSeventhO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_SeventhO[LINAi]<<setw(20)<<growth_SeventhO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_SeventhO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 8){
								if(growth_EighthO[LINAi] < 0. && growth_EighthO[LINAi-1] > 0.){
									
									outFileBlueEighthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_EighthO[LINAi]<<setw(20)<<growth_EighthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_EighthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 9){
								if(growth_NinethO[LINAi] < 0. && growth_NinethO[LINAi-1] > 0.){
									
									outFileBlueNinethO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_NinethO[LINAi]<<setw(20)<<growth_NinethO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_NinethO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
						}
					}
				}
				
				if(GetPositiveGrowthRateModels){
					
					//if f0 is positive, add to file
					if(LINAi != 0){
						
						for(int m5 = 0; m5 < NumOfModes; m5++){
							
							if(m5 == 0){
								if(growth_FU[LINAi] > 0){
									
									outFilePosFU<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FU[LINAi]<<setw(20)<<growth_FU[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FU[LINAi])<<setw(20)<<log(M[i])<<endl;
								
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}	
							}	
							
							if(m5 == 1){
								if(growth_FO[LINAi] > 0){
									
									outFilePosFO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FO[LINAi]<<setw(20)<<growth_FO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
								
							if(m5 == 2){
								if(growth_2O[LINAi] > 0){
									
									outFilePos2O<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_2O[LINAi]<<setw(20)<<growth_2O[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_2O[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 3){
								if(growth_ThirdO[LINAi] > 0){
									
									outFilePosThirdO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_ThirdO[LINAi]<<setw(20)<<growth_ThirdO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_ThirdO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 4){
								if(growth_ForthO[LINAi] > 0){
									
									outFilePosForthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_ForthO[LINAi]<<setw(20)<<growth_ForthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_ForthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 5){
								if(growth_FifthO[LINAi] > 0){
									
									outFilePosFifthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FifthO[LINAi]<<setw(20)<<growth_FifthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FifthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 6){
								if(growth_SixthO[LINAi] > 0){
									
									outFilePosSixthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_SixthO[LINAi]<<setw(20)<<growth_SixthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_SixthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 7){
								if(growth_SeventhO[LINAi] > 0){
									
									outFilePosSeventhO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_SeventhO[LINAi]<<setw(20)<<growth_SeventhO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_SeventhO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 8){
								if(growth_EighthO[LINAi] > 0){
									
									outFilePosEighthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_EighthO[LINAi]<<setw(20)<<growth_EighthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_EighthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 9){
								if(growth_NinethO[LINAi] > 0){
									
									outFilePosNinethO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_NinethO[LINAi]<<setw(20)<<growth_NinethO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_NinethO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
						}
					}
				}
				
				LINAi = LINAi + 1; //Incrementing LINAi
				
				inFile.close();
				
				if(!inFile.is_open() && !MinimizeOutputToScreen){
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
			
			//Loop for number of modes on output files. Each iteration looks at a different mode.
			for(int m1 = 0; m1 < NumOfModes; m1++){
				
				if(m1 == 0){
					strcpy(FU_outputFileName_ForBlueEdge, FU_OutputFilePrefix);
					strcat(FU_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of FU output file for blue edge data: "<<FU_outputFileName_ForBlueEdge<<endl;
					outFileBlueFU.open(FU_outputFileName_ForBlueEdge, ios::out);
					outFileBlueFU<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 1){
					strcpy(FO_outputFileName_ForBlueEdge, FO_OutputFilePrefix);
					strcat(FO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of FO output file for blue edge data: "<<FO_outputFileName_ForBlueEdge<<endl;
					outFileBlueFO.open(FO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueFO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 2){
					strcpy(SO_outputFileName_ForBlueEdge, SO_OutputFilePrefix);
					strcat(SO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of 2O output file for blue edge data: "<<SO_outputFileName_ForBlueEdge<<endl;
					outFileBlue2O.open(SO_outputFileName_ForBlueEdge, ios::out);
					outFileBlue2O<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}	
				
				if(m1 == 3){
					strcpy(ThirdO_outputFileName_ForBlueEdge, ThirdO_OutputFilePrefix);
					strcat(ThirdO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of ThirdO output file for blue edge data: "<<ThirdO_outputFileName_ForBlueEdge<<endl;
					outFileBlueThirdO.open(ThirdO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueThirdO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}

				if(m1 == 4){
					strcpy(ForthO_outputFileName_ForBlueEdge, ForthO_OutputFilePrefix);
					strcat(ForthO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of ForthO output file for blue edge data: "<<ForthO_outputFileName_ForBlueEdge<<endl;
					outFileBlueForthO.open(ForthO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueForthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 5){
					strcpy(FifthO_outputFileName_ForBlueEdge, FifthO_OutputFilePrefix);
					strcat(FifthO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of FifthO output file for blue edge data: "<<FifthO_outputFileName_ForBlueEdge<<endl;
					outFileBlueFifthO.open(FifthO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueFifthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 6){
					strcpy(SixthO_outputFileName_ForBlueEdge, SixthO_OutputFilePrefix);
					strcat(SixthO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of SixthO output file for blue edge data: "<<SixthO_outputFileName_ForBlueEdge<<endl;
					outFileBlueSixthO.open(SixthO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueSixthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 7){
					strcpy(SeventhO_outputFileName_ForBlueEdge, SeventhO_OutputFilePrefix);
					strcat(SeventhO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of SeventhO output file for blue edge data: "<<SeventhO_outputFileName_ForBlueEdge<<endl;
					outFileBlueSeventhO.open(SeventhO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueSeventhO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 8){
					strcpy(EighthO_outputFileName_ForBlueEdge, EighthO_OutputFilePrefix);
					strcat(EighthO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of EighthO output file for blue edge data: "<<EighthO_outputFileName_ForBlueEdge<<endl;
					outFileBlueEighthO.open(EighthO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueEighthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m1 == 9){
					strcpy(NinethO_outputFileName_ForBlueEdge, NinethO_OutputFilePrefix);
					strcat(NinethO_outputFileName_ForBlueEdge, outputFileName_ForBlueEdge);
					cout<<"Name of NinethO output file for blue edge data: "<<NinethO_outputFileName_ForBlueEdge<<endl;
					outFileBlueNinethO.open(NinethO_outputFileName_ForBlueEdge, ios::out);
					outFileBlueNinethO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}	
			}
		}
		
		if(GetPositiveGrowthRateModels){
			cout<<"Creating output files for Positive Growth"<<endl;
			
			//Loop for number of modes on output files. Each iteration looks at a different mode.
			for(int m2 = 0; m2 < NumOfModes; m2++){
				
				if(m2 == 0){
					strcpy(FU_outputFileName_ForPositiveGrowthModels, FU_OutputFilePrefix);
					strcat(FU_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of FU output file for positive growth data: "<<FU_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosFU.open(FU_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosFU<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 1){
					strcpy(FO_outputFileName_ForPositiveGrowthModels, FO_OutputFilePrefix);
					strcat(FO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of FO output file for positive growth data: "<<FO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosFO.open(FO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosFO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 2){
					strcpy(SO_outputFileName_ForPositiveGrowthModels, SO_OutputFilePrefix);
					strcat(SO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of 2O output file for positive growth data: "<<SO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePos2O.open(SO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePos2O<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}	
				
				if(m2 == 3){
					strcpy(ThirdO_outputFileName_ForPositiveGrowthModels, ThirdO_OutputFilePrefix);
					strcat(ThirdO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of ThirdO output file for positive growth data: "<<ThirdO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosThirdO.open(ThirdO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosThirdO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}

				if(m2 == 4){
					strcpy(ForthO_outputFileName_ForPositiveGrowthModels, ForthO_OutputFilePrefix);
					strcat(ForthO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of ForthO output file for positive growth data: "<<ForthO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosForthO.open(ForthO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosForthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 5){
					strcpy(FifthO_outputFileName_ForPositiveGrowthModels, FifthO_OutputFilePrefix);
					strcat(FifthO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of FifthO output file for positive growth data: "<<FifthO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosFifthO.open(FifthO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosFifthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 6){
					strcpy(SixthO_outputFileName_ForPositiveGrowthModels, SixthO_OutputFilePrefix);
					strcat(SixthO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of SixthO output file for positive growth data: "<<SixthO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosSixthO.open(SixthO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosSixthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 7){
					strcpy(SeventhO_outputFileName_ForPositiveGrowthModels, SeventhO_OutputFilePrefix);
					strcat(SeventhO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of SeventhO output file for positive growth data: "<<SeventhO_outputFileName_ForPositiveGrowthModels<<endl;
					ofstream outFilePosSeventhO(SeventhO_outputFileName_ForPositiveGrowthModels);
					outFilePosSeventhO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 8){
					strcpy(EighthO_outputFileName_ForPositiveGrowthModels, EighthO_OutputFilePrefix);
					strcat(EighthO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of EighthO output file for positive growth data: "<<EighthO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosEighthO.open(EighthO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosEighthO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
				
				if(m2 == 9){
					strcpy(NinethO_outputFileName_ForPositiveGrowthModels, NinethO_OutputFilePrefix);
					strcat(NinethO_outputFileName_ForPositiveGrowthModels, outputFileName_ForPositiveGrowthModels);
					cout<<"Name of NinethO output file for positive growth data: "<<NinethO_outputFileName_ForPositiveGrowthModels<<endl;
					outFilePosNinethO.open(NinethO_outputFileName_ForPositiveGrowthModels, ios::out);
					outFilePosNinethO<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<setw(20)<<"logT"<<setw(20)<<"logP"<<setw(20)<<"logM"<<endl; //Header
				}
			}
		}
		
		////////////////////////////////////
		
		//Variables to keep track of stuff//
		
		LINAi = 0;
		
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
			if(inFile.is_open() && !MinimizeOutputToScreen){
				cout<<"LINA File was opened successfully"<<endl;
			}
			
			
			if(inFile.good()){
			
				if(!MinimizeOutputToScreen){
					
					cout<<"LINA File is being read..."<<endl;
					
					//reads LINA file into arrays
					getline(inFile,LINA_header);
					cout<<LINA_header<<endl;
					
					//Loop for number of modes on LINA data. Each iteration looks at a different mode and fills array to be used.
					for(int m3 = 0; m3 < NumOfModes; m3++){
						
						if(m3 == 0){
							inFile>>mode_FU[LINAi]>>period_FU[LINAi]>>growth_FU[LINAi];
							cout<<setw(3)<<mode_FU[LINAi]<<setw(17)<<period_FU[LINAi]<<setw(17)<<growth_FU[LINAi]<<endl;
							
							if(period_FU[LINAi] < LowestPeriodFU){
								LowestPeriodFU = period_FU[LINAi];
							}
							if(period_FU[LINAi] > HighestPeriodFU){
								HighestPeriodFU = period_FU[LINAi];
							}
						}
						
						if(m3 == 1){
							inFile>>mode_FO[LINAi]>>period_FO[LINAi]>>growth_FO[LINAi];
							cout<<setw(3)<<mode_FO[LINAi]<<setw(17)<<period_FO[LINAi]<<setw(17)<<growth_FO[LINAi]<<endl;
						}
						
						if(m3 == 2){
							inFile>>mode_2O[LINAi]>>period_2O[LINAi]>>growth_2O[LINAi];
							cout<<setw(3)<<mode_2O[LINAi]<<setw(17)<<period_2O[LINAi]<<setw(17)<<growth_2O[LINAi]<<endl;
						}
						
						if(m3 == 3){
							inFile>>mode_ThirdO[LINAi]>>period_ThirdO[LINAi]>>growth_ThirdO[LINAi];
							cout<<setw(3)<<mode_ThirdO[LINAi]<<setw(17)<<period_ThirdO[LINAi]<<setw(17)<<growth_ThirdO[LINAi]<<endl;
						}
						
						if(m3 == 4){
							inFile>>mode_ForthO[LINAi]>>period_ForthO[LINAi]>>growth_ForthO[LINAi];
							cout<<setw(3)<<mode_ForthO[LINAi]<<setw(17)<<period_ForthO[LINAi]<<setw(17)<<growth_ForthO[LINAi]<<endl;
						}
						
						if(m3 == 5){
							inFile>>mode_FifthO[LINAi]>>period_FifthO[LINAi]>>growth_FifthO[LINAi];
							cout<<setw(3)<<mode_FifthO[LINAi]<<setw(17)<<period_FifthO[LINAi]<<setw(17)<<growth_FifthO[LINAi]<<endl;
						}
						
						if(m3 == 6){
							inFile>>mode_SixthO[LINAi]>>period_SixthO[LINAi]>>growth_SixthO[LINAi];
							cout<<setw(3)<<mode_SixthO[LINAi]<<setw(17)<<period_SixthO[LINAi]<<setw(17)<<growth_SixthO[LINAi]<<endl;
						}
						
						if(m3 == 7){
							inFile>>mode_SeventhO[LINAi]>>period_SeventhO[LINAi]>>growth_SeventhO[LINAi];
							cout<<setw(3)<<mode_SeventhO[LINAi]<<setw(17)<<period_SeventhO[LINAi]<<setw(17)<<growth_SeventhO[LINAi]<<endl;
						}
						
						if(m3 == 8){
							inFile>>mode_EighthO[LINAi]>>period_EighthO[LINAi]>>growth_EighthO[LINAi];
							cout<<setw(3)<<mode_EighthO[LINAi]<<setw(17)<<period_EighthO[LINAi]<<setw(17)<<growth_EighthO[LINAi]<<endl;
						}
						
						if(m3 == 9){
							inFile>>mode_NinethO[LINAi]>>period_NinethO[LINAi]>>growth_NinethO[LINAi];
							cout<<setw(3)<<mode_NinethO[LINAi]<<setw(17)<<period_NinethO[LINAi]<<setw(17)<<growth_NinethO[LINAi]<<endl;
						}
					}
				}
				
				if(MinimizeOutputToScreen){
					//reads LINA file into arrays
					getline(inFile,LINA_header);
					//cout<<LINA_header<<endl;
					
					//Loop for number of modes on LINA data. Each iteration looks at a different mode and fills array to be used.
					for(int m3 = 0; m3 < NumOfModes; m3++){
						
						if(m3 == 0){
							inFile>>mode_FU[LINAi]>>period_FU[LINAi]>>growth_FU[LINAi];
							//cout<<setw(3)<<mode_FU[LINAi]<<setw(17)<<period_FU[LINAi]<<setw(17)<<growth_FU[LINAi]<<endl;
							
							if(period_FU[LINAi] < LowestPeriodFU){
								LowestPeriodFU = period_FU[LINAi];
							}
							if(period_FU[LINAi] > HighestPeriodFU){
								HighestPeriodFU = period_FU[LINAi];
							}
						}
						
						if(m3 == 1){
							inFile>>mode_FO[LINAi]>>period_FO[LINAi]>>growth_FO[LINAi];
							//cout<<setw(3)<<mode_FO[LINAi]<<setw(17)<<period_FO[LINAi]<<setw(17)<<growth_FO[LINAi]<<endl;
						}
						
						if(m3 == 2){
							inFile>>mode_2O[LINAi]>>period_2O[LINAi]>>growth_2O[LINAi];
							//cout<<setw(3)<<mode_2O[LINAi]<<setw(17)<<period_2O[LINAi]<<setw(17)<<growth_2O[LINAi]<<endl;
						}
						
						if(m3 == 3){
							inFile>>mode_ThirdO[LINAi]>>period_ThirdO[LINAi]>>growth_ThirdO[LINAi];
							//cout<<setw(3)<<mode_ThirdO[LINAi]<<setw(17)<<period_ThirdO[LINAi]<<setw(17)<<growth_ThirdO[LINAi]<<endl;
						}
						
						if(m3 == 4){
							inFile>>mode_ForthO[LINAi]>>period_ForthO[LINAi]>>growth_ForthO[LINAi];
							//cout<<setw(3)<<mode_ForthO[LINAi]<<setw(17)<<period_ForthO[LINAi]<<setw(17)<<growth_ForthO[LINAi]<<endl;
						}
						
						if(m3 == 5){
							inFile>>mode_FifthO[LINAi]>>period_FifthO[LINAi]>>growth_FifthO[LINAi];
							//cout<<setw(3)<<mode_FifthO[LINAi]<<setw(17)<<period_FifthO[LINAi]<<setw(17)<<growth_FifthO[LINAi]<<endl;
						}
						
						if(m3 == 6){
							inFile>>mode_SixthO[LINAi]>>period_SixthO[LINAi]>>growth_SixthO[LINAi];
							//cout<<setw(3)<<mode_SixthO[LINAi]<<setw(17)<<period_SixthO[LINAi]<<setw(17)<<growth_SixthO[LINAi]<<endl;
						}
						
						if(m3 == 7){
							inFile>>mode_SeventhO[LINAi]>>period_SeventhO[LINAi]>>growth_SeventhO[LINAi];
							//cout<<setw(3)<<mode_SeventhO[LINAi]<<setw(17)<<period_SeventhO[LINAi]<<setw(17)<<growth_SeventhO[LINAi]<<endl;
						}
						
						if(m3 == 8){
							inFile>>mode_EighthO[LINAi]>>period_EighthO[LINAi]>>growth_EighthO[LINAi];
							//cout<<setw(3)<<mode_EighthO[LINAi]<<setw(17)<<period_EighthO[LINAi]<<setw(17)<<growth_EighthO[LINAi]<<endl;
						}
						
						if(m3 == 9){
							inFile>>mode_NinethO[LINAi]>>period_NinethO[LINAi]>>growth_NinethO[LINAi];
							//cout<<setw(3)<<mode_NinethO[LINAi]<<setw(17)<<period_NinethO[LINAi]<<setw(17)<<growth_NinethO[LINAi]<<endl;
						}
					}
				}
				
				
				
				if(GetFirstNegativeGrowthRateModels){
					//if f0 is negative and last was positive, then output model parameters and assign T and L to blue edge arrays
					if(LINAi != 0){
						
						//loop for number of modes on blue edge data. Each interation looks at a different mode.
						for(int m4 = 0; m4 < NumOfModes; m4++){
							
							if(m4 == 0){
								if(growth_FU[LINAi] < 0. && growth_FU[LINAi-1] > 0. && period_FU[LINAi] < Higher_Period_Boundry && period_FU[LINAi] > Lower_Period_Boundry){
									
									outFileBlueFU<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FU[LINAi]<<setw(20)<<growth_FU[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FU[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 1){	
								if(growth_FO[LINAi] < 0. && growth_FO[LINAi-1] > 0. && period_FO[LINAi] < Higher_Period_Boundry && period_FO[LINAi] > Lower_Period_Boundry){
									
									outFileBlueFO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FO[LINAi]<<setw(20)<<growth_FO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}	
							
							if(m4 == 2){
								if(growth_2O[LINAi] < 0. && growth_2O[LINAi-1] > 0. && period_2O[LINAi] < Higher_Period_Boundry && period_2O[LINAi] > Lower_Period_Boundry){
									
									outFileBlue2O<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_2O[LINAi]<<setw(20)<<growth_2O[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_2O[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 3){
								if(growth_ThirdO[LINAi] < 0. && growth_ThirdO[LINAi-1] > 0. && period_ThirdO[LINAi] < Higher_Period_Boundry && period_ThirdO[LINAi] > Lower_Period_Boundry){
									
									outFileBlueThirdO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_ThirdO[LINAi]<<setw(20)<<growth_ThirdO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_ThirdO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 4){
								if(growth_ForthO[LINAi] < 0. && growth_ForthO[LINAi-1] > 0. && period_ForthO[LINAi] < Higher_Period_Boundry && period_ForthO[LINAi] > Lower_Period_Boundry){
									
									outFileBlueForthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_ForthO[LINAi]<<setw(20)<<growth_ForthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_ForthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 5){
								if(growth_FifthO[LINAi] < 0. && growth_FifthO[LINAi-1] > 0. && period_FifthO[LINAi] < Higher_Period_Boundry && period_FifthO[LINAi] > Lower_Period_Boundry){
									
									outFileBlueFifthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FifthO[LINAi]<<setw(20)<<growth_FifthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FifthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 6){
								if(growth_SixthO[LINAi] < 0. && growth_SixthO[LINAi-1] > 0. && period_SixthO[LINAi] < Higher_Period_Boundry && period_SixthO[LINAi] > Lower_Period_Boundry){
									
									outFileBlueSixthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_SixthO[LINAi]<<setw(20)<<growth_SixthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_SixthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 7){
								if(growth_SeventhO[LINAi] < 0. && growth_SeventhO[LINAi-1] > 0. && period_SeventhO[LINAi] < Higher_Period_Boundry && period_SeventhO[LINAi] > Lower_Period_Boundry){
									
									outFileBlueSeventhO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_SeventhO[LINAi]<<setw(20)<<growth_SeventhO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_SeventhO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 8){
								if(growth_EighthO[LINAi] < 0. && growth_EighthO[LINAi-1] > 0. && period_EighthO[LINAi] < Higher_Period_Boundry && period_EighthO[LINAi] > Lower_Period_Boundry){
									
									outFileBlueEighthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_EighthO[LINAi]<<setw(20)<<growth_EighthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_EighthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
							if(m4 == 9){
								if(growth_NinethO[LINAi] < 0. && growth_NinethO[LINAi-1] > 0. && period_NinethO[LINAi] < Higher_Period_Boundry && period_NinethO[LINAi] > Lower_Period_Boundry){
									
									outFileBlueNinethO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_NinethO[LINAi]<<setw(20)<<growth_NinethO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_NinethO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									BlueEdgei = BlueEdgei + 1;
									NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
								}
							}
							
						}
					}
				}
				
				if(GetPositiveGrowthRateModels){
					
					//if f0 is positive, add to file
					if(LINAi != 0){
						
						for(int m5 = 0; m5 < NumOfModes; m5++){
							
							if(m5 == 0){
								if(growth_FU[LINAi] > 0 && period_FU[LINAi] < Higher_Period_Boundry && period_FU[LINAi] > Lower_Period_Boundry){
									
									outFilePosFU<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FU[LINAi]<<setw(20)<<growth_FU[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FU[LINAi])<<setw(20)<<log(M[i])<<endl;
								
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}	
							}	
							
							if(m5 == 1){
								if(growth_FU[LINAi] > 0 && period_FU[LINAi] < Higher_Period_Boundry && period_FU[LINAi] > Lower_Period_Boundry){
									
									outFilePosFO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FO[LINAi]<<setw(20)<<growth_FO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
								
							if(m5 == 2){
								if(growth_2O[LINAi] > 0 && period_2O[LINAi] < Higher_Period_Boundry && period_2O[LINAi] > Lower_Period_Boundry){
									
									outFilePos2O<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_2O[LINAi]<<setw(20)<<growth_2O[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_2O[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 3){
								if(growth_ThirdO[LINAi] > 0 && period_ThirdO[LINAi] < Higher_Period_Boundry && period_ThirdO[LINAi] > Lower_Period_Boundry){
									
									outFilePosThirdO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_ThirdO[LINAi]<<setw(20)<<growth_ThirdO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_ThirdO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 4){
								if(growth_ForthO[LINAi] > 0 && period_ForthO[LINAi] < Higher_Period_Boundry && period_ForthO[LINAi] > Lower_Period_Boundry){
									
									outFilePosForthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_ForthO[LINAi]<<setw(20)<<growth_ForthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_ForthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 5){
								if(growth_FifthO[LINAi] > 0 && period_FifthO[LINAi] < Higher_Period_Boundry && period_FifthO[LINAi] > Lower_Period_Boundry){
									
									outFilePosFifthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_FifthO[LINAi]<<setw(20)<<growth_FifthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_FifthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 6){
								if(growth_SixthO[LINAi] > 0 && period_SixthO[LINAi] < Higher_Period_Boundry && period_SixthO[LINAi] > Lower_Period_Boundry){
									
									outFilePosSixthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_SixthO[LINAi]<<setw(20)<<growth_SixthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_SixthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 7){
								if(growth_SeventhO[LINAi] > 0 && period_SeventhO[LINAi] < Higher_Period_Boundry && period_SeventhO[LINAi] > Lower_Period_Boundry){
									
									outFilePosSeventhO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_SeventhO[LINAi]<<setw(20)<<growth_SeventhO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_SeventhO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 8){
								if(growth_EighthO[LINAi] > 0 && period_EighthO[LINAi] < Higher_Period_Boundry && period_EighthO[LINAi] > Lower_Period_Boundry){
									
									outFilePosEighthO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_EighthO[LINAi]<<setw(20)<<growth_EighthO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_EighthO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
							if(m5 == 9){
								if(growth_NinethO[LINAi] > 0 && period_NinethO[LINAi] < Higher_Period_Boundry && period_NinethO[LINAi] > Lower_Period_Boundry){
									
									outFilePosNinethO<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period_NinethO[LINAi]<<setw(20)<<growth_NinethO[LINAi]<<setw(20)<<log(T[i])<<setw(20)<<log(period_NinethO[LINAi])<<setw(20)<<log(M[i])<<endl;
									
									PosGrowthi = PosGrowthi + 1;
									NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
								}
							}
							
						}
					}
				}
				
				LINAi = LINAi + 1; //Incrementing LINAi
				
				inFile.close();
				
				if(!inFile.is_open() && !MinimizeOutputToScreen){
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
	
	cout<<"Period Range for FU mode: "<<LowestPeriodFU<<" < P < "<<HighestPeriodFU<<endl;
	
	////////////////////////////////////
	
	return 0;
}