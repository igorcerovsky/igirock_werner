// InducedField.h: interface for the CInducedField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDUCEDFIELD_H__1EF09A45_27B1_4449_B3E9_7AF0F5C4DAFC__INCLUDED_)
#define AFX_INDUCEDFIELD_H__1EF09A45_27B1_4449_B3E9_7AF0F5C4DAFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInducedField : public CObject  
{
public:
	DECLARE_SERIAL(CInducedField)
	CInducedField();
	CInducedField(double fstren, double incl, double decl)
	{
		m_dFieldStrength = fstren;
		m_dInclination = incl;
		m_dDeclination = decl;
	}

	CInducedField(const CInducedField &IF)	// copy constructor
	{
		m_dFieldStrength = IF.m_dFieldStrength;
		m_dInclination   = IF.m_dInclination;
		m_dDeclination   = IF.m_dDeclination;
	}
	
	const CInducedField& operator =(const CInducedField& IF)
	{
		m_dFieldStrength = IF.m_dFieldStrength;
		m_dInclination   = IF.m_dInclination;
		m_dDeclination   = IF.m_dDeclination;
		return *this;
	}

	virtual ~CInducedField();
	virtual void Serialize(CArchive &ar);

public:
	double	m_dFieldStrength;
	double	m_dInclination;
	double	m_dDeclination;
};

#endif // !defined(AFX_INDUCEDFIELD_H__1EF09A45_27B1_4449_B3E9_7AF0F5C4DAFC__INCLUDED_)
