// Profile.h: interface for the CDtbProf class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "InvNew.h"
#include "DatabaseProfileChannel.h"


class CDtbProf : public CObject  
{
public:	// constructors, destructors
	DECLARE_SERIAL(CDtbProf)

	CDtbProf()
	{
		m_nChan = 0;
	}

	CDtbProf(const char* strID) : m_strID(strID)
	{
		m_nChan = 0;
	}

	CDtbProf(const char* strName, const char* strID) : m_strName(strName), m_strID(strID)
	{
		m_nChan = 0;
	}

	virtual ~CDtbProf();

	virtual void Serialize(CArchive &ar);


public:
	bool SetData(double *pData, int n);
	bool SetData(CArray<double, double>	*pData, CString strChanName);
	CString			GetName()			{ return m_strName; }
	CString			GetID()				{ return m_strID; }
	ChannnelArray*	GetChanArray();
	CDtbProfChan*	NewChannel(const char* strName );
	CDtbProfChan*	GetChannel(CString strName);
	int				GetChannelNum(CString strName);
	CDtbProfChan*	GetChannelID(CString strChanID);
	int				GetNumberOfChannels();
	int				GetSize();
	MINMAXD			GetDepthRange()		{ return m_depthRange; }	// depth range for profile plot
	void			SetDepthRange( MINMAXD depthRange ) { m_depthRange = depthRange;}

protected:
	DWORD	m_nChan;	// number of channels
	CString m_strID;	// ID of pofile
	CString m_strName;	// CString name of profile

	MINMAXD	m_depthRange;

	CTypedPtrArray<CObArray, CDtbProfChan*> m_chanArray;
	
public:
	CDtbProfChan* GetChannel(int i);
	int UpdateMinMax(void);
};

typedef CTypedPtrArray<CObArray, CDtbProf*> ProfileArray;
