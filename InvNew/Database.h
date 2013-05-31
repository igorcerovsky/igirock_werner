// Database.h: interface for the CDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASE_H__9C27A34C_4F6F_437A_B780_1D9507AED3E3__INCLUDED_)
#define AFX_DATABASE_H__9C27A34C_4F6F_437A_B780_1D9507AED3E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DatabaseProfile.h"

class CDatabase : public CObject  
{
DECLARE_SERIAL(CDatabase)

public:
	CDatabase() 
	{
	}

	CDatabase(const char* strID, const char* strName) : m_strID(strID), m_strName(strName) 
	{
	}

	virtual ~CDatabase();
	
public:
	bool IsEmpty();
	void		Serialize(CArchive &ar);
	CString		GetID()			{return m_strID;}
	CString		GetName()		{return m_strName;}

	ProfileArray* GetProfArray();
	CDtbProf*	GetProfile(const char* strProfID);
	CDtbProf*	GetProfile(int n);
	CDtbProf*	NewProfile(const char* strName);
	BOOL		FindProfChanName(CString& strProf, CString& strChan);	// finds prof and chan names, input strIDs are replaced by strNames
	BOOL		Export();
	BOOL		Import();
	CDtbProf*	GetNextProfile();
	CDtbProf*	GetLastProfile();
	CDtbProf*	GetFirstProfile();
	CDtbProf*	GetPrevProfile();

protected:
	CString			m_strID;
	CString			m_strName;
	ProfileArray	m_profArray;
	int				m_nCurProf;

public:
	int GetNumOfProfs(void);
	int UpdateMinMax(void);
	int GetMinMax(int nChan, double &min, double &max);
};

#endif // !defined(AFX_DATABASE_H__9C27A34C_4F6F_437A_B780_1D9507AED3E3__INCLUDED_)
