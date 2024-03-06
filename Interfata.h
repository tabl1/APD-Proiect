/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: OnMainPanel */
#define  PANEL_GRAPH_RAW                  2       /* control type: graph, callback function: (none) */
#define  PANEL_GRAPH_FILTERED             3       /* control type: graph, callback function: (none) */
#define  PANEL_PREV                       4       /* control type: command, callback function: PREV */
#define  PANEL_NEXT                       5       /* control type: command, callback function: NEXT */
#define  PANEL_FILTRU                     6       /* control type: ring, callback function: (none) */
#define  PANEL_Aplica                     7       /* control type: command, callback function: Buton_Filtrari */
#define  PANEL_TEXTMSG                    8       /* control type: textMsg, callback function: (none) */
#define  PANEL_Anvelopa                   9       /* control type: command, callback function: Anvelopa */
#define  PANEL_COMMANDBUTTON_4            10      /* control type: command, callback function: OnLoadButtonCB */
#define  PANEL_TEXTMSG_2                  11      /* control type: textMsg, callback function: (none) */
#define  PANEL_NUMERIC_MEAN               12      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MIN                13      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MAX                14      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_DISPERSIE          15      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MEDIANA            16      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_TRECERI            17      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_INDEX_MAX          18      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_INDEX_MIN          19      /* control type: numeric, callback function: (none) */
#define  PANEL_STOP                       20      /* control type: numeric, callback function: (none) */
#define  PANEL_Kurtosis                   21      /* control type: numeric, callback function: (none) */
#define  PANEL_SKEW                       22      /* control type: numeric, callback function: (none) */
#define  PANEL_START                      23      /* control type: numeric, callback function: (none) */
#define  PANEL_HISTOGRAM                  24      /* control type: graph, callback function: (none) */
#define  PANEL_ALPHA                      25      /* control type: numeric, callback function: (none) */
#define  PANEL_SAVE                       26      /* control type: command, callback function: SAVE */
#define  PANEL_SWITCH_PANEL               27      /* control type: binary, callback function: OnSwitchPanelCB */

#define  PANEL_FREQ                       2       /* callback function: OnFrPanel */
#define  PANEL_FREQ_SPECTER_GRAPH_3       2       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_SPECTER_GRAPH_2       3       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_WINDOWING_GRAPH_3     4       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_SPECTER_GRAPH         5       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_WINDOWING_GRAPH_2     6       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_POWER_PEAK_3          7       /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_WINDOWING_GRAPH       8       /* control type: graph, callback function: (none) */
#define  PANEL_FREQ_SWITCH_PANEL_FREQ     9       /* control type: binary, callback function: OnSwitchPanelCB */
#define  PANEL_FREQ_POWER_PEAK_2          10      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_FREQ_PEAK_3           11      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_POWER_PEAK            12      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_FILTRU_FEREASTRA      13      /* control type: ring, callback function: (none) */
#define  PANEL_FREQ_Aplica_2              14      /* control type: command, callback function: EveryNSamples */
#define  PANEL_FREQ_Aplica                15      /* control type: command, callback function: Aplica */
#define  PANEL_FREQ_FREQ_PEAK_2           16      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_FREQ_PEAK             17      /* control type: numeric, callback function: (none) */
#define  PANEL_FREQ_NSAMPLES              18      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Anvelopa(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Aplica(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Buton_Filtrari(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK EveryNSamples(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NEXT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnMainPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PREV(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SAVE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif