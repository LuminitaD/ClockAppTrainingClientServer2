#include <dbus/dbus-glib.h>
#ifndef __CLIENTPROXYLIBRARY_H__
#define __CLIENTPROXYLIBRARY_H__

void get_alarm_status(gchar **out_status_message);
void set_alarm_status(gint arg_status);
void set_alarm_time(gint hour, gint min);
void set_time(gint hour, gint min);
void init();
void deinit();
gboolean signal_function(void (*Callback)(void));
#endif