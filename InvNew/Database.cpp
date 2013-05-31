// Database.cpp: implementation of the CDatabase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvNew.h"
#include "Database.h"

#include <fstream>
#include "DlgDtbImport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDatabase, CObject, 0)


CDatabase::~CDatabase()
{
	for( int i=0; i<m_profArray.GetSize(); i++ )
	{
		delete m_profArray[i];
	}
	m_profArray.RemoveAll();
}

CDtbProf* CDatabase::NewProfile(const char *strName)
{
	CString nStrID;
	nStrID.Format("%d", m_profArray.GetSize());	
	CString strID("Profile ");
	strID += nStrID;

	CDtbProf* pProfile = new CDtbProf(strName, strID);
	m_profArray.Add( pProfile );

	return pProfile;
}

void CDatabase::Serialize(CArchive &ar)
{
	if( ar.IsStoring() ) {
		ar << m_strID << m_strName << m_nCurProf;
	}
    else {
		ar >> m_strID >> m_strName >> m_nCurProf;
	}
	m_profArray.Serialize(ar);
	UpdateMinMax();
}

BOOL CDatabase::Import()
{
	CDtbProf* pProfile;
	CDtbProfChan* pChan[20];
	int i, nItems, nChan, nMaxItems, nCheck=0;
	BOOL bRe = FALSE;				// attention new channel created but not filled with data
	BOOL bNewChannels = TRUE;		// create new channels
	BOOL bCheck = FALSE;			// checking if values are loaded for initial dialog
	CString strProfName;

	CFileDialog dlg(TRUE, "*.xyz", "imp.xyz", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ASCII xyz file (*.xyz)|*.xyz||");
	if(dlg.DoModal() == IDOK) {
		std::ifstream file;
		file.open((LPCSTR) dlg.GetPathName(), std::ifstream::in );

		float a[20];		// imported valuse
		char strLine[256];

		while( (!file.eof()) && (bCheck == FALSE) ) {
			file.getline(strLine, 255);
			TRACE("line %s\n", strLine);
			if( (strstr(strLine, "Profile") || (strstr(strLine, "profile") ||
				strstr(strLine, "line")) || strstr(strLine, "line")) || 
				strstr(strLine, "//") || (strlen(strLine) == 0)) {

				if( strstr(strLine, "Profile") || strstr(strLine, "profile") ||
					strstr(strLine, "Line") || strstr(strLine, "line") ) 
				{
					bRe = TRUE;
				}
			}
			else  {
				if(bRe) {
					nItems = sscanf(strLine, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
						&a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9],
						&a[10], &a[11], &a[12], &a[13], &a[14], &a[15], &a[16], &a[17], &a[18], &a[19]);
					nMaxItems = nItems;
					bCheck = TRUE;
				}
			}
		}
		file.close();

		CDlgDtbImport idlg;
		CString str, nstr, fstr;

		str.Format("%-12d", 1);
		nstr.Format("%10.2f", a[0]);
		fstr = "x";
		str += nstr;
		idlg.m_chanSmpList.AddTail(str);
		idlg.m_chanNameList.AddTail(fstr);

		str.Format("%-12d", 2);
		nstr.Format("%10.2f", a[1]);
		fstr = "z";
		str += nstr;
		idlg.m_chanSmpList.AddTail(str);
		idlg.m_chanNameList.AddTail(fstr);
		for(i = 2; i < nMaxItems; i++) {
			str.Format("%-12d", i+1);
			nstr.Format("%10.2f", a[i]);
			fstr.Format("f%d", i-1);
			str += nstr;
			idlg.m_chanSmpList.AddTail(str);
			idlg.m_chanNameList.AddTail(fstr);
		}
		if(idlg.DoModal() == IDOK) {
			file.open((LPCSTR) dlg.GetPathName());
			while(!file.eof()) {
				file.getline(strLine, 255);
				if( (strstr(strLine, "Profile") || (strstr(strLine, "profile") ||
					strstr(strLine, "line")) || strstr(strLine, "line")) || 
					strstr(strLine, "/") || (strlen(strLine) == 0)) 
				{
					TRACE("no data : %s \n", strLine);
					if( strstr(strLine, "Profile") || strstr(strLine, "profile") ||
						strstr(strLine, "Line") || strstr(strLine, "line") ) 
					{
						bNewChannels = TRUE;
						strProfName = strLine;
						
					}
				}
				else {
					if(bNewChannels) {	// create new profile and channels
						nItems = sscanf(strLine, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
							&a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9],
							&a[10], &a[11], &a[12], &a[13], &a[14], &a[15], &a[16], &a[17], &a[18], &a[19]);
						// create new profile and add to database
						pProfile = NewProfile(strProfName);	//NewProfile("Imported Profile");
						nCheck = 0;
						// create new channels and add to profile
						for(i=0; i<nItems; i++) {
							pChan[i] = pProfile->NewChannel(idlg.m_chanNameList.GetAt(idlg.m_chanNameList.FindIndex(i)));	
						}
						bNewChannels = FALSE;
						nChan = nItems;		// for checking if there are no more items then channels
						TRACE("New profile with %d channels\n", nChan);
						TRACE("\nData : %s\n", strLine);
						// fill first row to datadase
						for(i = 0; i<nChan; i++) {
							pChan[i]->GetData()->Add(a[i]);
							TRACE("%f  ", a[i]);
						}

					} 
					else {		// data input
						nCheck++;
						//if(nCheck > 300 ) {
						//	AfxMessageBox("Demo only for 256 data points!", MB_OK | MB_ICONSTOP);
						//	AfxGetMainWnd()->SendMessage(WM_CLOSE);
						//}
						nItems = sscanf(strLine, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
							&a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6], &a[7], &a[8], &a[9],
							&a[10], &a[11], &a[12], &a[13], &a[14], &a[15], &a[16], &a[17], &a[18], &a[19]);
						TRACE("\nData : %s\n", strLine);
						if(nChan != nItems) {
							AfxMessageBox("Different number of items and channels");
						}
						for(i = 0; i<nChan; i++) {
							pChan[i]->GetData()->Add(a[i]);
							TRACE("%f  ", a[i]);
						}
					}
				}
			}
		}
		UpdateMinMax();
		return true;
	}
	
	return false;
}

BOOL CDatabase::Export()
{
	/*CFileDialog dlg(TRUE, "xzy", "export.xzy", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ASCII xyz file (*.xyz)|*.xyz|ASCII data file (*.dat)|*.dat||");
	if(dlg.DoModal() == IDOK) {
		ofstream file;
		file.open((LPCSTR) dlg.GetFileName());
		file.setmode();
		
		file << "//" << m_strName << endl;

		POSITION pos = m_profArray.GetHeadPosition();
		int il = 1;
		while(pos) {
			CDtbProf* pProf = m_profArray.GetNext(pos);
			file << "line " << il << endl;
			file << "//" << pProf->GetProfileName() << endl;
		
			CDtbProfChan* pChan[20];
			int nChan = pProf->GetChanList()->GetCount();
			POSITION pos2 = pProf->GetChanList()->GetHeadPosition();
			int i = 0;
			while(pos2) {
				pChan[i] = pProf->GetChanList()->GetNext(pos2);
				i++;
			}
			for(int j = 0; j<pProf->GetChanList()->GetHead()->GetChannelData()->GetCount(); j++) {	
				for(i = 0; i<nChan; i++) {
					POSITION pos3 = pChan[i]->GetChannelData()->FindIndex(j);
					if(pos3) {
						double a = pChan[i]->GetChannelData()->GetAt(pos3);
						file << a << " ";
						TRACE("%f  ", a);
					}
				}
				file << endl;
			}
			il++;
		}
	}
	*/
	return TRUE;
}

BOOL CDatabase::FindProfChanName(CString &strProfID, CString &strChanID)
{
/*	// input parameters are IDs of profile and channel
	// IDs are replaced on otput by names of channel and profile

	CDtbProf* pProf;

	POSITION pos1 = m_profArray.GetHeadPosition();
	while(pos1) {
		POSITION pos_pom = pos1;
		if(m_profArray.GetNext(pos1)->GetID() == strProfID) {
			pProf = m_profArray.GetAt(pos_pom);	// takes profile from profList
			strProfID = pProf->GetProfileName();
			break;
		}
	}

	ChanList* pChanList = pProf->GetChanList();
	POSITION pos2 = pChanList->GetHeadPosition();
	while(pos2) {
		POSITION pos_pom = pos2;
		if(pChanList->GetNext(pos2)->GetID() == strChanID) {
			strChanID = pChanList->GetAt(pos_pom)->GetChannelName();
			return TRUE;
		}
	}
*/
	return FALSE;
}

CDtbProf* CDatabase::GetProfile(const char *strProfID)
{
	int n;
	CDtbProf *pProf;

	if( !IsEmpty() ) {
		n = (int)m_profArray.GetSize();
		for( int i=0; i<n; i++) {
			pProf = m_profArray[i];
			if( strProfID == pProf->GetID() )
				return pProf;
		}
	}

	return NULL;
}

ProfileArray* CDatabase::GetProfArray() 
{
	if( !IsEmpty() )
		return &m_profArray;
	else
		return NULL;
}

CDtbProf* CDatabase::GetNextProfile()
{
	if( IsEmpty() ) {
		if( m_nCurProf+1 < m_profArray.GetSize() ) {
			m_nCurProf++; 
			return m_profArray[m_nCurProf];
		}
		else
			return m_profArray[m_nCurProf];
	}

	return NULL;
}

CDtbProf* CDatabase::GetPrevProfile()
{
	if( IsEmpty() ) {
		if( m_nCurProf-1 > 0 ) {
			m_nCurProf--; 
			return m_profArray[m_nCurProf];
		}
		else
			return m_profArray[m_nCurProf];
	}

	return NULL;
}

CDtbProf* CDatabase::GetFirstProfile()
{
	if( m_profArray.GetUpperBound() > 0 ) {
		CDtbProf* pProf = m_profArray.GetAt( 0 );
		return pProf;
	}

	return NULL;
}

CDtbProf* CDatabase::GetLastProfile()
{
	if( m_profArray.GetUpperBound() > 0 ) {
		CDtbProf* pProf = m_profArray.GetAt( m_profArray.GetUpperBound() );
		return pProf;
	}

	return NULL;
}

bool CDatabase::IsEmpty()
{
	int n = (int)m_profArray.GetSize();
	if( m_profArray.GetSize() <= 0 )
		return true;

	return false;
}

int CDatabase::GetNumOfProfs(void)
{
	if( !IsEmpty() ) {
		return (int) m_profArray.GetSize();
	}
	return -1;
}

CDtbProf* CDatabase::GetProfile(int n)
{
	if( !IsEmpty() ) {
		if( n < m_profArray.GetSize() && n>=0 ) {
			return m_profArray[n];
		}
	}
	return NULL;
}

int CDatabase::UpdateMinMax(void)
{
	int n;
	CDtbProf *pProf;

	if( !IsEmpty() ) {
		n = (int)m_profArray.GetSize();
		for( int i=0; i<n; i++) {
			pProf = m_profArray[i];
			pProf->UpdateMinMax();
		}
	}

	return 0;
}

int CDatabase::GetMinMax(int nChan, double &min, double &max)
{
	int n;
	CDtbProf *pProf;
	bool bInit = false;
	double minT, maxT;

	if( !IsEmpty() ) {
		n = (int)m_profArray.GetSize();
		for( int i=0; i<n; i++) {
			pProf = m_profArray[i];
			pProf->GetChannel( nChan )->GetMinMaxValue( minT, maxT);
			if( !bInit ) {
				min = minT; max = maxT; bInit = true;
			}
			else  {
				min = min(min, minT);
				max = max(max, maxT);
			}
		}
	}

	return 0;
}
