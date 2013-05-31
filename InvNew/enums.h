typedef enum WhatFilter{
	WF_ALL = 0,
	WF_SELECTED = 1
};

typedef struct MinMaxStepInt{
	int min;
	int max;
	int step;
} MMSI;		// start step end

typedef struct MinMaxStepDouble{
	double min;
	double max;
	double step;
} MMSD;		// start step end

typedef struct MinMaxInt{
	int min;
	int max;
} MINMAXI;

typedef struct MinMaxDouble{
	double min;
	double max;
} MINMAXD;


typedef struct FiltSruct{
	WhatFilter whatFilter;
	double	 dCondNum;
	double	 dAltMax;
	double	 dAltMin;
	double	 dChiSq;
	int		 nIpEnd;
	int		 nIpStart;
	double	 dSiEnd;
	double	 dSiStart;
	int		 nWwEnd;
	double	 dWwEndPos;
	int		 nWwStart;
	double	 dWwStartPos;
}  FILTSTRCT;


