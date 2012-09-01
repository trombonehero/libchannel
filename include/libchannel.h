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
#include <stddef.h>

#include "libchannel-message.h"

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
 * [opaque]
 * A communications channel for sending data and capabilities (eg file
 * descriptors and channels).
 *
 * The implementation of a channel is platform-specific: on some platforms,
 * it will be a wrapper around a UNIX domain socket. On others, it may be a
 * more efficient message-passing primitive.
 */
struct channel;

/*!
 * Create a @ref channel that wraps a UNIX domain socket.
 *
 * @param  sock     A UNIX domain socket. This must be a UDS, rather than
 *                  e.g. a TCP connection, at least until we figure out how to
 *                  gracefully degrade when we can't send file descriptors and
 *                  other channels.
 * @return          NULL on error
 */
struct channel*     channel_wrap_socket(int sock);

//! Tests the validity of a pointer that claims to be a @ref channel.
bool                channel_isvalid(struct channel*);

/*!
 * Free a @ref channel.
 * @param c    must be a channel or NULL
 */
void                channel_destroy(struct channel *c);

//! Send data and/or capabilities over a channel.
int                 channel_send(struct channel*, struct message*);

// TODO: receiving things (integrated with application event model)...

/** @} */

__END_DECLS

#endif	/* !_LIBCHANNEL_H_ */

