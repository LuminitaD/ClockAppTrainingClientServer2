#include "myApp_generated.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <gio/gio.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <glib.h>
#include <glib/gprintf.h>

GError *error = NULL;
GDBusConnection *connection;

MyExampleComAppTrainningAlarmClock *proxy;
unsigned alarm_status = 0;    //0 - unset; 1-set

void set_time(MyExampleComAppTrainningAlarmClock *alarmClockInterface, GDBusMethodInvocation *invocation, int hour, int min)
{
    char *return_msg = (char *) malloc(100);
    if ((hour >= 0) && (hour < 24))
    {
        if ((min >= 0) && (min < 60))
        {
            sprintf(return_msg, "Time is set at %d:%d", hour, min);
            my_example_com_app_trainning_alarm_clock_call_set_time_sync(proxy, hour, min, &return_msg, NULL, &error);
        }
        else
        {
            sprintf(return_msg, "Invalid argument to set the time");
        }
    }
    else
    {
        sprintf(return_msg, "Invalid argument to set the time");
    }
    my_example_com_app_trainning_alarm_clock_complete_set_time(alarmClockInterface, invocation, return_msg);
    free(return_msg);
}

void set_alarm_time(MyExampleComAppTrainningAlarmClock *alarmClockInterface, GDBusMethodInvocation *invocation, int hour, int min)
{
    char *return_msg = (char *) malloc(100);
    if ((hour >= 0) && (hour < 24))
    {
        if ((min >= 0) && (min < 60))
        {
            alarm_status = 1;
            sprintf(return_msg, "Alarm is set at %d:%d", hour, min);
            my_example_com_app_trainning_alarm_clock_call_set_alarm_time_sync(proxy, hour, min, &return_msg, NULL, &error);
        }
        else
        {
            sprintf(return_msg, "Invalid argument to set the alarm");
        }
    }
    else
    {
        sprintf(return_msg, "Invalid argument to set the alarm");
    }
    my_example_com_app_trainning_alarm_clock_complete_set_alarm_time(alarmClockInterface, invocation, return_msg);
    free(return_msg);
}

void set_alarm_status(MyExampleComAppTrainningAlarmClock *alarmClockInterface, GDBusMethodInvocation *invocation, gint status)
{
    char *return_msg = (char *) malloc(100);
    if (status == 1)
    {
        alarm_status = 1;
        strcpy(return_msg, "Alarm status: ON");
    }
    else if (status == 0)
    {
        alarm_status = 0;
        strcpy(return_msg, "Alarm status: OFF");
    }
    else
        strcpy(return_msg, "Alarm status: Unknow");
    my_example_com_app_trainning_alarm_clock_complete_set_alarm_status(alarmClockInterface, invocation, return_msg);
    printf("status= %d" , status);
    free(return_msg);
}

void get_alarm_status(MyExampleComAppTrainningAlarmClock *alarmClockInterface, GDBusMethodInvocation *invocation)
{
    char *return_msg = (char *) malloc(100);
    if (alarm_status == 1)
    {
        strcpy(return_msg, "Alarm is set");
    }
    else
    {
        strcpy(return_msg, "Alarm is unset");
    }
    my_example_com_app_trainning_alarm_clock_complete_get_alarm_status(alarmClockInterface, invocation, return_msg);
    free(return_msg);
}

static void name_lost (GDBusConnection *connection, const gchar *name, gpointer user_data)
{
    g_print("Lost the name %s on the session \n", name);
}

static void name_acquired (GDBusConnection *connection, const gchar *name, gpointer user_data)
{
    g_print("Acquired the name %s on the session \n", name);
}

static void bus_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data)
{
    MyExampleObjectSkeleton *skeletonObject;
    GDBusObjectManagerServer *serviceManager = NULL;
    MyExampleComAppTrainningAlarmClock *alarmClockInterface;

    serviceManager = g_dbus_object_manager_server_new("/com/AppTrainning/AlarmClock/manager");
    skeletonObject = my_example_object_skeleton_new("/com/AppTrainning/AlarmClock/manager/Object");
    alarmClockInterface = my_example_com_app_trainning_alarm_clock_skeleton_new();

    my_example_object_skeleton_set_com_app_trainning_alarm_clock(skeletonObject, alarmClockInterface);

    g_signal_connect (alarmClockInterface, "handle_set_alarm_status", G_CALLBACK (set_alarm_status), NULL);

    g_signal_connect (alarmClockInterface, "handle_get_alarm_status", G_CALLBACK (get_alarm_status), NULL);

    g_signal_connect (alarmClockInterface, "handle_set_time", G_CALLBACK (set_time), NULL);

    g_signal_connect (alarmClockInterface, "handle-set-alarm-time", G_CALLBACK (set_alarm_time), NULL);

    g_dbus_object_manager_server_export(serviceManager, G_DBUS_OBJECT_SKELETON(skeletonObject));
    g_dbus_object_manager_server_set_connection(serviceManager, connection);

    g_object_unref(skeletonObject);
    g_object_unref(alarmClockInterface);
    printf("Sccesfully aquired bus: %s\n",name);
}

void *signal_function()
{
    if (alarm_status == 1 )    //alarm is set
    {
        my_example_com_app_trainning_alarm_clock_emit_ring_alarm(proxy);
    }
    return NULL;
}

int main()
{
    guint owner_id;
    GMainLoop *loop;
    owner_id = g_bus_own_name(G_BUS_TYPE_SYSTEM, "com.AppTrainning.AlarmClock", G_BUS_NAME_OWNER_FLAGS_NONE, bus_acquired, name_acquired, name_lost, NULL, NULL);

    pthread_t thread;
    pthread_create(&thread, NULL, signal_function, NULL);

    loop = g_main_loop_new (NULL, FALSE);

    g_main_loop_run (loop);
    pthread_join (thread, NULL);

    g_bus_unown_name (owner_id);

    return 0;
}