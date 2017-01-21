#pragma once

#ifndef __ENGINE__NHASHEDSTRING__INL__H__
#define __ENGINE__NHASHEDSTRING__INL__H__

#include <cstring>
#include <malloc.h>

namespace Illehc
{
	inline HashedString :: HashedString() :
		m_Hash(Hash(""))
#ifdef DEBUG_KEEP_STRING
		, m_pString(NULL)
#endif
	{
	}

	inline HashedString :: HashedString(const char * i_string) :
		m_Hash(Hash(i_string))
#ifdef DEBUG_KEEP_STRING
		,m_pString(_strdup(i_string))
#endif
	{
	}

	inline HashedString :: HashedString(const HashedString & i_other) :
		m_Hash(i_other.m_Hash)
#ifdef DEBUG_KEEP_STRING
		, m_pString(_strdup(i_other.m_pString))
#endif
	{
	}

	inline HashedString :: ~HashedString()
	{
#ifdef DEBUG_KEEP_STRING
		if (m_pString)
			free(const_cast<char *>(m_pString));
#endif

	}

	inline HashedString & HashedString :: operator=(const HashedString & i_other)
	{
		m_Hash = i_other.m_Hash;

#ifdef DEBUG_KEEP_STRING
		if (m_pString)
			free(const_cast<char *>(m_pString));

			m_pString = i_other.m_pString;
#endif

		return *this;
	}

	inline unsigned int HashedString :: Get(void) const
	{
		return m_Hash;
	}

	inline bool HashedString :: operator == (const HashedString & i_other) const
	{
		return m_Hash == i_other.m_Hash;
	}

	inline bool HashedString :: operator != (const HashedString & i_other) const
	{
		return ! (operator == (i_other));
	}

	inline bool HashedString :: operator < (const HashedString & i_other) const
	{
		return m_Hash < i_other.m_Hash;
	}
} //namespace Illehc


#endif // !__ENGINE__NHASHEDSTRING__INL__H__

