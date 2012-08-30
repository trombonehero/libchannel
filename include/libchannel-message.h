/*! @file libchannel-message.h  Declaration of @ref message. */
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

#ifndef	_LIBCHANNEL_MESSAGE_H_
#define	_LIBCHANNEL_MESSAGE_H_

#include <sys/cdefs.h>

#include <stdbool.h>
#include <stddef.h>

#include "libchannel-array.h"

__BEGIN_DECLS

/*!
 * \addtogroup libchannel-message
 *
 * Abstractions around packet-oriented messages with data, file descriptors
 * and channels.
 *
 * @{
 */

struct channel;
DEFINE_STRUCT_ARRAY(channel);
DEFINE_ARRAY_BASE(channelptr, struct channel*);

/*!
 * @struct message
 *
 * [opaque]
 * A message that can be sent over a @ref channel.
 *
 * A message can incorporate raw data, file descriptors and other channels.
 */
struct message;

/*!
 * Append [more] raw data to an existing message.
 *
 * @param[in,out] m       the message to append to; will be re-allocated
 * @return                success (true on success, false on failure)
 */
bool                message_append_data(struct message **m, array(char));

/*!
 * Append [more] descriptors to an existing message.
 *
 * @param[in,out] m       the message to append to; will be re-allocated
 * @return                success (true on success, false on failure)
 */
bool                message_append_descriptors(struct message **m, array(int));

/*!
 * Append [more] raw data to an existing message.
 *
 * @param[in,out] m       the message to append to; will be re-allocated
 * @return                success (true on success, false on failure)
 */
bool                message_append_channels(struct message **m,
                                            array(channelptr));

//! Build a message that only contains raw data. This copies the data.
struct message*     message_raw_data(char *data, size_t len);

//! Obtain a reference to the raw data contained in a message.
array(char)         message_get_data(struct message*);

//! Obtain a reference to any descriptors contained in a message.
array(int)          message_get_descriptors(struct message*);

//! How many channels does this message hold?
size_t              message_get_channels(struct message*);

//! Retrieve a particular channel from the message.
struct channel*     message_get_channel(struct message*, size_t);

//! Free a message, including any data it has copied.
void                message_free(struct message*);

/** @} */

__END_DECLS

#endif	/* !_LIBCHANNEL_H_ */

