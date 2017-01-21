#pragma once

#ifndef __ENGINE__NHASHEDSTRING__H__
#define __ENGINE__NHASHEDSTRING__H__

#define DEBUG_KEEP_STRING

namespace Illehc
{
	class HashedString
	{
	public:
		HashedString();
		~HashedString();

		HashedString(const char * i_string);
		HashedString(const HashedString & i_other);
		HashedString & operator = (const HashedString & i_other);

		unsigned int Get() const;

		inline bool operator == (const HashedString & i_other) const;
		inline bool operator != (const HashedString & i_other) const;
		inline bool operator < (const HashedString & i_other) const;

		static unsigned int Hash(const char * i_string);
		static unsigned int Hash(const void * i_bytes, unsigned int i_count);
	private:
		unsigned int 	m_Hash;

#ifdef DEBUG_KEEP_STRING
		const char * 	m_pString;
#endif
	};
} //namespace Illehc

#include "NHashedString-inl.h"

#endif // !__ENGINE__NHASHEDSTRING__H__
