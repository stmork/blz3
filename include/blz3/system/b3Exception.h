/*
**
**	$Filename:  b3Exception.h $
**	$Release:   Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer: Steffen A. Mork $
**
**	Blizzard III - Exception template
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_EXCEPTION_H
#define B3_SYSTEM_EXCEPTION_H

#include "blz3/b3Config.h"
#include <stdexcept>

typedef unsigned int b3_errno;
typedef unsigned int b3_excno;

class b3ExceptionBase;

typedef void         (*b3ExceptionLogger)(const b3ExceptionBase *exception);
typedef const char * (*b3ExceptionMsgFunc)(const b3_errno ErrNo);

/**
 * This exception class is a base class for al Blizzard III exceptions.
 */
class B3_PLUGIN b3ExceptionBase : public std::exception
{
	static char                m_LocalMessageBuffer[512];
	static b3ExceptionLogger   m_Logger;
	static b3ExceptionMsgFunc  m_GetMessage;
	b3_excno            m_ExceptionType;
	b3_errno            m_ErrorCode;
	b3_count            m_LineNo;
	const char         *m_FileName;

protected:
	/**
	* This constructor creates an exception instance.
	*
	* @param ErrNo The error code.
	* @param ExcNo A special unique exception code. This can be used for mapping
	*              into a clear text message.
	* @param lineno The line number in the source code where the exception is thrown.
	* @param filename The filename of the source code where the exception is thrown.
	*/
	explicit b3ExceptionBase(
		const b3_errno  ErrNo,
		const b3_excno  ExcNo,
		const b3_count  lineno,
		const char     *filename);

	/**
	* This copy constructor creates an exception instance.
	*
	* @param exception The exception from where the data is copied from.
	*/
	explicit b3ExceptionBase(const b3ExceptionBase &exception);

	/**
	 * This method logs an exception.
	 *
	 * @param exception The exception to log.
	 */
	static void        b3Log(const b3ExceptionBase *exception);

	/**
	 * This method computes an error message from the given error code.
	 *
	 * @param ErrNo The error code.
	 * @return The resulting error message.
	 */
	static const char *b3GetMessage(const b3_errno ErrNo);

public:
	/**
	 * This method returns the exceptions error code.
	 *
	 * \return The exceptions error cod.
	 */
	inline b3_errno b3GetError()
	{
		return m_ErrorCode;
	}

	/**
	 * This method returns the exceptions error message.
	 *
	 * \return The exceptions error message.
	 */
	inline const char *b3GetErrorMsg() const
	{
		return m_GetMessage(m_ErrorCode);
	}

	/**
	 * This method sets the logger for the exception.
	 *
	 * \param logger The new exception logger.
	 */
	static void        b3SetLogger(b3ExceptionLogger logger = null);

	/**
	 * This method sets a procedure for logging the exception.
	 *
	 * \param converter The Message converter.
	 */
	static void        b3SetMsgFunc(b3ExceptionMsgFunc converter = null);

	const char *what() const throw();
};

#define B3_MK_ERRNO(e,t) (((e) & 0xff) | ((t) << 8))

/**
 * This template class is the main exception class.
 */
template <class T,b3_excno C> class B3_PLUGIN b3Exception : public b3ExceptionBase
{
public:
	/**
	 * This constructor initializes the exception.
	 *
	 * \param error The exception base.
	 * \param *FileName The modules' file name.
	 * \param LineNo The line number in the module.
	 */
	explicit b3Exception(
		const T     error,
		const char *FileName,
		const int   LineNo) : b3ExceptionBase(
					B3_MK_ERRNO(error,C),
					C,
					LineNo,
					FileName)
	{
	}
};

#define B3_THROW(e,no) throw e(no,__FILE__,__LINE__)

#endif
