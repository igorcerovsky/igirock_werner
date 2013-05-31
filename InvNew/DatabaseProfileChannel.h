// ProfileChannel.h: interface for the CDtbProfChan class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "afxtempl.h"

typedef CArray<double, double> ChannelData;	// double channel data

class CDtbProfChan : public CObject  
{
protected:
	CString		m_strID;	// CString channel ID
	CString		m_strName;	// CString channel name
	BOOL		m_bVisible;	// visible indicator
	ChannelData m_data;		// double CArray data
	double		m_dMin, m_dMax;	// min, max value
	

public:
	DECLARE_SERIAL(CDtbProfChan)
	
	CDtbProfChan::CDtbProfChan()
	{
	}

	CDtbProfChan::CDtbProfChan(const char* strID) : m_strID(strID)
	{
	}

	CDtbProfChan(const char* strName, const char* strID) : m_strName(strName), m_strID(strID)
	{
	} 
	virtual ~CDtbProfChan();
	
	virtual void Serialize(CArchive &ar);
	
public:	// functions
	void UpdateMinMax();
	BOOL GetMinMaxValue(double& min, double& max);

	CString GetName()				{ return m_strName; }
	CDtbProfChan* GetChannel()		{ return this; }
	CString GetID()					{ return m_strID;}
	ChannelData* GetData()			{ return &m_data; }
	int GetSize()					{ return (int)m_data.GetSize(); }



};

typedef	CTypedPtrArray<CObArray, CDtbProfChan*> ChannnelArray;

