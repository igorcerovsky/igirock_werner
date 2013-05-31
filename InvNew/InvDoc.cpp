// InvDoc.cpp : implementation of the CInvDoc class
//

#include "stdafx.h"
#include "InvNew.h"

#include "InvDoc.h"
#include "InvView.h"
#include "DlgWerner.h"
#include "Meth2DLinWerner.h"
#include "DlgSelectChannels.h"
#include "DlgEditMethod.h"
#include "RglrzDlg.h"
#include "DlgFilterResults.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int ThreadClustering(LPVOID pParam)
{
	ComputeStrct* pStrct;

	pStrct = (ComputeStrct*) pParam;
	CMeth2DLinWerner* pWer = (CMeth2DLinWerner*) pStrct->pMethod;
	pWer->Clustering();

	pStrct->pDoc->ThreadFinished();
	delete pStrct;
	return 0;
}


// CInvDoc

IMPLEMENT_DYNCREATE(CInvDoc, CDocument)

BEGIN_MESSAGE_MAP(CInvDoc, CDocument)
	ON_COMMAND(ID_DATABASE_IMPORT, OnDatabaseImport)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_IMPORT, OnUpdateDatabaseImport)
	ON_COMMAND(ID_METHOD_WERNER, OnMethodWerner)
	ON_COMMAND(ID_VIEW_SETCHANNELS, OnViewSetChannels)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SETCHANNELS, OnUpdateViewSetChannels)
	ON_COMMAND(ID_EDIT_REMOVEHIDEN, OnEditRemovehiden)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REMOVEHIDEN, OnUpdateEditRemovehiden)
	ON_COMMAND(ID_EDIT_METHOD, OnEditMethod)
	ON_UPDATE_COMMAND_UI(ID_EDIT_METHOD, OnUpdateEditMethod)
	ON_COMMAND(ID_METHOD_EXPORT, OnMethodExport)
	ON_UPDATE_COMMAND_UI(ID_METHOD_EXPORT, OnUpdateMethodExport)
	ON_COMMAND(ID_DATABASE_PROFILE_FIRST, OnDatabaseProfileFirst)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_PROFILE_FIRST, OnUpdateDatabaseProfileFirst)
	ON_COMMAND(ID_DATABASE_PROFILE_LAST, OnDatabaseProfileLast)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_PROFILE_LAST, OnUpdateDatabaseProfileLast)
	ON_COMMAND(ID_DATABASE_PROFILE_NEXT, OnDatabaseProfileNext)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_PROFILE_NEXT, OnUpdateDatabaseProfileNext)
	ON_COMMAND(ID_DATABASE_PROFILE_PREV, OnDatabaseProfilePrev)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_PROFILE_PREV, OnUpdateDatabaseProfilePrev)
	ON_COMMAND(ID_DATABASE_COMPUTE, OnDatabaseCompute)
	ON_UPDATE_COMMAND_UI(ID_DATABASE_COMPUTE, OnUpdateDatabaseCompute)
	ON_COMMAND(ID_EDIT_DELETE_METHOD, OnEditDeleteMethod)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE_METHOD, OnUpdateEditDeleteMethod)
	ON_COMMAND(ID_METHOD_FILTER_RESULTS, OnMethodFilterResults)
	ON_UPDATE_COMMAND_UI(ID_METHOD_FILTER_RESULTS, OnUpdateMethodFilterResults)
END_MESSAGE_MAP()


// CInvDoc construction/destruction

CInvDoc::CInvDoc()
{
	m_nCurProf = 0;
	m_pMethSel = NULL;
	m_nMethID = 0;
	m_pThread = NULL;
}

CInvDoc::~CInvDoc()
{
	for( int i=0; i<m_methArray.GetSize(); i++ )
	{
		delete m_methArray[i];
	}
	m_methArray.RemoveAll();
}

BOOL CInvDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CInvDoc serialization

void CInvDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nCurProf << m_strPos << m_strElev << m_strFld << m_nMethID;
	}
	else
	{
		ar >> m_nCurProf >> m_strPos >> m_strElev >> m_strFld >> m_nMethID; 
	}
	m_dtb.Serialize( ar );
	m_methArray.Serialize( ar );
}


// CInvDoc diagnostics

#ifdef _DEBUG
void CInvDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInvDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CInvDoc commands

void CInvDoc::OnDatabaseImport()
{
	m_dtb.Import();
	if( !m_dtb.IsEmpty() ) {
		SelectChannels();
		POSITION pos = GetFirstViewPosition();
		while(pos) {
			CInvView *pView = (CInvView*) this->GetNextView( pos );
			pView->OnInitialUpdate();
		}
	}
	SetModifiedFlag();
}

void CInvDoc::OnUpdateDatabaseImport(CCmdUI *pCmdUI)
{
	if( m_dtb.IsEmpty() ) 
		pCmdUI->Enable();
	else
		pCmdUI->Enable( false );
}

bool CInvDoc::IsEmpty(void)
{
	return m_dtb.IsEmpty();
}

bool CInvDoc::IsCurProfileOK(void)
{
	if( !m_dtb.IsEmpty()) {
		if( m_nCurProf >= 0  && m_nCurProf < m_dtb.GetNumOfProfs() ) {
			return true;}
	}
	return false;
}

CDtbProf* CInvDoc::GetCurProf(void)
{
	if( IsCurProfileOK() ) {
		return m_dtb.GetProfile( m_nCurProf );
	}
	return NULL;
}

void CInvDoc::UpdateAllViews(void)
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL) {
		CView* pView = GetNextView(pos);
		pView->Invalidate(FALSE);
	}   
}

void CInvDoc::OnMethodWerner()
{
	// if computing thread is running
	if( m_pThread != NULL ) {
		m_pThread->SuspendThread();
		int ret = AfxMessageBox("The computation is currently running.\n Stop computation?", MB_YESNO | MB_ICONQUESTION);
		if( ret == IDYES ) {
			m_pThread->SuspendThread();
			m_pThread->Delete();
			m_pThread = NULL;
		}
		else {
			m_pThread->ResumeThread();
			return;
		}
	}

	if( !m_dtb.IsEmpty() ) {
		CDlgWerner	dlg;
		MMSI ww, ip;
		MMSD prof;
		MINMAXD depth;
		double *pX=NULL, *pZ=NULL, *pF=NULL;
		int n;
		
		dlg.m_pDtb = &m_dtb;
		dlg.m_strCurProf = m_dtb.GetProfile( m_nCurProf )->GetID();
		dlg.m_strPos = m_strPos;
		dlg.m_strFld = m_strFld;
		dlg.m_strElev = m_strElev;
		if( dlg.DoModal() == IDOK ) {
			ww.min = dlg.m_nWwStart;
			ww.max = dlg.m_nWwEnd;
			ww.step = dlg.m_nWwStep;
			ip.min = dlg.m_nIpStart;
			ip.max = dlg.m_nIpEnd;
			depth.min = dlg.m_dMinElev;
			depth.max = dlg.m_dMaxElev;

			if(dlg.m_pChanZ != NULL) {
				//dlg.m_pChanZ->GetMinMaxValue( depth.min, depth.max );
				pZ = dlg.m_pChanZ->GetData()->GetData();
			}
			pX = dlg.m_pChanX->GetData()->GetData();
			dlg.m_pChanX->GetMinMaxValue( prof.min, prof.max );
			pF = dlg.m_pChanY->GetData()->GetData();
			n = dlg.m_nNProfPoints;

			CString strID;
			strID.Format("Werner %d", m_nMethID);
			CMeth2DLinWerner *pWr = new CMeth2DLinWerner( strID, dlg.m_strSolutionName, ip, ww, depth, prof );
			CString str = dlg.m_pProf->GetID();
			pWr->SetProfID( str );
			pWr->SetDataPointers(pX, pZ, pF, NULL, NULL, n);
			m_methArray.Add( pWr );
			ComputeStrct *pStrct = new ComputeStrct;
			pStrct->pDoc = this; pStrct->pMethod = pWr;
			m_pThread = AfxBeginThread( (AFX_THREADPROC)ThreadWerner, pStrct, THREAD_PRIORITY_NORMAL);
			SetModifiedFlag();
		}
	}
}

int CInvDoc::Clustering(CMeth2DLin* pMeth)
{
	// if computing thread is running
	if( m_pThread != NULL ) {
		m_pThread->SuspendThread();
		int ret = AfxMessageBox("The computation is currently running.\n Stop computation?", MB_YESNO | MB_ICONQUESTION);
		if( ret == IDYES ) {
			m_pThread->SuspendThread();
			m_pThread->Delete();
			m_pThread = NULL;
		}
		else {
			m_pThread->ResumeThread();
			return 0;
		}
	}

	if( pMeth != NULL ) {
		ComputeStrct *pStrct = new ComputeStrct;
		pStrct->pDoc = this; pStrct->pMethod = pMeth;
		m_pThread =  AfxBeginThread( (AFX_THREADPROC)ThreadClustering, pStrct, THREAD_PRIORITY_NORMAL);
	}
	else {
		AfxMessageBox("Select a method.");
	}
	return 0;
}

void CInvDoc::ThreadFinished(void)
{
	m_pThread = NULL;
	UpdateAllViews();
	SetModifiedFlag();
}

void CInvDoc::SelectChannels(void)
{
	CDlgSelectChannels dlg;

	dlg.m_pDtb = &m_dtb;
	if( dlg.DoModal() == IDOK ) {
		m_strPos = dlg.m_strPos;
		m_strElev = dlg.m_strElev;
		m_strFld = dlg.m_strFld;
	}
}

void CInvDoc::OnViewSetChannels()
{
	SelectChannels();
}

void CInvDoc::OnUpdateViewSetChannels(CCmdUI *pCmdUI)
{
	if( m_dtb.IsEmpty() ) 
		pCmdUI->Enable( false );
	else
		pCmdUI->Enable();
}

void CInvDoc::OnEditRemovehiden()
{
	if( m_pMethSel != NULL ) {
		m_pMethSel->RemoveSolutions();
		UpdateAllViews();
	}
}

void CInvDoc::OnUpdateEditRemovehiden(CCmdUI *pCmdUI)
{
	if( m_methArray.IsEmpty() ) 
		pCmdUI->Enable( false );
	else
		pCmdUI->Enable();
}

void CInvDoc::OnEditMethod()
{
	CDlgEditMethod dlg;

	dlg.m_pMethArray = &m_methArray;
	dlg.m_pMeth = m_pMethSel;
	dlg.m_pDoc = this;
	dlg.m_nCurProf = m_nCurProf;
	dlg.m_strCurProfID = m_dtb.GetProfile( m_nCurProf )->GetID();
	
	dlg.DoModal();

}

void CInvDoc::OnUpdateEditMethod(CCmdUI *pCmdUI)
{
	if( m_methArray.IsEmpty() ) 
		pCmdUI->Enable( false );
	else
		pCmdUI->Enable();
}

void CInvDoc::OnMethodExport()
{
	if( m_pMethSel != NULL) {
		m_pMethSel->Export();
	}
	else
		OnEditMethod();
}

void CInvDoc::OnUpdateMethodExport(CCmdUI *pCmdUI)
{
	if( m_methArray.IsEmpty() ) 
		pCmdUI->Enable( false );
	else
		pCmdUI->Enable();
}

void CInvDoc::OnDatabaseProfileFirst()
{
	SetCurrentProfile( 0 );
	UpdateAllViews();
}

void CInvDoc::OnUpdateDatabaseProfileFirst(CCmdUI *pCmdUI)
{
	if( m_dtb.IsEmpty() ) 		pCmdUI->Enable( false );
	else						pCmdUI->Enable();
}

void CInvDoc::OnDatabaseProfileLast()
{
	SetCurrentProfile( m_dtb.GetNumOfProfs()-1 );
	UpdateAllViews();
}

void CInvDoc::OnUpdateDatabaseProfileLast(CCmdUI *pCmdUI)
{
	if( m_dtb.IsEmpty() ) 		pCmdUI->Enable( false );
	else						pCmdUI->Enable();
}

void CInvDoc::OnDatabaseProfileNext()
{
	SetCurrentProfile( m_nCurProf+1 );
	UpdateAllViews();
}

void CInvDoc::OnUpdateDatabaseProfileNext(CCmdUI *pCmdUI)
{
	if( m_dtb.IsEmpty() ) 		pCmdUI->Enable( false );
	else						pCmdUI->Enable();
}

void CInvDoc::OnDatabaseProfilePrev()
{
	SetCurrentProfile( m_nCurProf-1 );
	UpdateAllViews();
}

void CInvDoc::OnUpdateDatabaseProfilePrev(CCmdUI *pCmdUI)
{
	if( m_dtb.IsEmpty() ) 		pCmdUI->Enable( false );
	else						pCmdUI->Enable();
}

bool CInvDoc::SetCurrentProfile(int n)
{
	if( n >= 0 && n < m_dtb.GetNumOfProfs() ) {
		m_nCurProf = n;
		return true;
	}
	return false;
}

void CInvDoc::OnDatabaseCompute()
{
	CDlgReglrz dlg;

	dlg.m_pDtb = &m_dtb;
	dlg.m_strCurProf = m_dtb.GetProfile( m_nCurProf )->GetID();
	dlg.m_strPos = m_strPos;
	dlg.m_strFld = m_strFld;
	dlg.DoModal();
	UpdateAllViews();
}

void CInvDoc::OnUpdateDatabaseCompute(CCmdUI *pCmdUI)
{
	if( m_dtb.IsEmpty() ) 		pCmdUI->Enable( false );
	else						pCmdUI->Enable();
}

void CInvDoc::OnEditDeleteMethod()
{
	for( int i=0; i<m_methArray.GetSize(); i++ ) {
		CMeth2DLin* pMeth = m_methArray.GetAt(i);
		if( pMeth == m_pMethSel ) {
			CString str = "Are you sure, you want do delete method ";
			str += pMeth->GetName() +"?";
			int res = AfxMessageBox(str , MB_YESNO | MB_ICONQUESTION);
			if( res == IDYES ) {
				m_methArray.RemoveAt( i );
				delete pMeth;
				m_pMethSel = NULL;
				UpdateAllViews();
				return;
			}
		}
	}
}

void CInvDoc::OnUpdateEditDeleteMethod(CCmdUI *pCmdUI)
{
	if( m_pMethSel != NULL )	pCmdUI->Enable();
	else						pCmdUI->Enable(false);
}


void CInvDoc::OnCloseDocument()
{
	if( m_pThread != NULL ) {
		m_pThread->SuspendThread();
		int ret = AfxMessageBox("The computation is currently running.\n Close anyway?", MB_YESNO | MB_ICONQUESTION);
		if( ret == IDYES ) {
			m_pThread->Delete();
			m_pThread = NULL;
		}
		else {
			m_pThread->ResumeThread();
			return;
		}
	}
	
	CDocument::OnCloseDocument();
}

void CInvDoc::OnMethodFilterResults()
{
	CDlgFilterResults dlg;

	dlg.m_pDoc = this;
	dlg.m_pMeth = m_pMethSel;

	if(dlg.DoModal() == IDOK) {
	}
}

void CInvDoc::OnUpdateMethodFilterResults(CCmdUI *pCmdUI)
{
	if(m_pMethSel)	pCmdUI->Enable();
	else			pCmdUI->Enable(FALSE);
}
