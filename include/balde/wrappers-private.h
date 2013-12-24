/*
 * balde: A microframework for C based on GLib and bad intentions.
 * Copyright (C) 2013 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 *
 * This program can be distributed under the terms of the LGPL-2 License.
 * See the file COPYING.
 */

#ifndef _BALDE_WRAPPERS_PRIVATE_H
#define _BALDE_WRAPPERS_PRIVATE_H

#include <glib.h>
#include <balde/routing.h>
#include <balde/wrappers.h>

void balde_response_free(balde_response_t *response);
balde_response_t* balde_make_response_from_exception(GError *error);
void balde_fix_header_name(gchar *name);
void balde_header_render(gchar *key, gchar *value, GString *str);
gchar* balde_response_render(balde_response_t *response, gboolean with_body);
void balde_response_print(balde_response_t *response, gboolean with_body);
GHashTable* balde_request_headers(void);
balde_request_t* balde_make_request(void);
void balde_request_free(balde_request_t *request);

#endif /* _BALDE_WRAPPERS_PRIVATE_H */
