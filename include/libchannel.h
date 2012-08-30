/*! @file libchannel.h  Public API for libchannel. */
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

#ifndef	_LIBCHANNEL_H_
#define	_LIBCHANNEL_H_

#include <sys/cdefs.h>
#include <stdbool.h>

__BEGIN_DECLS

/*!
 * \addtogroup libchannel libchannel
 *
 * libchannel provides an abstraction around communication channels like
 * sockets.
 *
 * This allows a message-passing style of interaction, whether the backend is
 * a socket or a function call in the same address space.
 *
 * @{
 */

/*!
 * @struct channel
 *
 * A communications channel for sending data and capabilities (eg file
 * descriptors and channels).
 *
 * The implementation of a channel is platform-specific: on some platforms,
 * it will be a wrapper around a UNIX domain socket. On others, it may be a
 * more efficient message-passing primitive.
 */
struct channel;


#define	CHANNEL_RELIABLE     0x0001	// reliable delivery
#define	CHANNEL_INORDER      0x0002	// in-order delivery

/*!
 * Create a @ref channel with certain properties.
 *
 * @param  flags   any of a number of flags OR'ed together:
 *                  - CHANNEL_RELIABLE:  guarantee reliable delivery
 *                  - CHANNEL_INORDER:   guarantee in-order delivery
 */
struct channel*     channel_create(int flags);

//! Tests the validity of a pointer that claims to be a @ref channel.
bool                channel_isvalid(struct channel*);

/*!
 * Free a @ref channel.
 * @param c    must be a channel or NULL
 */
void                channel_destroy(struct channel *c);

/*!
 * Get a channel's flags.
 *
 * These flags cannot be changed after creation, but they can be retrieved.
 *
 * @return  -1 on error, OR'ed combination of flags otherwise
 */
int                 channel_flags(struct channel*);

/**
 * A file descriptor representation of a channel.
 *
 * This descriptor can be polled, selected, etc.
 */
int                 channel_descriptor(struct channel*);

/** @} */

__END_DECLS

#endif	/* !_LIBSANDBOX_H_ */

