// ProfileChannel.cpp: implementation of the CDtbProfChan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvNew.h"
#include "DatabaseProfileChannel.h"
#include  <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDtbProfChan, CObject, 0)

CDtbProfChan::~CDtbProfChan()
{
}

void CDtbProfChan::Serialize(CArchive &ar)
{
	if( ar.IsStoring() ) {
		ar << m_strID << m_strName << m_bVisible << m_dMin << m_dMax;
	}
    else {
		ar >> m_strID >> m_strName >> m_bVisible >> m_dMin >> m_dMax;
	}
	m_data.Serialize(ar);
}


BOOL CDtbProfChan::GetMinMaxValue(double &min, double &max)
{
	min = m_dMin;
	max = m_dMax;

	return TRUE;
}

void CDtbProfChan::UpdateMinMax()
{
	int n;
	double x;

	n = (int) m_data.GetSize();
	m_dMin = m_data[0];
	m_dMax = m_data[0];
	for( int i = 1; i < n; i++ ) {
		x = m_data[i];
		if(x < m_dMin) {
			m_dMin = x;		}
		if(x > m_dMax) {
			m_dMax = x;		}
	}
}