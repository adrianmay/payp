#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>
#include <microhttpd.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"

#define PORT 7143
#define PAGE "<html><head><title>libmicrohttpd demo</title></head><body>Query string for &quot;%s&quot; was &quot;%s&quot;</body></html>"

static struct MHD_Daemon *webserver;

static int ahc_echo (void *cls,
		struct MHD_Connection *connection,
		const char *url,
		const char *method,
		const char *version,
		const char *upload_data, size_t *upload_data_size, void **ptr)
{
	static int aptr;
	const char *fmt = cls;
	const char *val;
	char *me;
	struct MHD_Response *response;
	int ret;

	if (0 != strcmp (method, "GET"))
		return MHD_NO;              /* unexpected method */
	if (&aptr != *ptr)
	{
		/* do never respond on first call */
		*ptr = &aptr;
		return MHD_YES;
	}
	*ptr = NULL;                  /* reset when done */
	val = MHD_lookup_connection_value (connection, MHD_GET_ARGUMENT_KIND, "q");
	me = malloc (snprintf (NULL, 0, fmt, "q", val) + 1);
	if (me == NULL)
		return MHD_NO;
	sprintf (me, fmt, "q", val);
	response = MHD_create_response_from_buffer (strlen (me), me,
			MHD_RESPMEM_MUST_FREE);
	if (response == NULL)
	{
		free (me);
		return MHD_NO;
	}
	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);
	return ret;
}

int initWeb () {
	webserver = MHD_start_daemon (MHD_USE_THREAD_PER_CONNECTION | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG, PORT, NULL, NULL, &ahc_echo, PAGE, MHD_OPTION_END);
	return (NULL != webserver);
}

void unitWeb () {
	MHD_stop_daemon (webserver);
}
