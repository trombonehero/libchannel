PROG=	test-libchannel
INTERNALPROG=	yes
SRCS=	\
	ctest.c \
	main.c \
	lifecycle.c \
	message.c \
	sendrecv.c \

NO_MAN=

CSTD?=	c99

INCDIR=		${.CURDIR}/../include

CFLAGS+=	-Wall -Werror -I${INCDIR}
LDFLAGS+=	-L../libchannel -lchannel

all: ${PROG}
run_tests:
	sh ${.CURDIR}/test.t || exit 1

.include <bsd.prog.mk>
