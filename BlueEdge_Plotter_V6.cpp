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
	int NumOfSpaces = 1; // Number of spaces between each models output - Currently off
	
	char inputFileName[30] = "InputDataFULL.dat"; //File with the model parameters
	char outputFileName_ForBlueEdge[50] = "BlueEdge_SetD_PeriodRange1.dat"; //File with model parameters on the blue edge
	char outputFileName_ForPositiveGrowthModels[50] = "PostiveGrowth_SetD_PeriodRange1.dat"; //File with model parameters that have positve growth rates
	char LINAFileName[30] = "LINA_period_growth.data"; //Normally should be "LINA_period_growth.data"
	char log_directory_prefix[30] = "Raw_LINA_data/SetD/SetD/LOGS_"; //Prefix to log_directory, suffix is model number. This is where LINA file should be
	
	bool GetPositiveGrowthRateModels = true; // Positive Growth. When true, postive growth rate models will be outputted to a file.
	bool GetFirstNegativeGrowthRateModels = true; // Blue Edge. When true, the first negative growth rate models will be outputted to a file (For blue edge!)
	bool GetOnlyModelsWithDefinedPeriodRange = true; // Period Range. When true, gets only models within the defined period range.
	
	double Lower_Period_Boundry = 1; //Low bound to period range in days. Only is used when GetOnlyModelsWithDefinedPeriodRange is true 
	double Higher_Period_Boundry = 4; //High bound to period range in days. Only is used when GetOnlyModelsWithDefinedPeriodRange is true
	
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
	
	///////////////////////////////////////////////////////////////
	
	//Main program//
		
	////////////////////////////////////////////////////////////
	
	//Getting models with any period//
	if(!GetOnlyModelsWithDefinedPeriodRange){
	
		if(GetFirstNegativeGrowthRateModels){
			cout<<"Creating output file for Blue Edge"<<endl;
			ofstream outFileBlue(outputFileName_ForBlueEdge);
			outFileBlue<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<endl; //Header
			//outFile<<""<<endl;
		}
		
		if(GetPositiveGrowthRateModels){
			cout<<"Creating output file for Positive Growth"<<endl;
			ofstream outFilePos(outputFileName_ForPositiveGrowthModels);
			outFilePos<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<endl; //Header
			//outFile<<""<<endl;
		}
		
		////////////////////////////////////
		
		//Variables to keep track of stuff//
		
		int LINAi = 0;
		
		if(GetFirstNegativeGrowthRateModels){
			int BlueEdgei = 0;
			int NumOfFailedModels = 0;
			int NumOfBlueEdgePoints = 0;
			
			double LowestTemp_Blue = 10000;
			double HighestTemp_Blue = 0;
			double LowestLum_Blue = 10000;
			double HighestLum_Blue = 0;
		}
		
		if(GetPositiveGrowthRateModels){
			int PosGrowthi = 0;
			int NumOfFailedModels = 0;
			int NumOfPosGrowthPoints = 0;
			
			double LowestTemp_Pos = 10000;
			double HighestTemp_Pos = 0;
			double LowestLum_Pos = 10000;
			double HighestLum_Pos = 0;
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
			
				//loop that reads LINA file into temp arrays, saving L and T of input file if Blue Edge 
				getline(inFile,LINA_header);
				cout<<LINA_header<<endl;
				inFile>>mode[LINAi]>>period[LINAi]>>growth[LINAi];
				cout<<setw(3)<<mode[LINAi]<<setw(17)<<period[LINAi]<<setw(17)<<growth[LINAi]<<endl;
				
				
				// for(int l = 1; l < NumOfModes; l++){
					
					// inFile>>mode[i]>>period[i]>>growth[i];
					
					// cout<<setw(3)<<mode[i]<<setw(17)<<period[i]<<setw(17)<<growth[i]<<endl;
				
					
				// }
				
				
				if(GetFirstNegativeGrowthRateModels){
					//if f0 is negative and last was positive, then output model parameters and assign T and L to blue edge arrays
					if(LINAi != 0){
						
						if(growth[LINAi] < 0. && growth[LINAi-1] > 0.){
							
							outFileBlue<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period[LINAi]<<setw(20)<<growth[LINAi]<<endl;
							
							L_Blue[BlueEdgei] = L[i];
							T_Blue[BlueEdgei] = T[i];
							
							//Selectors to keep track of graphics limits
							if(L_Blue[BlueEdgei] > HighestLum_Blue){
								HighestLum_Blue = L_Blue[BlueEdgei];
							}
							if(L_Blue[BlueEdgei] < LowestLum_Blue){
								LowestLum_Blue = L_Blue[BlueEdgei];
							}
							
							if(T_Blue[BlueEdgei] > HighestTemp_Blue){
								HighestTemp_Blue = T_Blue[BlueEdgei];
							}
							if(T_Blue[BlueEdgei] < LowestTemp_Blue){
								LowestTemp_Blue = T_Blue[BlueEdgei];
							}
							
							BlueEdgei = BlueEdgei + 1;
							NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
						}	
					}
				}
				
				if(GetPositiveGrowthRateModels){
					
					//if f0 is positive, add to file
					if(LINAi != 0){
						
						if(growth[LINAi] > 0){
							
							outFilePos<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period[LINAi]<<setw(20)<<growth[LINAi]<<endl;
							
							L_Blue[PosGrowthi] = L[i];
							T_Blue[PosGrowthi] = T[i];
							
							//Selectors to keep track of graphics limits
							if(L_Blue[PosGrowthi] > HighestLum_Pos){
								HighestLum_Pos = L_Blue[PosGrowthi];
							}
							if(L_Blue[PosGrowthi] < LowestLum_Pos){
								LowestLum_Pos = L_Blue[PosGrowthi];
							}
							
							if(T_Blue[PosGrowthi] > HighestTemp_Pos){
								HighestTemp_Pos = T_Blue[PosGrowthi];
							}
							if(T_Blue[PosGrowthi] < LowestTemp_Pos){
								LowestTemp_Pos = T_Blue[PosGrowthi];
							}
							
							PosGrowthi = PosGrowthi + 1;
							NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
						}	
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
	}

		
	///////////////////////////////////////////////
	
	//Getting models within defined period range//
	if(GetOnlyModelsWithDefinedPeriodRange){
	
		if(GetFirstNegativeGrowthRateModels){
			cout<<"Creating output file for Blue Edge"<<endl;
			ofstream outFileBlue(outputFileName_ForBlueEdge);
			outFileBlue<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<endl; //Header
			//outFile<<""<<endl;
		}
		
		if(GetPositiveGrowthRateModels){
			cout<<"Creating output file for Positive Growth"<<endl;
			ofstream outFilePos(outputFileName_ForPositiveGrowthModels);
			outFilePos<<setw(5)<<"Model"<<setw(20)<<"z"<<setw(20)<<"x"<<setw(20)<<"M"<<setw(20)<<"L"<<setw(20)<<"T"<<setw(20)<<"Lin Period"<<setw(20)<<"Growth rate"<<endl; //Header
			//outFile<<""<<endl;
		}
		
		////////////////////////////////////
		
		//Variables to keep track of stuff//
		
		int LINAi = 0;
		
		if(GetFirstNegativeGrowthRateModels){
			int BlueEdgei = 0;
			int NumOfFailedModels = 0;
			int NumOfBlueEdgePoints = 0;
			
			double LowestTemp_Blue = 10000;
			double HighestTemp_Blue = 0;
			double LowestLum_Blue = 10000;
			double HighestLum_Blue = 0;
		}
		
		if(GetPositiveGrowthRateModels){
			int PosGrowthi = 0;
			int NumOfFailedModels = 0;
			int NumOfPosGrowthPoints = 0;
			
			double LowestTemp_Pos = 10000;
			double HighestTemp_Pos = 0;
			double LowestLum_Pos = 10000;
			double HighestLum_Pos = 0;
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
			
				//loop that reads LINA file into temp arrays, saving L and T of input file if Blue Edge 
				getline(inFile,LINA_header);
				cout<<LINA_header<<endl;
				inFile>>mode[LINAi]>>period[LINAi]>>growth[LINAi];
				cout<<setw(3)<<mode[LINAi]<<setw(17)<<period[LINAi]<<setw(17)<<growth[LINAi]<<endl;
				
				
				// for(int l = 1; l < NumOfModes; l++){
					
					// inFile>>mode[i]>>period[i]>>growth[i];
					
					// cout<<setw(3)<<mode[i]<<setw(17)<<period[i]<<setw(17)<<growth[i]<<endl;
				
					
				// }
				
				
				if(GetFirstNegativeGrowthRateModels){
					//if f0 is negative and last was positive, then output model parameters and assign T and L to blue edge arrays
					if(LINAi != 0){
						
						if(growth[LINAi] < 0. && growth[LINAi-1] > 0. && period[LINAi] < Higher_Period_Boundry && period[LINAi] > Lower_Period_Boundry){
							
							outFileBlue<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period[LINAi]<<setw(20)<<growth[LINAi]<<endl;
							
							L_Blue[BlueEdgei] = L[i];
							T_Blue[BlueEdgei] = T[i];
							
							//Selectors to keep track of graphics limits
							if(L_Blue[BlueEdgei] > HighestLum_Blue){
								HighestLum_Blue = L_Blue[BlueEdgei];
							}
							if(L_Blue[BlueEdgei] < LowestLum_Blue){
								LowestLum_Blue = L_Blue[BlueEdgei];
							}
							
							if(T_Blue[BlueEdgei] > HighestTemp_Blue){
								HighestTemp_Blue = T_Blue[BlueEdgei];
							}
							if(T_Blue[BlueEdgei] < LowestTemp_Blue){
								LowestTemp_Blue = T_Blue[BlueEdgei];
							}
							
							BlueEdgei = BlueEdgei + 1;
							NumOfBlueEdgePoints = NumOfBlueEdgePoints + 1;
						}	
					}
				}
				
				if(GetPositiveGrowthRateModels){
					
					//if f0 is positive, add to file
					if(LINAi != 0){
						
						if(growth[LINAi] > 0 && period[LINAi] < Higher_Period_Boundry && period[LINAi] > Lower_Period_Boundry){
							
							outFilePos<<setw(5)<<Mod[i]<<setw(20)<<z[i]<<setw(20)<<x[i]<<setw(20)<<M[i]<<setw(20)<<L[i]<<setw(20)<<T[i]<<setw(20)<<period[LINAi]<<setw(20)<<growth[LINAi]<<endl;
							
							L_Blue[PosGrowthi] = L[i];
							T_Blue[PosGrowthi] = T[i];
							
							//Selectors to keep track of graphics limits
							if(L_Blue[PosGrowthi] > HighestLum_Pos){
								HighestLum_Pos = L_Blue[PosGrowthi];
							}
							if(L_Blue[PosGrowthi] < LowestLum_Pos){
								LowestLum_Pos = L_Blue[PosGrowthi];
							}
							
							if(T_Blue[PosGrowthi] > HighestTemp_Pos){
								HighestTemp_Pos = T_Blue[PosGrowthi];
							}
							if(T_Blue[PosGrowthi] < LowestTemp_Pos){
								LowestTemp_Pos = T_Blue[PosGrowthi];
							}
							
							PosGrowthi = PosGrowthi + 1;
							NumOfPosGrowthPoints = NumOfPosGrowthPoints + 1;
						}	
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
	
	////////////////////////////////////////
	
	
	return 0;
}