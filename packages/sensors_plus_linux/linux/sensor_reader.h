#ifndef SENSOR_READER_H
#define SENSOR_READER_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _SensorReader SensorReader;
typedef struct
{
  GObjectClass parent_class;
} SensorReaderClass;

GType sensor_reader_get_type();

typedef struct
{
  gboolean has_accelerometer;
  gfloat accelerometer[3];

  gboolean has_gyroscope;
  gfloat gyroscope[3];
} SensorReading;

typedef void (*SensorReaderFunc)(SensorReading reading, gpointer user_data);

SensorReader *sensor_reader_new(SensorReaderFunc func, gpointer user_data);
gboolean sensor_reader_is_active(SensorReader *reader);
void sensor_reader_start(SensorReader *reader);
void sensor_reader_stop(SensorReader *reader);

G_END_DECLS

#endif // SENSOR_READER_H
