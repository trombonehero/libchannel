/*-
 * Copyright (c) 2012 Jonathan Anderson
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/stat.h>

#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <libchannel.h>

#include "ctest.h"

int
test_message_data()
{
	int success = PASSED;

	char text[] = "Hello, world!";

	struct message *m = message_raw_data(text, sizeof(text));
	CHECK(m != NULL);
	CHECK(strncmp(text, message_get_data(m).data, sizeof(text) + 1) == 0);

	message_free(m);

	return success;
}

int
test_message_complex()
{
	int success = PASSED;

	// Prerequisites: things to put in the message.
	char hello[] = "Hello, ";
	char world[] = "world!";
	char total[] = "Hello, \0world!";

	int hosts = open("/etc/hosts", O_RDONLY);
	REQUIRE(hosts);

	int group = open("/etc/group", O_RDONLY);
	REQUIRE(group);

	int profile = open("/etc/profile", O_RDONLY);
	REQUIRE(profile);

	int fds[] = { hosts, group };
	size_t fdcount = sizeof(fds) / sizeof(int);

	int fake_uds[] = { 42, 43 };
	struct channel* ch[] = {
		channel_wrap_socket(fake_uds[0]),
		channel_wrap_socket(fake_uds[1])
	};
	size_t chcount = sizeof(ch) / sizeof(struct channel*);
	array(channelptr) channels = channelptr_array(ch, chcount);
	for (size_t i = 0; i < chcount; i++) assert(channel_isvalid(ch[i]));

	// Build the message.
	struct message *m = message_raw_data(hello, sizeof(hello));
	CHECK(m != NULL);

	CHECK(message_append_descriptors(&m, int_array(fds, fdcount)));
	CHECK(message_append_data(&m, char_array(world, sizeof(world))));
	CHECK(message_append_descriptors(&m, int_array(&profile, 1)));
	CHECK(message_append_channels(&m, channels));

	// Make sure it's got what we expect in it.
	array(char) str_out = message_get_data(m);
	CHECK(str_out.len == sizeof(total));
	CHECK(memcmp(str_out.data, total, sizeof(total)) == 0);

	array(int) fds_out = message_get_descriptors(m);
	CHECK(fds_out.len == fdcount + 1);
	for (size_t i = 0; i < fdcount; i++) {
		struct stat sb;
		CHECK_SUCCESS(fstat, fds_out.data[i], &sb);
	}

	CHECK(message_get_channels(m) == chcount);
	for (size_t i = 0; i < chcount; i++)
		CHECK(channel_isvalid(message_get_channel(m, i)));

	return success;
}

