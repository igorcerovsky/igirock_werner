// InducedField.cpp: implementation of the CInducedField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InvNew.h"
#include "InducedField.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CInducedField, CObject, 0)

CInducedField::CInducedField()
{

}

CInducedField::~CInducedField()
{

}

void CInducedField::Serialize(CArchive &ar)
{
	if(ar.IsStoring()) {
		ar << m_dFieldStrength << m_dInclination << m_dDeclination;
	}
	else {
		ar >> m_dFieldStrength >> m_dInclination >> m_dDeclination;
	}
}
