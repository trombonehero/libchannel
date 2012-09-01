/*! @file message.c  Implementation of the @ref message abstraction. */
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

#include "libchannel.h"
#include "libchannel-internal.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


static message*
message_realloc(message **mp, size_t data_len, size_t descrips, size_t ch_size)
{
	size_t total_len = sizeof(message)
		+ data_len
		+ descrips * sizeof(int)
		+ ch_size;

	bool have_existing = (mp != NULL) && (*mp != NULL);
	message *m = realloc(have_existing ? *mp : NULL, total_len);
	if (m == NULL) return NULL;

	m->total_len = total_len;

	char     *data         = ((char*) m) + sizeof(message);
	int      *new_descrip  = (int*) (data + data_len);
	channel  *new_channels = (channel*) (new_descrip + descrips);

	// If we are changing the layout of the data trailer, we need to move
	// things around. Do this back-to-front to avoid overwriting things.
	if (have_existing) {
		// Re-calculate locations of old data; we can't trust the old
		// pointer values because the message struct may have been
		// relocated by realloc().
		int *old_descrip = (int*) (data + m->data.len);
		channel *old_channels =
			(channel*) (old_descrip + m->descriptors.len);

		// TODO: less than ch_size
		memmove(new_channels, old_channels, ch_size);

		size_t fdcount = m->descriptors.len;
		memmove(new_descrip, old_descrip, fdcount * sizeof(int));

		// Don't need to move the raw data: realloc(3) does this.
	}

	m->data.data = data;
	m->data.len = data_len;

	m->descriptors.data = new_descrip;
	m->descriptors.len = descrips;

	m->channels.data = new_channels;
	// don't set channels.len: this function only knows bytes, not count

	if (mp != NULL) *mp = m;
	return m;
}

static message*
message_alloc(size_t data_len, size_t descrips, size_t channels)
{
	message *m = message_realloc(NULL, data_len, descrips, channels);
	m->channels.len = 0;
	return m;
}


void
message_free(struct message *m)
{
	if (m == NULL) return;
	free(m);
}


bool
message_append_data(message **m, array(char) data)
{
	assert(m != NULL);
	assert(*m != NULL);

	message old = **m;
	message *enlarged = message_realloc(m,
		old.data.len + data.len, old.descriptors.len, old.channels.len);
	if (enlarged == NULL) return false;

	assert(enlarged->data.len == old.data.len + data.len);

	memcpy(enlarged->data.data + old.data.len, data.data, data.len);

	return true;
}

bool
message_append_descriptors(message **mp, array(int) fds)
{
	assert(mp != NULL);
	assert(*mp != NULL);

	message *m = *mp;
	size_t oldcount = m->descriptors.len;

	message *enlarged = message_realloc(mp,
		m->data.len, m->descriptors.len + fds.len, m->channels.len);
	if (enlarged == NULL) return false;

	assert(enlarged->descriptors.len == oldcount + fds.len);
	char *target = (char*) (enlarged->descriptors.data + oldcount);
	memcpy(target, fds.data, fds.len * sizeof(int));

	return true;
}

bool
message_append_channels(message **m, array(channelptr) channels)
{
	assert(m != NULL);
	assert(*m != NULL);
	for (size_t i = 0; i < channels.len; i++)
		assert(channel_isvalid(channels.data[i]));

	message *old = *m;

	// Calculate the total size required to store the channels.
	// There is no universal sizeof(channel), so we must enumerate 'em all.
	size_t channel_bytes = 0;

	for (size_t i = 0; i < old->channels.len; i++)
		channel_bytes += channel_size(old->channels.data + i);

	for (size_t i = 0; i < channels.len; i++)
		channel_bytes += channel_size(channels.data[i]);

	message *enlarged = message_realloc(m,
		old->data.len, old->descriptors.len, channel_bytes);
	if (enlarged == NULL) return false;
	*m = enlarged;

	char *new_channels = (char*) enlarged->channels.data;
	char *next = new_channels;

	for (size_t i = 0; i < channels.len; i++) {
		channel *src = channels.data[i];
		int copied = channel_copy(next, src);
		if (copied <= 0) {
			message_free(enlarged);
			return -1;
		}
		next += copied;
		enlarged->channels.len++;
	}

	return true;
}

struct message*
message_raw_data(char *data, size_t len)
{
	message *m = message_alloc(len, 0, 0);
	char *trailer = ((char*) m) + sizeof(message);
	memcpy(trailer, data, len);

	m->data.data = trailer;
	m->data.len = len;

	return m;
}

array(char)
message_get_data(struct message *m)
{
	assert(m != NULL);
	return m->data;
}

array(int)
message_get_descriptors(struct message *m)
{
	assert(m != NULL);
	return m->descriptors;
}

size_t
message_get_channels(struct message *m)
{
	assert(m != NULL);
	return m->channels.len;
}

channel*
message_get_channel(struct message *m, size_t index)
{
	assert(m != NULL);
	channel *next = m->channels.data;

	for (size_t i = 0; i < index; i++)
		next = (channel*) (((char*) next) + channel_size(next));

	return next;
}

#ifdef	DEBUG
#include <stdio.h>

void
message_dump(message *m)
{
	printf("message { size %lu (0x%lx),\n", m->total_len, m->total_len);
	printf("  data     { %4lu B : ", m->data.len);
	for (size_t i = 0; i < m->data.len; i++) {
		printf("%02x ", (int) (0xff & m->data.data[i]));
	}
	printf("}\n");

	printf("  descrip  { %6lu : ", m->descriptors.len);
	for (size_t i = 0; i < m->descriptors.len; i++) {
		printf("%d ", m->descriptors.data[i]);
	}
	printf("}\n");

	printf("  channels { %6lu : ", m->channels.len);
	for (size_t i = 0; i < m->channels.len; i++)
		printf("0x%lx ", (unsigned long) message_get_channel(m, i));
	printf("}\n");

	printf("}\n");

	size_t total_size = m->total_len;
	for (size_t row = 0; row < total_size; row += 8) {
		for (int i = row; i < row + 8; i++) {
			if (i >= total_size) printf("   ");
			else printf(" %02x", 0xff & ((char*) m)[i]);
		}

		printf("        ");

		for (int i = row; i < row + 8 && i < total_size; i++) {
			char c = ((char*) m)[i];
			if (((c >= 'A') && (c <= 'Z')) ||
			    ((c >= 'a') && (c <= 'z')) ||
			    (c == ',') || (c == ' ') || (c == '!') ||
			    (c == '"') || (c == '\'')
			   )
				printf("%c", c);

			else printf(".");
		}

		printf("\n");
	}
	printf("\n");
}
#endif	/* DEBUG */

