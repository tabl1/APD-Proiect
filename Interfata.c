#include <advanlys.h>
//#include <analysis.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Interfata.h"
static int exit_panel;


//==============================================================================
// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
double* axisArray = 0;
ssize_t* hist = 0;
double *waveDataFiltrat = 0; //aici o sa stocam datele filtrate
double *waveDataDeAfisareRawSecunda = 0;
int globalPageNumber = 1;
static int panelFreq;
static int panelHandle;
int start = 0;
int stop = 1;
int nSamplesWindowing = 0;
char unit[999] = "V";
int switched = 0;
int tick_number = 1;
void Ferestruire(int panel,int nsamples,int start,int stop);

double* signal_chebyshev1 = 0;
double* signal_chebyshev2 = 0;

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			// activarea butoanelor,etc
			SetCtrlAttribute (panel, PANEL_Anvelopa, ATTR_DIMMED, 0);
			SetCtrlAttribute (panel, PANEL_FILTRU, ATTR_DIMMED, 0);
			SetCtrlAttribute (panelHandle, PANEL_Aplica, ATTR_DIMMED, 0);
			SetCtrlAttribute (panelHandle, PANEL_ALPHA, ATTR_DIMMED, 0);
			//executa script python pentru conversia unui fisierului .wav in .txt
			//LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			//Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//npoints = sampleRate * 6;
			
			// Atentie!: se schimba si lungimea pe care o consideram (samplerate * 6) 
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			//pentru calcularea ultimelor 6 secunde
			
			int l,k = 0;
			double* waveDataIntermediar = 0;
			waveDataIntermediar = (double *) calloc(npoints-sampleRate*4, sizeof(double));
			for(l=sampleRate*4;l<npoints;l++){
				waveDataIntermediar[k] = waveData[l];
				k++;
			}
			free(waveData);
			waveData = waveDataIntermediar;
			npoints = sampleRate * 6 ; 
			
			
			//afisare pe graf pentru intreg intervalul
			//DeleteGraphPlot(panel,PANEL_GRAPH_RAW,-1,VAL_IMMEDIATE_DRAW);
			//PlotY(panel, PANEL_GRAPH_RAW  , waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			//pentru afisarea in intervale de o secunda
			
			int startValue;
			int stopValue;
			GetCtrlVal(panel,PANEL_START,&startValue);
			GetCtrlVal(panel,PANEL_STOP,&stopValue);
			DeleteGraphPlot(panel,PANEL_GRAPH_RAW,-1,VAL_IMMEDIATE_DRAW);
			waveDataDeAfisareRawSecunda = (double *) calloc(sampleRate * stopValue, sizeof(double));
			k = 0;
			int i;
			for(i=sampleRate * startValue;i<sampleRate * stopValue;i++){
			waveDataDeAfisareRawSecunda[k] = waveData[i];
			k++;
			}
			DeleteGraphPlot(panel,PANEL_GRAPH_RAW,-1,VAL_IMMEDIATE_DRAW);
			PlotY(panel, PANEL_GRAPH_RAW , waveDataDeAfisareRawSecunda,k, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			//free(waveDataDeAfisareRawSecunda);		
			nSamplesWindowing = npoints;
			//int i;
			double index_min = 0,index_max = 0;
			double media;
			double mediana,dispersie,treceri_zero;
			double min,max;
			min = 99000;
			max = -99000;
			media = 0;
			treceri_zero = 0;
			for(i=0;i<npoints;i++){
				if(waveData[i] < min){
					min = waveData[i];
					index_min = i;
				}
				if(waveData[i] > max){
					max = waveData[i];
					index_max = i;
				}
				if(i < npoints - 1){// pana la penultimul element
					if((waveData[i] >=0 && waveData[i+1] < 0) || (waveData[i] < 0 && waveData[i+1] >= 0)){
						treceri_zero++;
					}
				}
			}
			//verificam trecerea prin 0 al ultimului element
			i=npoints;
			if((waveData[i-1] >=0 && waveData[i-2] < 0) || (waveData[i-1] < 0 && waveData[i-2] >= 0)){
				treceri_zero++;
			}
			SetCtrlVal(panel,PANEL_NUMERIC_MIN,min);
			SetCtrlVal(panel,PANEL_NUMERIC_MAX,max);
			Variance(waveData,npoints,&media,&dispersie);
			SetCtrlVal(panel,PANEL_NUMERIC_MEAN,media);
			Median(waveData,npoints,&mediana);
			SetCtrlVal(panel,PANEL_NUMERIC_MEDIANA,mediana);
			SetCtrlVal(panel,PANEL_NUMERIC_DISPERSIE,dispersie);
			SetCtrlVal(panel,PANEL_NUMERIC_TRECERI ,treceri_zero);
			SetCtrlVal(panel,PANEL_NUMERIC_INDEX_MIN ,index_min);
			SetCtrlVal(panel,PANEL_NUMERIC_INDEX_MAX ,index_max);
			
			//histograma pentru data filtrata
			axisArray = (double *) calloc(npoints,sizeof(double));
			hist = (ssize_t *) calloc(npoints,sizeof(ssize_t));
			
    	    Histogram(&waveData[0],npoints,min,max,hist,axisArray,20);
			
			DeleteGraphPlot(panel,PANEL_HISTOGRAM,-1,VAL_IMMEDIATE_DRAW);
			PlotXY(panel, PANEL_HISTOGRAM , axisArray, hist,npoints, VAL_DOUBLE, VAL_SSIZE_T,VAL_VERTICAL_BAR,VAL_SOLID_SQUARE, VAL_SOLID, 1, VAL_BLUE);
			
			free(hist);
			free(axisArray);
			
			break;
	}
	return 0;
}

int CVICALLBACK OnMainPanel (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (exit_panel);
			break;
	}
	return 0;
}

int CVICALLBACK OnFrPanel (int panel, int event, void *callbackData,
						   int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (exit_panel);
			break;
	}
	return 0;
}

int CVICALLBACK Buton_Filtrari (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			// filtrarile
			char tip_filtru[128] = {0};
			GetCtrlVal(panel,PANEL_FILTRU,tip_filtru);
			if(strcmp(tip_filtru, "Mediere 16") == 0){
				waveDataFiltrat = (double *) calloc(npoints, sizeof(double));
				/*
				// 
				for(i=0;i<npoints-16;i++){
					medie = 0;
					for(j=i;j<i+16;j++){
						medie += waveData[j];
					}
					waveDataFiltrat[i] = medie/16;
				}
				for(i=npoints-16;i<npoints;i++){
					waveDataFiltrat[i] = waveData[i];
				}
				*/
				int i,j,k;
				double medie = 0;
				waveDataFiltrat[0] = waveData[0];
				j=2;
				for(i=1;i<15;i++){
					medie = 0;
					for(k=0;k<=i;k++){
						medie += waveData[k];
					}
					waveDataFiltrat[i] = medie / j;
					j++;
				}
				
				for(i = 16;i<npoints-16;i++){
					medie = medie - waveData[i-16] + waveData[i];
					waveDataFiltrat[i] = medie / 16;
				}
				//Skewness si kurtosis:
				double skew,kurt;
				
				Moment(waveDataFiltrat,256,3,&skew);
				Moment(waveDataFiltrat,256,4,&kurt);
				SetCtrlVal(panel,PANEL_SKEW,skew);
				SetCtrlVal(panel,PANEL_Kurtosis,kurt);
				
				//pentru afisare pe tot intervalul:
				//DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				//PlotY(panel, PANEL_GRAPH_FILTERED , waveDataFiltrat,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
				//pentru afisarea in intervale de o secunda
				
				int startValue;
				int stopValue;
				GetCtrlVal(panel,PANEL_START,&startValue);
				GetCtrlVal(panel,PANEL_STOP,&stopValue);
				DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				double *waveDataFiltratDeAfisare = 0;
				waveDataFiltratDeAfisare = (double *) calloc(sampleRate * stopValue, sizeof(double));
			    k = 0;
				for(i=sampleRate * startValue;i<sampleRate * stopValue;i++){
					waveDataFiltratDeAfisare[k] = waveDataFiltrat[i];
					k++;
				}
				DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				PlotY(panel, PANEL_GRAPH_FILTERED , waveDataFiltratDeAfisare,k, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				free(waveDataFiltratDeAfisare);
				
			}
			if(strcmp(tip_filtru, "Mediere 32") == 0){
				waveDataFiltrat = (double *) calloc(npoints, sizeof(double));
				/*
				// 
				for(i=0;i<npoints-16;i++){
					medie = 0;
					for(j=i;j<i+16;j++){
						medie += waveData[j];
					}
					waveDataFiltrat[i] = medie/16;
				}
				for(i=npoints-16;i<npoints;i++){
					waveDataFiltrat[i] = waveData[i];
				}
				*/
				int i,j,k;
				double medie = 0;
				waveDataFiltrat[0] = waveData[0];
				j=2;
				for(i=1;i<31;i++){
					medie = 0;
					for(k=0;k<=i;k++){
						medie += waveData[k];
					}
					waveDataFiltrat[i] = medie / j;
					j++;
				}
				
				for(i = 32;i<npoints-32;i++){
					medie = medie - waveData[i-32] + waveData[i];
					waveDataFiltrat[i] = medie / 32;
				}
				//Skewness si kurtosis:
				double skew,kurt;
						  
				Moment(waveDataFiltrat,256,3,&skew);
				Moment(waveDataFiltrat,256,4,&kurt);
				SetCtrlVal(panel,PANEL_SKEW,skew);
				SetCtrlVal(panel,PANEL_Kurtosis,kurt);
				
				//pentru afisare pe tot intervalul:
				//DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				//PlotY(panel, PANEL_GRAPH_FILTERED , waveDataFiltrat,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
				//pentru afisarea in intervale de o secunda
				
				int startValue;
				int stopValue;
				GetCtrlVal(panel,PANEL_START,&startValue);
				GetCtrlVal(panel,PANEL_STOP,&stopValue);
				DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				double *waveDataFiltratDeAfisare = 0;
				waveDataFiltratDeAfisare = (double *) calloc(sampleRate * stopValue, sizeof(double));
			    k = 0;
				for(i=sampleRate * startValue;i<sampleRate * stopValue;i++){
					waveDataFiltratDeAfisare[k] = waveDataFiltrat[i];
					k++;
				}
				DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				PlotY(panel, PANEL_GRAPH_FILTERED , waveDataFiltratDeAfisare,k, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				free(waveDataFiltratDeAfisare);
				
			}
			if(strcmp(tip_filtru, "Alpha") == 0){
				double alpha = 0;
				GetCtrlVal(panel,PANEL_ALPHA,&alpha);
				
				int i;
				waveDataFiltrat = (double *) calloc(npoints, sizeof(double));
				waveDataFiltrat[0] = waveData[0]; // sau 0
				for(i=1;i<npoints-1;i++){
					waveDataFiltrat[i] = (1-alpha)*waveDataFiltrat[i-1]+alpha*waveData[i];
				}
				waveDataFiltrat[npoints-1] = waveData[npoints-1];
				//Skewness si kurtosis:
				
				
				//pentru afisare pe tot intervalul:
				//DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				//PlotY(panel, PANEL_GRAPH_FILTERED , waveDataFiltrat,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
				//pentru afisarea in intervale de o secunda
				double skew,kurt;
				int startValue;
				int stopValue;
				GetCtrlVal(panel,PANEL_START,&startValue);
				GetCtrlVal(panel,PANEL_STOP,&stopValue);
				
				Moment(&waveData[startValue],256,3,&skew);
				Moment(&waveData[startValue],256,4,&kurt);
				SetCtrlVal(panel,PANEL_SKEW,skew);
				SetCtrlVal(panel,PANEL_Kurtosis,kurt);
				DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				double *waveDataFiltratDeAfisare = 0;
				waveDataFiltratDeAfisare = (double *) calloc(sampleRate * stopValue, sizeof(double));
				int k = 0;
				for(i=sampleRate * startValue;i<sampleRate * stopValue;i++){
					waveDataFiltratDeAfisare[k] = waveDataFiltrat[i];
					k++;
				}
				DeleteGraphPlot(panel,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				PlotY(panel, PANEL_GRAPH_FILTERED , waveDataFiltratDeAfisare,k, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				free(waveDataFiltratDeAfisare);
				
			}
			free(waveDataFiltrat);
		
			break;
	}
	return 0;
}

int CVICALLBACK Anvelopa (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			// salvez in Anvelopa.txt waveData-ul doar cu ultimele 6 secunde!
			//ArrayToFile ("semnal.txt", waveData, VAL_DOUBLE, npoints, 1,
              //           VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS,
                //         VAL_CONST_WIDTH, 10, 1, VAL_TRUNCATE);
			// datele din Anvelopa.txt vor fi utilizate pentru transformata Hilbert
			//executa script python pentru transformata Hilbert
			//LaunchExecutable("python Hilbert.py");
			
			
			//Delay(8);
			
			double *AnvelopaData = 0;
			AnvelopaData = (double *) calloc(npoints, sizeof(double));
			FileToArray("anvelopa.txt", AnvelopaData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//anvelopa pe tot intervalul
			//PlotY(panel, PANEL_GRAPH_RAW  , AnvelopaData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			//PlotY(panel, PANEL_GRAPH_RAW  , waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			//anvelopa pe interval de o secunda
			
			int i;
			int startValue;
			int stopValue;
			GetCtrlVal(panel,PANEL_START,&startValue);
			GetCtrlVal(panel,PANEL_STOP,&stopValue);
			DeleteGraphPlot(panel,PANEL_GRAPH_RAW,-1,VAL_IMMEDIATE_DRAW);
			double *waveDataDeAfisare = 0;
			waveDataDeAfisare = (double *) calloc(sampleRate * stopValue, sizeof(double));
			int k = 0;
			for(i=sampleRate * startValue;i<sampleRate * stopValue;i++){
				waveDataDeAfisare[k] = AnvelopaData[i];
				k++;
			}
			PlotY(panel, PANEL_GRAPH_RAW , waveDataDeAfisare,k, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			PlotY(panel, PANEL_GRAPH_RAW  , waveDataDeAfisareRawSecunda, 22050, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			free(waveDataDeAfisare);
			free(waveDataDeAfisareRawSecunda);
			
			free(AnvelopaData);
			break;
	}
	return 0;
}

int CVICALLBACK PREV (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int stopValue;
			int startValue;
			GetCtrlVal(panel,PANEL_START,&startValue);
			GetCtrlVal(panel,PANEL_STOP,&stopValue);
			stopValue -= 1;
			startValue -= 1;
			if(stopValue > 0){
				SetCtrlVal(panel,PANEL_START,startValue);
				SetCtrlVal(panel,PANEL_STOP,stopValue);
				start = startValue;
				stop = stopValue;
				
			}
			break;
	}
	return 0;
}

int CVICALLBACK NEXT (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int stopValue;
			int startValue;
			GetCtrlVal(panel,PANEL_START,&startValue);
			GetCtrlVal(panel,PANEL_STOP,&stopValue);
			stopValue += 1;
			startValue += 1;
			if(startValue < 6){
				SetCtrlVal(panel,PANEL_START,startValue);
				SetCtrlVal(panel,PANEL_STOP,stopValue);
				start = startValue;
				stop = stopValue;
			}
			break;
	}
	return 0;
}

int CVICALLBACK SAVE (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int bitmapID;
			char buf[100];
			// Pentru Raw Data
			sprintf(buf, "GraficRaw%d.jpg",globalPageNumber); 
			GetCtrlDisplayBitmap(panel, PANEL_GRAPH_RAW, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, buf, JPEG_PROGRESSIVE, 100);
			
			// Pentru Filtered Data
			sprintf(buf, "GraficFiltered%d.jpg",globalPageNumber); 
			GetCtrlDisplayBitmap(panel, PANEL_GRAPH_FILTERED, 1, &bitmapID);
			SaveBitmapToJPEGFile(bitmapID, buf, JPEG_PROGRESSIVE, 100);
			globalPageNumber++;

			break;
	}
	return 0;
}



int CVICALLBACK Aplica (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	int nsamples;
	GetCtrlVal(panel,PANEL_FREQ_NSAMPLES,&nsamples);
	start = 0;
	stop = 1;
	switch (event)
	{
		case EVENT_COMMIT:
			Ferestruire(panel,nsamples,start,stop);
			break;
	}
	return 0;
}

int CVICALLBACK EveryNSamples (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int nsamples;
	GetCtrlVal(panel,PANEL_FREQ_NSAMPLES,&nsamples);
	switch (event)
	{
		case EVENT_COMMIT:
			if(stop<=6 && start <=5){
				stop++;
				start++;
				Ferestruire(panel,nsamples,start,stop);
			}
			break;
	}
	return 0;
}

void Ferestruire(int panel,int nsamples,int start,int stop)
{
	char* window_type;
	window_type = (char *) calloc(50,sizeof(char));
	double* semnal_ferestruit = (double *) calloc(nsamples,sizeof(double));
	int k = 0;
	for(int j = nsamples*start;j<nsamples*stop;j++){
		semnal_ferestruit[k] = waveData[j];
		k++;
		}
	GetCtrlVal(panel,PANEL_FREQ_FILTRU_FEREASTRA,window_type);
	if(strcmp(window_type, "1") == 0){
		TriWin(semnal_ferestruit,k);
	}else{
		BkmanWin(semnal_ferestruit,k);
	}
	
	DeleteGraphPlot(panel, PANEL_FREQ_WINDOWING_GRAPH, -1 ,VAL_IMMEDIATE_DRAW);
	PlotY(panel, PANEL_FREQ_WINDOWING_GRAPH, semnal_ferestruit, nsamples,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	
	
	//COPIE WAVEDATA!!!!
	double* waveData_Copy = 0;
	waveData_Copy = (double *) calloc(npoints, sizeof(double));
	int m = 0;
	for(int j = nsamples*start;j<nsamples*stop; j++){
		waveData_Copy[m] = waveData[j];
		m++;
	}
	
	WindowConst window_const;
	WindowConst window_constC1;
	WindowConst window_constC2;
	
	if(strcmp(window_type, "1") == 0){
		ScaledWindowEx(waveData_Copy,m,TRIANGLE,1,&window_const);
	}else{
		ScaledWindowEx(waveData_Copy,m,BLKMAN,1,&window_const);
	}
	
	double* autoSpectrum = (double *) calloc(m / 2, sizeof(double));
	double* autoSpectrumC1 = (double *) calloc(m / 2, sizeof(double));
	double* autoSpectrumC2 = (double *) calloc(m / 2, sizeof(double));
	
	double* convertedSpectrum = (double *) calloc(m / 2, sizeof(double));
	double* convertedSpectrumC1 = (double *) calloc(m / 2, sizeof(double));
	double* convertedSpectrumC2 = (double *) calloc(m / 2, sizeof(double));
	
	//frecventa estimata pentru spectrul de putere (maxim) din vectorul autoSpectrum
	double powerPeak = 0.0;
	double powerPeakC1 = 0.0;
	double powerPeakC2 = 0.0;
	
    //valoarea maxima din spectru de putere (din autoSpectrum)
	double freqPeak = 0.0;
	double freqPeakC1 = 0.0;
	double freqPeakC2 = 0.0;
	
    //variabila ce reprezinta pasul in domeniul frecventei
	double df = 0.0;
	double dfC1 = 0.0;
	double dfC2 = 0.0;
	
	AutoPowerSpectrum(waveData_Copy, m, 1.0/sampleRate, autoSpectrum, &df);
	PowerFrequencyEstimate(autoSpectrum, m / 2,-1,window_const,df,7,&freqPeak,&powerPeak);
	SetCtrlVal(panel, PANEL_FREQ_POWER_PEAK, powerPeak);
	SetCtrlVal(panel, PANEL_FREQ_FREQ_PEAK, freqPeak);
	SpectrumUnitConversion(autoSpectrum,m / 2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df,window_const ,convertedSpectrum , unit);
	DeleteGraphPlot(panel, PANEL_FREQ_SPECTER_GRAPH, -1 ,VAL_IMMEDIATE_DRAW);
	PlotWaveform( panel, PANEL_FREQ_SPECTER_GRAPH, convertedSpectrum, m/2 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,
                 VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
	
	//chebyshev 1!!!!!
	signal_chebyshev1 = (double *) calloc(m, sizeof(double));
	m = 0;
	for(int j = start*nsamples; j < nsamples*stop; j++){
		waveData_Copy[m] = waveData[j];
		m++;
	}
	Ch_BPF(waveData_Copy,m,sampleRate,nsamples/3,2*nsamples/3,0.1,5,signal_chebyshev1);
	DeleteGraphPlot(panel, PANEL_FREQ_WINDOWING_GRAPH_2, -1 ,VAL_IMMEDIATE_DRAW);
	PlotY(panel, PANEL_FREQ_WINDOWING_GRAPH_2, signal_chebyshev1, m,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	
	if(strcmp(window_type, "1") == 0){
		ScaledWindowEx(signal_chebyshev1,m,TRIANGLE,1,&window_constC1);
	}else{
		ScaledWindowEx(signal_chebyshev1,m,BLKMAN,1,&window_constC1);
	}
	
	AutoPowerSpectrum(signal_chebyshev1, m, 1.0/sampleRate, autoSpectrumC1, &dfC1);
	PowerFrequencyEstimate(autoSpectrumC1, m / 2,-1,window_constC1,dfC1,7,&freqPeakC1,&powerPeakC1);
	SetCtrlVal(panel, PANEL_FREQ_POWER_PEAK_2, powerPeakC1);
	SetCtrlVal(panel, PANEL_FREQ_FREQ_PEAK_2, freqPeakC1);
	SpectrumUnitConversion(autoSpectrumC1,m / 2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, dfC1,window_const ,convertedSpectrumC1 , unit);
	DeleteGraphPlot(panel, PANEL_FREQ_SPECTER_GRAPH_2, -1 ,VAL_IMMEDIATE_DRAW);
	PlotWaveform( panel, PANEL_FREQ_SPECTER_GRAPH_2, convertedSpectrumC1, m/2 ,VAL_DOUBLE, 1.0, 0.0, 0.0, dfC1,
                 VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
	
	//chebyshev 2!!!!!
	signal_chebyshev2 = (double *) calloc(m, sizeof(double));
	InvCh_BPF(waveData_Copy,m,sampleRate,nsamples/3,2*nsamples/3,40,5,signal_chebyshev2);//FRECVENTA NYQUIST/2
	DeleteGraphPlot(panel, PANEL_FREQ_WINDOWING_GRAPH_3, -1 ,VAL_IMMEDIATE_DRAW);
	PlotY(panel, PANEL_FREQ_WINDOWING_GRAPH_3, signal_chebyshev2, m,VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	
	if(strcmp(window_type, "1") == 0){
		ScaledWindowEx(signal_chebyshev2,m,TRIANGLE,1,&window_constC2);
	}else{
		ScaledWindowEx(signal_chebyshev2,m,BLKMAN,1,&window_constC2);
	}
	
	AutoPowerSpectrum(signal_chebyshev2, m, 1.0/sampleRate, autoSpectrumC2, &dfC2);
	PowerFrequencyEstimate(autoSpectrumC2, m / 2,-1,window_constC2,dfC2,7,&freqPeakC2,&powerPeakC2);
	SetCtrlVal(panel, PANEL_FREQ_POWER_PEAK_3, powerPeakC2);
	SetCtrlVal(panel, PANEL_FREQ_FREQ_PEAK_3, freqPeakC2);
	SpectrumUnitConversion(autoSpectrumC2,m / 2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, dfC2,window_const ,convertedSpectrumC2 , unit);
	DeleteGraphPlot(panel, PANEL_FREQ_SPECTER_GRAPH_3, -1 ,VAL_IMMEDIATE_DRAW);
	PlotWaveform( panel, PANEL_FREQ_SPECTER_GRAPH_3, convertedSpectrumC1, m/2 ,VAL_DOUBLE, 1.0, 0.0, 0.0, dfC2,
                 VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_DK_GREEN);
	
	free(semnal_ferestruit);
	free(signal_chebyshev1);
	free(signal_chebyshev2);
	free(autoSpectrum);
	free(autoSpectrumC1);
	free(autoSpectrumC2);
	free(convertedSpectrum);
	free(convertedSpectrumC1);
	free(convertedSpectrumC2);
	free(waveData_Copy);
	free(window_type);
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == panelHandle)//daca suntep pe primul panou
			{
				SetCtrlVal(panelFreq, PANEL_FREQ_SWITCH_PANEL_FREQ, 1);//setam valoare de pe al 2-lea pe 1
				DisplayPanel(panelFreq);
				HidePanel(panel);
				switched = 1;
			}
			else
			{
				SetCtrlVal(panelHandle, PANEL_SWITCH_PANEL, 0);
				DisplayPanel(panelHandle);
				HidePanel(panel);
				switched = 0;
			}
			break;
	}
	return 0;
}


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Interfata.uir", PANEL)) < 0)
		return -1;
	if ((panelFreq = LoadPanel (0, "Interfata.uir", PANEL_FREQ)) < 0)
		return -1;
	SetCtrlAttribute (panelHandle, PANEL_Anvelopa, ATTR_DIMMED, 1);
	SetCtrlAttribute (panelHandle, PANEL_FILTRU, ATTR_DIMMED, 1); 
	SetCtrlAttribute (panelHandle, PANEL_Aplica, ATTR_DIMMED, 1);
	SetCtrlAttribute (panelHandle, PANEL_ALPHA, ATTR_DIMMED, 1);
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}









