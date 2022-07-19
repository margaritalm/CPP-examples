/* lily.c - FireTalk SLCP protocol definitions
** Copyright 2002-2006 Daniel Reed <n@ml.org>
*/
#include <assert.h>	/* assert() */
#include <ctype.h>	/* isspace() */
#include <string.h>	/* strcasecmp() */
#include <stdarg.h>	/* va_* */
#include <stdio.h>	/* vsnprintf() */
#include <stdlib.h>
#include <time.h>	/* time_t */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#define ROOMSTARTS "-"

typedef enum {
	UNKNOWN,
	HERE,
	AWAY,
	OFFLINE
} lily_state_t;

typedef struct {
	int		 handle;
	char		*name;
#if 0
	unsigned char	*blurb,
			*attrib,
			*pronoun;
#endif
#ifdef ENABLE_FT_LILY_CTCPMAGIC
	unsigned char	 ctcpmagic;
#endif
	time_t		 login,
			 idle;
	lily_state_t	 state;
} lily_user_t;

typedef struct {
	int		 handle;
	char		*name,
			*title;
	time_t		 creation,
			 idle;
	int		 users;
	char		 ismember,
			 isemote;
} lily_chat_t;

typedef struct {
	char	*nickname,
		*password,
		 buffer[1024+1];
	lily_user_t
		*lily_userar;
	int	 lily_userc;
	lily_chat_t
		*lily_chatar;
	int	 lily_chatc;
	struct {
		int	key;
		char	str[1024];
	}	*qar;
	int	 qc;
} lily_conn_t;

static lily_user_t
	*lily_user_find_hand(lily_conn_t *c, int handle) {
	int	i;

	for (i = 0; i < c->lily_userc; i++)
		if (c->lily_userar[i].handle == handle)
			return(c->lily_userar+i);
	return(NULL);
}

static fte_t lily_got_cmd(lily_conn_t *c, char *str) {
	static int	infolen = 0,
			blocknum = -1;
	static char	*blockwhat = NULL,
			*info = NULL;

#define BMATCH(x)	((strncmp(str, "%" x " ", sizeof(x)+1) == 0) \
			&& (str += sizeof(x)+1))
	if BMATCH("NOTIFY") {
#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "NOTIFY [%s]\n", str);
#endif
		lily_recv_parse(c, str);
		return(lily_got_notify(c));
	} else if BMATCH("DATA") {
#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "DATA [%s]\n", str);
#endif
		lily_recv_parse(c, str);
		return(lily_got_DATA(c));
	} else if BMATCH("USER") {
		const char	*login,
				*input;

		lily_recv_parse(c, str);

		login = lily_recv_get("LOGIN");
		input = lily_recv_get("INPUT");
		if (login == NULL)
			login = "0";
		if (input == NULL)
			input = "0";

		lily_user_add(c,
			atoi(lily_recv_get("HANDLE")+1),
			lily_recv_get("NAME"),
			lily_recv_get("BLURB"),
			atol(login),
			atol(input),
			lily_user_state(lily_recv_get("STATE")),
			lily_recv_get("ATTRIB"),
			lily_recv_get("PRONOUN"));
	} else if BMATCH("DISC") {
		const char	*creation,
				*input,
				*users;

		lily_recv_parse(c, str);

		creation = lily_recv_get("CREATION");
		input = lily_recv_get("INPUT");
		users = lily_recv_get("USERS");
		if (creation == NULL)
			creation = "0";
		if (input == NULL)
			input = "0";
		if (users == NULL)
			users = "0";

		lily_chat_add(c,
			atoi(lily_recv_get("HANDLE")+1),
			lily_recv_get("NAME"),
			lily_recv_get("TITLE"),
			atol(creation),
			atol(input),
			lily_recv_get("ATTRIB"),
			atoi(users));			
	} else if BMATCH("command") {
		char	*sp = strchr(str, ' ');

		assert(blocknum != -1);
		assert(blockwhat != NULL);
		//assert(blocknum == atoi(str+1));

		if ((sp != NULL) && (*(sp+1) != 0) && (*blockwhat == '/')) {
			*sp++ = 0;

			if (strncasecmp(blockwhat, "/REVIEW ", sizeof("/REVIEW ")-1) == 0) {
				if (*sp != '#')
					firetalk_callback_chat_getmessage(c, ":REVIEW", "*", 0, sp);
				else
					firetalk_callback_chat_getmessage(c, ":REVIEW", "#", 0, sp+2);
			} else if (strncasecmp(blockwhat, "/MEMO ", sizeof("/MEMO ")-1) == 0) {
				if (*sp != '#')
					firetalk_callback_chat_getmessage(c, ":MEMO", "*", 0, sp);
				else
					firetalk_callback_chat_getmessage(c, ":MEMO", "#", 0, sp+2);
			} else if (strncasecmp(blockwhat, "/FINGER ", sizeof("/FINGER ")-1) == 0) {
				if (infolen == 0) {
					infolen = strlen(sp)+1;
					info = calloc(1, infolen+1);
					if (info == NULL)
						abort();
					*info = ' ';
					strcpy(info+1, sp);
				} else {
					infolen += sizeof("<BR>")-1 + strlen(sp);
					info = realloc(info, infolen+1);
					if (info == NULL)
						abort();
					strcat(info, "<BR>");
					strcat(info, sp);
				}
			} else if (strncasecmp(blockwhat, "/WHAT ", sizeof("/WHAT ")-1) == 0) {
				if (infolen == 0) {
					infolen = strlen(sp)+3;
					info = calloc(1, infolen+1);
					if (info == NULL)
						abort();
					strcpy(info, " * ");
					strcpy(info+3, sp);
				} else {
					infolen += sizeof("<BR>")-1 + 3 + strlen(sp);
					info = realloc(info, infolen+1);
					if (info == NULL)
						abort();
					strcat(info, "<BR> * ");
					strcat(info, sp);
				}
			} else if (strncasecmp(blockwhat, "/WHERE ", sizeof("/WHERE ")-1) == 0) {
				assert(infolen != 0);

				infolen += sizeof("<BR>")-1 + sizeof("<BR>")-1 + strlen(sp) + sizeof("<BR>")-1;
				info = realloc(info, infolen+1);
				if (info == NULL)
					abort();
				strcat(info, "<BR>");
				strcat(info, "<BR>");
				strcat(info, sp);
				strcat(info, "<BR>");
			} else if (strncasecmp(blockwhat, "/INFO ", sizeof("/INFO ")-1) == 0) {
/*				if (strcasecmp(blockwhat, "/INFO SET") != 0) { */
					assert(infolen != 0);

					infolen += sizeof("<BR>")-1 + strlen(sp);
					info = realloc(info, infolen+1);
					if (info == NULL)
						abort();
					strcat(info, "<BR>");
					strcat(info, sp);
/*				} */
			} else if (strncasecmp(blockwhat, "/JOIN ", sizeof("/JOIN ")-1) == 0) {
				if (strncmp(sp, "(could find no discussion to match to ", sizeof("(could find no discussion to match to ")-1) == 0)
					firetalk_callback_error(c, FE_ROOMUNAVAILABLE, blockwhat+sizeof("/JOIN ")-1, sp);
				else if (strncmp(sp, "(you are already a member of ", sizeof("(you are already a member of ")-1) == 0)
					;
				else
					firetalk_callback_error(c, FE_BADROOM, blockwhat+sizeof("/JOIN ")-1, sp);
			} else if (strncasecmp(blockwhat, "/QUIT ", sizeof("/QUIT ")-1) == 0) {
				if (strncmp(sp, "(could find no discussion to match to ", sizeof("(could find no discussion to match to ")-1) == 0)
					firetalk_callback_error(c, FE_ROOMUNAVAILABLE, blockwhat+sizeof("/QUIT ")-1, sp);
				else
					firetalk_callback_error(c, FE_BADROOM, blockwhat+sizeof("/QUIT ")-1, sp);
			} else if (strncasecmp(blockwhat, "/CREATE ", sizeof("/CREATE ")-1) == 0) {
				if (strstr(sp, " is already in use)") != NULL)
					firetalk_callback_error(c, FE_ROOMUNAVAILABLE, blockwhat+sizeof("/CREATE ")-1, sp);
				else
					firetalk_callback_error(c, FE_BADROOM, blockwhat+sizeof("/CREATE ")-1, sp);
			} else if (strcasecmp(blockwhat, "/WHERE") == 0) {
				char	*comma;

				if (strncmp(sp, "You are a member of ", sizeof("You are a member of ")-1) == 0) {

					sp += sizeof("You are a member of ")-1;
					comma = strchr(sp, ',');

					while (sp != NULL) {
						/*const*/ lily_chat_t
							*lily_chat;

						if (comma != NULL)
							*comma = 0;

						while (isspace(*sp))
							sp++;

						if ((lily_chat = lily_chat_find_name(c, lily_normalize_room_name(sp))) == NULL)
							abort();

						lily_chat_joined(c, lily_chat);

						if (lily_send_printf(c, "#$# what #%i", lily_chat->handle) != FE_SUCCESS)
							return(FE_PACKET);

						if (comma == NULL)
							sp = NULL;
						else {
							sp = comma+1;
							comma = strchr(sp, ',');
						}
					}
				}
			} else
				firetalk_callback_chat_getmessage(c, ":RAW", blockwhat, 0, sp);
		} else if (strstr(str, " is ignoring you ") != NULL) {
			char	*co;

			if ((co = strchr(blockwhat, ':')) != NULL) {
				*co = 0;
				firetalk_callback_error(c, FE_USERUNAVAILABLE, blockwhat, "You are being ignored");
			}
		}
	} else if BMATCH("begin") {
		char	*sp = strchr(str, ' ');

		if (sp != NULL) {
			*sp++ = 0;
			blocknum = atoi(str+1);
			free(blockwhat);
			blockwhat = strdup(sp);
#ifdef DEBUG_ECHO
			lily_echof(c, "got_cmd", "beginning [%s]\n", blockwhat);
#endif
		}
	} else if BMATCH("end") {
		//assert(blocknum == atoi(str+1));

		if (infolen) {
			char	*who = strchr(blockwhat, ' ');

			assert(who != NULL);
			*who++ = 0;

			if (strcasecmp(blockwhat, "/FINGER") == 0)
				lily_send_printf(c, "/WHERE %s", who);
			else if (strcasecmp(blockwhat, "/WHERE") == 0)
				lily_send_printf(c, "/INFO %s", who);
			else if (strcasecmp(blockwhat, "/WHAT") == 0)
				lily_send_printf(c, "/INFO %s", who);
			else if (strcasecmp(blockwhat, "/INFO") == 0) {
				firetalk_callback_gotinfo(c, who, info, 0, 0, 0, 0);
				infolen = 0;
				free(info);
				info = NULL;
			}
		}

#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "ending [%s]\n", blockwhat);
#endif
		blocknum = -1;
		free(blockwhat);
		blockwhat = NULL;
	} else if BMATCH("pong") {
		if (strncmp(str, "!LC! ", sizeof("!LC! ")-1) == 0)
			firetalk_callback_subcode_request(c, c->nickname, "LC", str+sizeof("!LC! ")-1);
		else
			firetalk_callback_chat_getmessage(c, ":RAW", "%pong", 0, str);
	} else if BMATCH("prompt") {
		if (lily_send_printf(c, "") != FE_SUCCESS)
			return(FE_PACKET);
	} else if BMATCH("options") {
#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "options [%s]\n", str);
#endif
	} else if BMATCH("connected") {
#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "connected [%s]\n", str);
#endif
	} else if BMATCH("whoami") {
		lily_recv_parse(c, str);

		firetalk_callback_newnick(c, lily_recv_get("name"));
	} else
		firetalk_callback_chat_getmessage(c, ":RAW", "(unrecognized)", 0, str);
#undef BMATCH

	return(FE_SUCCESS);
}

static fte_t lily_got_data(lily_conn_t *c, unsigned char *_buffer, unsigned short *bufferpos) {
	char	*buffer = (char *)_buffer, *str;

	while ((str = lily_recv_line(c, buffer, bufferpos)) != NULL)
		lily_got_cmd(c, str);

	return(FE_SUCCESS);
}

static fte_t lily_got_data_connecting(lily_conn_t *c, unsigned char *_buffer, unsigned short *bufferpos) {
	char	*buffer = (char *)_buffer, *str;

	while ((str = lily_recv_line(c, buffer, bufferpos)) != NULL) {
		if (strncmp(str, "%whoami ", sizeof("%whoami ")-1) == 0) {
			char	*s = strstr(str, " name="),
				buf[1024];
			int	len;

			s += sizeof(" name=")-1;
			len = atoi(s);
			assert(len < sizeof(buf)-1);
			s = strchr(s, '=');
			strncpy(buf, s+1, len);
			buf[len] = 0;
			free(c->nickname);
			c->nickname = strdup(buf);
			if (c->nickname == NULL)
				abort();
		} else if (strncmp(str, "%connected ", sizeof("%connected ")-1) == 0) {
			firetalk_callback_doinit(c, c->nickname);
			firetalk_callback_connected(c);
#ifdef ENABLE_FT_LILY_CTCPMAGIC
/*			if (lily_send_printf(c, "/CREATE ctcpmagic \"For identifying CTCP-aware clients, no discussion\"") != FE_SUCCESS)
				return(FE_PACKET); */
			if (lily_send_printf(c, "/JOIN -ctcpmagic") != FE_SUCCESS)
				return(FE_PACKET);
#endif
			if (lily_send_printf(c, "/WHERE") != FE_SUCCESS)
				return(FE_PACKET);
		} else if (str[0] == '%')
			lily_got_cmd(c, str);
		else if (str[0] == '#') {
			if (str[2] != 0)
				firetalk_callback_chat_getmessage(c, ":REVIEW", "#", 0, str+2);
		}

#if 0
		if (0) {
			firetalk_callback_connectfailed(c,FE_PACKET,"Server returned ERROR");
			return FE_PACKET;
		}
#endif
	}

	return(FE_SUCCESS);
}
