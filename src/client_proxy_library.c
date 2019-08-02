#include "my_app_generated.h"
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
static gboolean has_init = FALSE;
static gboolean has_deinit = FALSE;


void get_alarm_status(gchar **out_status_message)
{
    gboolean not_null_ptr = TRUE;
    if ((out_status_message == NULL) || (*out_status_message == NULL))
    {
        not_null_ptr = FALSE;
        printf("Pointe is null");
    }
    if (not_null_ptr)
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
    gboolean no_error = TRUE;
    if (has_init == FALSE)
    {
        addr = g_dbus_address_get_for_bus_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
        if (addr != NULL)
        {
            connection = g_dbus_connection_new_for_address_sync
                (
                    addr,
                    G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT | G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION,
                    NULL,
                    NULL,
                    &error
                );
                if (connection != NULL)
                {
                    source = my_example_com_app_trainning_alarm_clock_proxy_new_sync
                        (
                            connection,
                            G_DBUS_PROXY_FLAGS_NONE,
                            name,
                            objectName,
                            NULL,
                            &error
                        );
                    if (source == NULL)
                    {
                        no_error = FALSE;
                        printf("No source \n");
                    }
                }
                else
                {
                    printf("No connection\n");
                    no_error = FALSE;
                }
        }
        else
        {
            printf("No address \n");
            no_error = FALSE;
        }
        has_init = TRUE;
    }
    else
    {
        printf("init - To many times\n");
    }    
    return no_error;
}

void deinit()
{
    if (has_deinit == FALSE)
    {
        g_object_unref(connection);
        g_object_unref(source);
        has_deinit = TRUE;
    }
    else
    {
        printf("deinit - To many times \n");
    }
}

gboolean signal_function(void (*Callback)(void))
{
    gboolean is_signal_connection = TRUE;
    printf("signal - Inside the Library\n");
    g_signal_connect(source, "ring-alarm", G_CALLBACK(Callback), &error);
    if (error != NULL)
        is_signal_connection = FALSE;
    return is_signal_connection;
}