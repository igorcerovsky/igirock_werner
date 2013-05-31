// ScaleBar.h: interface for the CScaleBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEBAR_H__988B3F97_8663_47DC_91DC_9CB1A5A44C18__INCLUDED_)
#define AFX_SCALEBAR_H__988B3F97_8663_47DC_91DC_9CB1A5A44C18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum ScaleBarClk {
	SB_NON,
	SB_MIN,
	SB_MAX,
	SB_MIDDLE
};

class CMyScaleBar : public CObject  
{
public:
	CMyScaleBar();
	CMyScaleBar(int nType);
	virtual ~CMyScaleBar();

public:
	void Zoom(CPoint lPt, BOOL bIn=TRUE);
	void Zoom(double nin, double max);
	void SetType(int nType)		{m_nType = nType;}
	void GetDataExtent(double &min, double &max);
	CRect GetBoundRect() {return CRect(m_nL, m_nT, m_nR, m_nB);}
	void SetMaxDataExtent(double min, double max);
	void Move(CPoint lPt);
	void Scale();
	int IsClicked(CPoint lPt);
	void SetDataExtend(double min, double max);
	void SetDrawRect(int left, int up, int right, int bottom);
	void Draw(CDC *pDC);

protected:
	int 	m_nType;		// csale bar type horizontal or vertical
	double	m_dMinData;		// minimum data extent
	double	m_dMaxData;		
	double	m_dMin;			// current minimum
	double	m_dMax;			// current maximum
	int		m_nMinExt;		// in logical coordinates
	int		m_nMaxExt;
	int		m_nMin;
	int		m_nMax;
	int		m_ndl;			// difference for moving the position
	int		m_ndr;
	double  m_sc;			// scale
	double	m_off;			// offset
	int		m_nT, m_nL, m_nB, m_nR;		//in logical coordinates top, left,bottom, right
	int		m_nClk;
	double	m_dZoomIn;		//	zoom ratio
	double	m_dZoomOut;		//

	// drawing
	CBrush	m_brushH1, m_brushH2, m_brushV1, m_brushV2;
	CBitmap m_bmpH1, m_bmpH2, m_bmpV1, m_bmpV2;
public:
	void GetScOff(double& sc, double& off);
	void GetDataMinMax(double& min, double& max);
	int ZoomToMax(void);
};

#endif // !defined(AFX_SCALEBAR_H__988B3F97_8663_47DC_91DC_9CB1A5A44C18__INCLUDED_)
