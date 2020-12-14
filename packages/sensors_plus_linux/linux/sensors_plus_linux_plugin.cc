#include "include/sensors_plus_linux/sensors_plus_linux_plugin.h"
#include "sensor_reader.h"

#include <flutter_linux/flutter_linux.h>
#include <glib.h>

#define SENSORS_PLUS_LINUX_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), sensors_plus_linux_plugin_get_type(), \
                              SensorsPlusLinuxPlugin))

static const float kGravity = 9.8;
static const char *kAccelerometerEventChannel = "dev.fluttercommunity.plus/sensors/accelerometer";
static const char *kUserAccelerometerEventChannel = "dev.fluttercommunity.plus/sensors/user_accel";
static const char *kGyroscopeEventChannel = "dev.fluttercommunity.plus/sensors/gyroscope";

struct _SensorsPlusLinuxPlugin
{
  GObject parent_instance;
  SensorReader *reader = nullptr;

  guint accelerometer_listeners = 0;
  FlEventChannel *accelerometer_channel = nullptr;

  guint user_accelerometer_listeners = 0;
  FlEventChannel *user_accelerometer_channel = nullptr;

  guint gyroscope_listeners = 0;
  FlEventChannel *gyroscope_channel = nullptr;
};

G_DEFINE_TYPE(SensorsPlusLinuxPlugin, sensors_plus_linux_plugin, g_object_get_type())

static void send_sensor_event(FlEventChannel *event_channel, gfloat *xyz, gfloat factor = 1)
{
  g_autoptr(FlValue) list = fl_value_new_list();
  fl_value_append(list, fl_value_new_float(-xyz[0] * factor));
  fl_value_append(list, fl_value_new_float(xyz[1] * factor));
  fl_value_append(list, fl_value_new_float(xyz[2] * factor));

  g_autoptr(GError) error = nullptr;
  if (!fl_event_channel_send(event_channel, list, NULL, &error))
  {
    g_warning("Failed to send event: %s", error->message);
  }
}

static void sensors_plus_linux_plugin_read(SensorsPlusLinuxPlugin *self, SensorReading reading)
{
  if (self->accelerometer_listeners > 0 && reading.has_accelerometer)
  {
    send_sensor_event(self->accelerometer_channel, reading.accelerometer, kGravity);
  }

  // ### TODO: remove gravity
  // if (self->user_accelerometer_listeners > 0 && reading.has_accelerometer)
  // {
  //   send_sensor_event(self->user_accelerometer_channel, reading.accelerometer);
  // }

  if (self->gyroscope_listeners > 0 && reading.has_gyroscope)
  {
    send_sensor_event(self->gyroscope_channel, reading.gyroscope);
  }
}

static gboolean sensors_plus_linux_plugin_has_listeners(SensorsPlusLinuxPlugin *self)
{
  return self->accelerometer_listeners > 0 || self->user_accelerometer_listeners > 0 || self->gyroscope_listeners > 0;
}

static void sensors_plus_linux_plugin_listen(SensorsPlusLinuxPlugin *self)
{
  if (!sensor_reader_is_active(self->reader) && sensors_plus_linux_plugin_has_listeners(self))
  {
    sensor_reader_start(self->reader);
  }
}

static void sensors_plus_linux_plugin_cancel(SensorsPlusLinuxPlugin *self)
{
  if (sensor_reader_is_active(self->reader) && !sensors_plus_linux_plugin_has_listeners(self))
  {
    sensor_reader_stop(self->reader);
  }
}

static void sensors_plus_linux_plugin_dispose(GObject *object)
{
  SensorsPlusLinuxPlugin *plugin = SENSORS_PLUS_LINUX_PLUGIN(object);
  g_clear_object(&plugin->reader);

  G_OBJECT_CLASS(sensors_plus_linux_plugin_parent_class)->dispose(object);
}

static void sensors_plus_linux_plugin_class_init(SensorsPlusLinuxPluginClass *klass)
{
  G_OBJECT_CLASS(klass)->dispose = sensors_plus_linux_plugin_dispose;
}

static void read_cb(SensorReading reading, gpointer user_data)
{
  SensorsPlusLinuxPlugin *plugin = SENSORS_PLUS_LINUX_PLUGIN(user_data);
  sensors_plus_linux_plugin_read(plugin, reading);
}

static void sensors_plus_linux_plugin_init(SensorsPlusLinuxPlugin *self)
{
  self->reader = sensor_reader_new(read_cb, self);
}

static FlMethodErrorResponse *listen_accelerometer_cb(FlEventChannel *event_channel,
                                                      FlValue *args,
                                                      gpointer user_data)
{
  SensorsPlusLinuxPlugin *plugin = SENSORS_PLUS_LINUX_PLUGIN(user_data);
  ++plugin->accelerometer_listeners;
  sensors_plus_linux_plugin_listen(plugin);
  return nullptr;
}

static FlMethodErrorResponse *cancel_accelerometer_cb(FlEventChannel *event_channel,
                                                      FlValue *args,
                                                      gpointer user_data)
{
  SensorsPlusLinuxPlugin *plugin = SENSORS_PLUS_LINUX_PLUGIN(user_data);
  --plugin->accelerometer_listeners;
  sensors_plus_linux_plugin_cancel(plugin);
  return nullptr;
}

static FlMethodErrorResponse *listen_user_accelerometer_cb(FlEventChannel *event_channel,
                                                           FlValue *args,
                                                           gpointer user_data)
{
  SensorsPlusLinuxPlugin *plugin = SENSORS_PLUS_LINUX_PLUGIN(user_data);
  ++plugin->user_accelerometer_listeners;
  sensors_plus_linux_plugin_listen(plugin);
  return nullptr;
}

static FlMethodErrorResponse *cancel_user_accelerometer_cb(FlEventChannel *event_channel,
                                                           FlValue *args,
                                                           gpointer user_data)
{
  SensorsPlusLinuxPlugin *plugin = SENSORS_PLUS_LINUX_PLUGIN(user_data);
  --plugin->user_accelerometer_listeners;
  sensors_plus_linux_plugin_cancel(plugin);
  return nullptr;
}

static FlMethodErrorResponse *listen_gyroscope_cb(FlEventChannel *event_channel,
                                                  FlValue *args,
                                                  gpointer user_data)
{
  SensorsPlusLinuxPlugin *plugin = SENSORS_PLUS_LINUX_PLUGIN(user_data);
  ++plugin->gyroscope_listeners;
  sensors_plus_linux_plugin_listen(plugin);
  return nullptr;
}

static FlMethodErrorResponse *cancel_gyroscope_cb(FlEventChannel *event_channel,
                                                  FlValue *args,
                                                  gpointer user_data)
{
  SensorsPlusLinuxPlugin *plugin = SENSORS_PLUS_LINUX_PLUGIN(user_data);
  --plugin->gyroscope_listeners;
  sensors_plus_linux_plugin_cancel(plugin);
  return nullptr;
}

void sensors_plus_linux_plugin_register_with_registrar(FlPluginRegistrar *registrar)
{
  SensorsPlusLinuxPlugin *plugin = SENSORS_PLUS_LINUX_PLUGIN(
      g_object_new(sensors_plus_linux_plugin_get_type(), nullptr));

  FlBinaryMessenger *messenger = fl_plugin_registrar_get_messenger(registrar);
  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();

  plugin->accelerometer_channel = fl_event_channel_new(messenger,
                                                       kAccelerometerEventChannel,
                                                       FL_METHOD_CODEC(codec));
  fl_event_channel_set_stream_handlers(plugin->accelerometer_channel,
                                       listen_accelerometer_cb,
                                       cancel_accelerometer_cb,
                                       g_object_ref(plugin),
                                       g_object_unref);

  plugin->user_accelerometer_channel = fl_event_channel_new(messenger,
                                                            kUserAccelerometerEventChannel,
                                                            FL_METHOD_CODEC(codec));
  fl_event_channel_set_stream_handlers(plugin->user_accelerometer_channel,
                                       listen_user_accelerometer_cb,
                                       cancel_user_accelerometer_cb,
                                       g_object_ref(plugin),
                                       g_object_unref);

  plugin->gyroscope_channel = fl_event_channel_new(messenger,
                                                   kGyroscopeEventChannel,
                                                   FL_METHOD_CODEC(codec));
  fl_event_channel_set_stream_handlers(plugin->gyroscope_channel,
                                       listen_gyroscope_cb,
                                       cancel_gyroscope_cb,
                                       g_object_ref(plugin),
                                       g_object_unref);

  g_object_unref(plugin);
}
