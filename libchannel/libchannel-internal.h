/** @file libchannel-internal.h  Internal definitions for libchannel. */
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

#ifndef	_LIBCHANNEL_INTERNAL_H_
#define	_LIBCHANNEL_INTERNAL_H_

#include "libchannel.h"

#include <stdbool.h>

/*!
 * \addtogroup libchannel libchannel
 * @{
 */

/**
 * @internal
 * A channel "supertype" basically contains some function pointers.
 *
 * It is intended that "subclasses" will include a struct channel field
 * (perhaps called 'super').
 */
typedef struct channel {
	int     flags;
	int     (*get_descriptor)(struct channel*);
	int     magic;       //!< @internal @brief implementation-specific
} channel;

/**
 * @internal
 * A message structure is immediately followed by the data it references;
 * they are allocated and freed together.
 *
 * The memory layout is:
 *   struct message
 *   char      data[data.len]
 *   int       descriptors[descriptors.len]
 *   channel   channels[channels.len]
 */
typedef struct message {
	size_t            total_len; //!< @internal @brief len(struct + data)
	array(char)       data;
	array(int)        descriptors;
	array(channel)    channels;
} message;


/*
 * Several channel implementations follow.
 */

/*! @internal @brief UNIX domain socket implementation of a channel. */
typedef struct uds_channel {
	channel     super;
	int         socket;
} uds_channel;

#define UDS_MAGIC   0xBCAEAB67

uds_channel*        uds_create(int flags);
channel*            uds_wrap(uds_channel*);
uds_channel*        uds_unwrap(channel*);
int                 uds_descriptor(channel*);

//! @}

#endif	/* !_LIBCHANNEL_INTERNAL_H_ */

