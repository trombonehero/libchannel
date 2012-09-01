LIB=	channel

INCDIR=	${.CURDIR}/../include
MANDIR=	${.CURDIR}/../man/man3

INCS=	${INCDIR}/libchannel.h
SRCS=	\
	libchannel.c      \
	message.c         \
	uds.c             \


CFLAGS+=	-std=c99 -Wall -Werror -I ${.CURDIR} -I ${INCDIR}

SHLIB_MAJOR=	1

MAN=	${MANDIR}/channel.3 ${MANDIR}/libchannel.3

MLINKS=	\
	libchannel.3	channel_create.3        \
	libchannel.3	channel_isvalid.3       \
	libchannel.3	channel_destroy.3       \
	libchannel.3	channel_flags.3         \
	libchannel.3	channel_descriptor.3    \

.include <bsd.lib.mk>

