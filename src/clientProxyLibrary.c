#include "myApp_generated.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <glib/gprintf.h>
#include <gio/gio.h>
#include <dbus/dbus-glib.h>
#include <stdlib.h> /* exit, EXIT_FAILURE */

static MyExampleComAppTrainningAlarmClock* source = NULL;
static GDBusConnection* connection = NULL;
static GError * error = NULL;
static const gchar *name = "com.AppTrainning.AlarmClock";
static const gchar *objectName = "/com/AppTrainning/AlarmClock/manager/Object";

void get_alarm_status(gchar **out_status_message)
{
    if (out_status_message == NULL)
    {
        printf("Pointe is null");
        return;
    }
    my_example_com_app_trainning_alarm_clock_call_get_alarm_status_sync (source, out_status_message, NULL, &error);
}

void set_alarm_status(gint arg_status)
{
    my_example_com_app_trainning_alarm_clock_call_set_alarm_status_sync (source, arg_status, NULL, NULL, &error);
}

void set_alarm_time(gint hour, gint min)
{
    my_example_com_app_trainning_alarm_clock_call_set_alarm_time_sync (source, hour, min, NULL, NULL, &error);
}

void set_time(gint hour, gint min)
{
    my_example_com_app_trainning_alarm_clock_call_set_time_sync (source, hour, min, NULL, NULL, &error);
}

gboolean init()
{
    gchar *addr = NULL;
    gboolean noError = TRUE;
    addr = g_dbus_address_get_for_bus_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (addr == NULL)
        noError = FALSE;
    if (noError)
    {
        connection = g_dbus_connection_new_for_address_sync
            (
                addr,
                G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT|G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION,
                NULL,
                NULL,
                &error
            );
        source = my_example_com_app_trainning_alarm_clock_proxy_new_sync
            (
                connection,
                G_DBUS_PROXY_FLAGS_NONE,
                name,
                objectName,
                NULL,
                &error
            );
    }
    return noError;
}

void deinit()
{
    g_object_unref(connection);
    g_object_unref(source);
}

gboolean signal_function(void (*Callback)(void))
{
    gboolean isSignalConnection = TRUE;
    printf("signal - Inside the Library\n");
    g_signal_connect(source, "ring-alarm", G_CALLBACK(Callback), &error);
    if (error != NULL)
        isSignalConnection = FALSE;
    return isSignalConnection;
}