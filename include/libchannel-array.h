/*! @file libchannel-array.h  Something like bounded type-safe arrays in C. */
/*-
 * Copyright (c) 2012 Jonathan Anderson
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef	_LIBCHANNEL_ARRAY_H_
#define	_LIBCHANNEL_ARRAY_H_

#include <sys/cdefs.h>

#include <stddef.h>

__BEGIN_DECLS

/*!
 * \addtogroup libchannel-array libchannel-array
 * Something like type-safe bounded arrays in C.
 * @{
 */

/*!
 * @internal
 * @brief Base implementation of @ref DEFINE_ARRAY and @ref DEFINE_STRUCT_ARRAY.
 */
#define	DEFINE_ARRAY_BASE(tag, type)                                          \
	struct array_of_##tag { type *data; size_t len; };                    \
	inline array(tag) tag##_array(type *data, size_t len) {               \
		array(tag) tmp = { data, len };                               \
		return tmp;                                                   \
	}

//! Define struct array_of_[type] and function [type]_array(data, num).
#define	DEFINE_ARRAY(type)           DEFINE_ARRAY_BASE(type, type)

//! @ref DEFINE_ARRAY for structures.
#define	DEFINE_STRUCT_ARRAY(type)    DEFINE_ARRAY_BASE(type, struct type)

#define	array(type)          struct array_of_##type

DEFINE_ARRAY(char);
DEFINE_ARRAY(int);
DEFINE_ARRAY(unsigned);
DEFINE_ARRAY(float);
DEFINE_ARRAY(double);

/** @} */

__END_DECLS

#endif	/* !_LIBCHANNEL_ARRAY_H_ */

