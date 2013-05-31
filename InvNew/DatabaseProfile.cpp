// Profile.cpp: implementation of the CDtbProf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DatabaseProfile.h"
#include "DatabaseProfileChannel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDtbProf, CObject, 0)



CDtbProf::~CDtbProf()
{
	for( int i=0; i<m_chanArray.GetSize(); i++ )
	{
		delete m_chanArray[i];
	}
	m_chanArray.RemoveAll();
}

void CDtbProf::Serialize(CArchive &ar)
{
	if( ar.IsStoring() ) {
		ar << m_strID << m_strName << m_nChan << m_depthRange.min << m_depthRange.max;
	}
    else {
		ar >> m_strID >> m_strName >> m_nChan >> m_depthRange.min >> m_depthRange.max;
	}
	m_chanArray.Serialize(ar);
}

CDtbProfChan* CDtbProf::NewChannel(const char* strName)
{
	CString n_strID;
	n_strID.Format("%d", m_chanArray.GetSize());	
	CString strID("Channel ");
	strID += n_strID;

	CDtbProfChan* pChannel = new CDtbProfChan(strName, strID);
	m_chanArray.Add(pChannel);
	return pChannel;
}

CDtbProfChan* CDtbProf::GetChannel(CString strName)
{
	int n;
	CDtbProfChan *pChan;

	n = (int) m_chanArray.GetSize();
	for( int i=0; i<n; i++) {
		pChan = m_chanArray[i];
		if( pChan->GetName() == strName )
			return pChan;
	}

	return NULL;
}

CDtbProfChan* CDtbProf::GetChannelID(CString strID)
{
	int n;
	CDtbProfChan *pChan;

	n = (int) m_chanArray.GetSize();
	for( int i=0; i<n; i++) {
		pChan = m_chanArray[i];
		if( pChan->GetID() == strID )
			return pChan;
	}

	return NULL;
}

int CDtbProf::GetChannelNum(CString strName)
{
	int n;
	CDtbProfChan *pChan;

	n = (int) m_chanArray.GetSize();
	for( int i=0; i<n; i++) {
		pChan = m_chanArray[i];
		CString str;
		str = pChan->GetName();
		if( str == strName )
			return i;
	}

	return NULL;
}


ChannnelArray* CDtbProf::GetChanArray()
{
	if( m_chanArray.GetSize() > 1 ) 
		return &m_chanArray;
	else
		return NULL;
}

int	CDtbProf::GetNumberOfChannels()
{
	return (int) m_chanArray.GetSize();
}

int	CDtbProf::GetSize()		
{
	return m_chanArray[0]->GetSize();
}


bool CDtbProf::SetData(double *pData, int n)
{
	return false;
}

bool CDtbProf::SetData(CArray<double, double>	*pData, CString strChanName)
{
	CDtbProfChan	*pChan;
	 CArray<double, double>	*pChanData; 

	pChan = GetChannel( strChanName );
	if( pChan != NULL ) {
		int res = AfxMessageBox("Channel already exists! Overwrite?", MB_YESNO | MB_ICONQUESTION);
		if( res == IDNO ) {
			AfxMessageBox(" Set Data canceled. ", MB_OK | MB_ICONINFORMATION );
			return false;
		}
	}
	// create new channel
	else {
		pChan = NewChannel( strChanName );
	}

	pChanData = pChan->GetData();
	//if( n == pChanData->GetSize() ) {
	//}
	//else {
	//	AfxMessageBox(" Size doesn't match. ", MB_OK | MB_ICONSTOP );
	//	return;
	//}

	// copy data
	pChanData->RemoveAll();
	pChanData->Append( *pData );
	pChan->UpdateMinMax();
	return true;
}

CDtbProfChan* CDtbProf::GetChannel(int i)
{
	if( m_chanArray.GetSize() >= 1 && m_chanArray.GetSize() > i ) 
		return m_chanArray.GetAt( i );
	else
		return NULL;
}

int CDtbProf::UpdateMinMax(void)
{
	int n;
	CDtbProfChan *pChan;

	n = (int) m_chanArray.GetSize();
	for( int i=0; i<n; i++) {
		pChan = m_chanArray[i];
		pChan->UpdateMinMax();
	}

	return 0;
}
