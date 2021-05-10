/*
**
**	$Filename:	b3ConstantMap.h $
**	$Release:	Dortmund 2021 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Mapping enumeration or constants into strings
**
**	(C) Copyright 2021  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#pragma once

#ifndef B3_BASE_CONSTANT_MAP_H
#define B3_BASE_CONSTANT_MAP_H

#include <unordered_map>
#include <string>

#define B3_CONSTANT(f) { f, #f }

/**
 * This convenience class provides a mapping from a symbolic
 * value to its string representation. It is used for printing
 * clear text value while debugging.
 *
 * The following example shows how to map error codes to their
 * std::string representation:
 *
 * @code
 *	flag_map TestUtil::map
 *	{
 *		B3_CONSTANT(EINVAL),
 *		B3_CONSTANT(EAGAIN),
 *		B3_CONSTANT(EBUSY)
 *	};
 * @endcode
 *
 * You may access the mapper like every std::unordered_map.
 *
 * @note The operator[] may throw a std::out_of_range exception. It
 * is a safe way to map using the b3Get() method instead.
 */
template<class T> class b3ConstantMap : public std::unordered_map<T, std::string>
{
public:
	/**
	 * This alias is a short cut to the containing key/value pairs.
	 */
	using ConstantMapPair          = std::pair<const T, std::string>;

	/**
	 * This alias is a short cut to the containing iterator.
	 */
	using ConstantMapIterator      = typename std::unordered_map<T, std::string>::iterator;

	/**
	 * This alias is a short cut to the containing const iterator.
	 */
	using ConstantMapConstIterator = typename std::unordered_map<T, std::string>::const_iterator;

	/**
	 * This constructor makes it possible to initialize this mapper
	 * with a static initializer list.
	 *
	 * @param list The initializier list which may be provided by
	 * the compiler.
	 */
	explicit b3ConstantMap(
		const std::initializer_list<ConstantMapPair> & list) :
		std::unordered_map<T, std::string>(list)
	{
	}

	/**
	 * This method provides a safe way to represent a symbolic value
	 * as a std::string. If the key is not present in the map its
	 * hexadecimal representation is returned instead.
	 *
	 * @param key The symbolic value.
	 * @return The string representation of the symbolic value.
	 */
	inline std::string b3Get(const T key) const
	{
		ConstantMapConstIterator it = std::unordered_map<T, std::string>::find(key);

		if (it != std::unordered_map<T, std::string>::end())
		{
			return it->second;
		}
		else
		{
			char buffer[32];

			snprintf(buffer, sizeof(buffer), "0x%02X", (unsigned)key);

			return std::string(buffer);
		}
	}

	/**
	 * This method is a reverse lookup to find a key from its value. The
	 * return value is an const_iterator to a ConstantMapPair type. If this
	 * iterator is equal to end() no key was found. Otherwise you can
	 * select the key by using
	 *
	 * @code
	 * auto it = mapper.b3FindKey("WHAT_EVER");
	 *
	 * if (it != mapper.end())
	 * {
	 * 		auto key = it->first;
	 * }
	 * @endcode
	 *
	 * @param value The value from which the key should be reverse lookedup.
	 * @return A FlagMapPair const_iterator which may contain the key when
	 * the const_iterator is not equal to the end() iterator.
	 */
	inline ConstantMapConstIterator b3FindKey(
		const std::string & value) const
	{
		ConstantMapConstIterator it;

		for (it = std::unordered_map<T, std::string>::cbegin();
			it != std::unordered_map<T, std::string>::cend();
			++it)
		{
			if (it->second == value)
			{
				break;
			}
		}
		return it;
	}
};

#endif

