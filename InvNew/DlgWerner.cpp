// DlgWerner.cpp : implementation file
//

#include "stdafx.h"
#include "InvNew.h"
#include "DlgWerner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWerner dialog


CDlgWerner::CDlgWerner(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWerner::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWerner)
	m_dDeclination = 0.0;
	m_dFieldStrength = 48000.0;
	m_dInclination = 60.0;
	m_nIpEnd = 4;
	m_nIpStart = 0;
	m_strSolutionName = _T("My Werner Solution");
	m_nWwEnd = 10;
	m_nWwStart = 6;
	m_nWwStep = 1;
	m_dMaxElev = 0.0;
	m_dMinElev = 0.0;
	//}}AFX_DATA_INIT
	m_pProf = NULL;
	m_pChanX = NULL;
	m_pChanY = NULL;
	m_pChanZ = NULL;
}


void CDlgWerner::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWerner)
	DDX_Control(pDX, IDC_WD_PROF, m_combProf);
	DDX_Control(pDX, IDC_CHANNEL_POS, m_combPos);
	DDX_Control(pDX, IDC_CHANNEL_FIELD, m_combField);
	DDX_Control(pDX, IDC_CHANNEL_ALTITUDE, m_combElev);
	DDX_Text(pDX, IDC_DECL, m_dDeclination);
	DDX_Text(pDX, IDC_FIELDSTREN, m_dFieldStrength);
	DDX_Text(pDX, IDC_INLC, m_dInclination);
	DDX_Text(pDX, IDC_IPEND, m_nIpEnd);
	DDV_MinMaxByte(pDX, m_nIpEnd, 0, 20);
	DDX_Text(pDX, IDC_IPSTART, m_nIpStart);
	DDV_MinMaxByte(pDX, m_nIpStart, 0, 20);
	DDX_Text(pDX, IDC_SOL_NAME, m_strSolutionName);
	DDX_Text(pDX, IDC_WWEND, m_nWwEnd);
	DDV_MinMaxUInt(pDX, m_nWwEnd, 4, 10000);
	DDX_Text(pDX, IDC_WWSTART, m_nWwStart);
	DDV_MinMaxUInt(pDX, m_nWwStart, 4, 10000);
	DDX_Text(pDX, IDC_WWSTEP, m_nWwStep);
	DDV_MinMaxUInt(pDX, m_nWwStep, 1, 10000);
	DDX_Text(pDX, IDC_MAX_ELEV, m_dMaxElev);
	DDV_MinMaxDouble(pDX, m_dMaxElev, -200000., 12000.);
	DDX_Text(pDX, IDC_MIN_ELEV, m_dMinElev);
	DDV_MinMaxDouble(pDX, m_dMinElev, -200000., 10000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWerner, CDialog)
	//{{AFX_MSG_MAP(CDlgWerner)
	ON_CBN_SELENDOK(IDC_CHANNEL_ALTITUDE, OnSelendokChannelAltitude)
	ON_CBN_SELENDOK(IDC_WD_PROF, OnSelendokWdProf)
	ON_CBN_SELENDOK(IDC_CHANNEL_FIELD, OnSelendokChannelField)
	ON_CBN_SELENDOK(IDC_CHANNEL_POS, OnSelendokChannelPos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWerner message handlers

BOOL CDlgWerner::OnInitDialog() 
{
	TRACE("Entering CWerDialog::OnInitDialog\n");


	m_strSolutionName = "Werner ";
	m_dFieldStrength = 47000;
	m_dInclination = 60;
	m_dDeclination = 0.0;
	m_nIpStart = 0;
	m_nIpEnd = 4;
	m_nWwEnd = 5;
	m_dMaxElev = 0.0;
	m_nWwStart = 4;
	m_nWwStep = 1;

	CDialog::OnInitDialog();

	ProfileArray* pProfArray = m_pDtb->GetProfArray();
	for( int i=0; i<pProfArray->GetSize(); i++) {
		CString strID = pProfArray->GetAt(i)->GetID();
		m_combProf.AddString( strID );
		if( strID == m_strCurProf ) {
			m_combProf.SetCurSel( i);
			OnSelendokWdProf();
		}
	}
	//m_combProf.AddString( "All" );

	if(m_combProf.GetCount() < 1) {
		AfxMessageBox( "No Profile in Database");
		OnCancel();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWerner::OnSelendokWdProf() 
{
	int nIndex = m_combProf.GetCurSel();

	if (nIndex != LB_ERR) {
		//POSITION pos = m_pDtb->GetProfList()->FindIndex(nIndex);
		m_pProf = m_pDtb->GetProfArray()->GetAt(nIndex);	
		TRACE("Selected profile : %s\n", m_pProf->GetID());

		m_combPos.ResetContent();		//removimg all items
		m_combField.ResetContent();		//removimg all items
		m_combElev.ResetContent();		//removimg all items
		ChannnelArray* pChanArray = m_pProf->GetChanArray();
		for(int i=0; i<pChanArray->GetSize(); i++) {
			CString str;
			str = pChanArray->GetAt(i)->GetName();
			m_combPos.AddString( str );			// adding new items 
			if( str == m_strPos ) {
				m_combPos.SetCurSel( i);
				OnSelendokChannelPos();
			}
			m_combField.AddString( pChanArray->GetAt(i)->GetName() );		// adding new items 
			if( str == m_strFld ) {
				m_combField.SetCurSel( i);
				OnSelendokChannelField();
			}
			m_combElev.AddString( pChanArray->GetAt(i)->GetName() );	// adding new items 
			if( str == m_strElev ) {
				m_combElev.SetCurSel( i);
				OnSelendokChannelAltitude();
			}
		}
		m_combElev.AddString( "None" );		// adding new items 
	}
}

void CDlgWerner::OnSelendokChannelField() 
{
	int nIndex = m_combField.GetCurSel();
	if (nIndex != LB_ERR) {
		m_pChanY = m_pProf->GetChanArray()->GetAt(nIndex);
		TRACE("Selected channel Y: %s\n", m_pChanY->GetID());
		if(m_pChanX == m_pChanY) {
			AfxMessageBox( "Select Different Channel" );
			m_pChanY = NULL;
			m_combField.SetCurSel(-1);
		}
	}
}

void CDlgWerner::OnSelendokChannelPos() 
{
	int nIndex = m_combPos.GetCurSel();
	if (nIndex != LB_ERR) {
		m_pChanX = m_pProf->GetChanArray()->GetAt(nIndex);
		TRACE("Selected channel X: %s\n", m_pChanX->GetID());

		m_nNProfPoints = m_pChanX->GetSize();
		m_nWwEnd = (m_nNProfPoints / 3);			// window width end is default 1/3 of all points
		SetDlgItemInt(IDC_WWEND, m_nWwEnd, FALSE);
		//double min, max;
		//m_pChanX->GetMinMaxValue(min, max);
		//m_dMinElev = (max - min) / 10;
		//CString str;
		//str.Format("%6.2f", m_dMinElev);
		//SetDlgItemText(IDC_MIN_ELEV, str);
		if((m_pChanX == m_pChanY) || (m_pChanX == m_pChanZ)) {
			AfxMessageBox( "Select Different Channel" );
			m_combPos.SetCurSel(-1);
			m_pChanX = NULL;
		}
	}
}

void CDlgWerner::OnSelendokChannelAltitude() 
{
	int nIndex = m_combElev.GetCurSel();
	if (nIndex != LB_ERR) {
		CString itemStr;
		m_combElev.GetLBText(nIndex, itemStr);
		// normal werner deconvolution
		if(itemStr == "None" ) {
			m_bVariableAltitude = FALSE;
			m_pChanZ = NULL;
		}
		// variable altitude
		else {
			m_pChanZ = m_pProf->GetChanArray()->GetAt(nIndex);
			// set min max elevation value
			double min, max;
			m_pChanZ->GetMinMaxValue(min, max);
			m_dMaxElev = max;
			m_dMinElev = min;
			CString str;
			str.Format("%6.2f", m_dMaxElev);
			SetDlgItemText(IDC_MAX_ELEV, str);
			if( m_pChanX != NULL) {
				m_pChanX->GetMinMaxValue(min, max);
				m_dMinElev -=  (max - min) / 2;
			}
			str.Format("%6.2f", m_dMinElev);
			SetDlgItemText(IDC_MIN_ELEV, str);
			m_nWwStart = 6;
			UpdateData(FALSE);
			m_bVariableAltitude = TRUE;

			TRACE("Selected channel Z: %s\n", m_pChanZ->GetID());
			if((m_pChanX == m_pChanZ) || (m_pChanZ == m_pChanY)) {
				AfxMessageBox( "Select Different Channel" );
				m_combElev.SetCurSel(0);
				m_pChanZ = NULL;
			}
		}
	}
}


void CDlgWerner::OnOK() 
{
	UpdateData();
	if(m_pChanZ != NULL)
		m_bVariableAltitude = TRUE;
	if(ExtraValidation()) {
		CDialog::OnOK();
	}
}



BOOL CDlgWerner::ExtraValidation()
{
	if(m_dMaxElev <= m_dMinElev) {
		AfxMessageBox( "Maximum altitude <= minimum!" );
		return FALSE;
	}
	if((m_nWwEnd < m_nWwStart)) {
		AfxMessageBox( "Window width end < start!" );
		return FALSE;
	}
	if((m_nWwEnd-m_nWwStart) < m_nWwStep) {
		AfxMessageBox( "Ivalid indow width step." );
		return FALSE;
	}
	if(m_nIpEnd < m_nIpStart) {
		AfxMessageBox( "Interference polynomial end < start!" );
		return FALSE;
	}
	if((m_pChanX == NULL)  || (m_pChanY == NULL) || (m_pProf == NULL)) {
		AfxMessageBox( "Select Channel." );
		return FALSE;
	}
	if(m_pChanX == m_pChanY) {
		AfxMessageBox( "Select Different Position or Field Channel!" );
		return FALSE;
	}
	if(m_bVariableAltitude && (m_nIpEnd > 4) ) {
		AfxMessageBox( "Maximun allowed interference polymial\nfor variable altitue is 4!" );
		m_nIpEnd = 4;
		UpdateData(FALSE);
		return FALSE;
	}
	if(m_bVariableAltitude && (m_nWwStart < 6) ) {
		AfxMessageBox( "Minmimum window width for variable altitude is 6 points!" );
		m_nWwStart = 6;
		UpdateData(FALSE);
		return FALSE;
	}
	return TRUE;
}
