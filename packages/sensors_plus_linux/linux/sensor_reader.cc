#include "sensor_reader.h"

#include <iostream>

#include "RTIMULib.h"

//#define SENSOR_DEBUG

#define SENSOR_READER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), sensor_reader_get_type(), SensorReader))

struct _SensorReader {
  GObject parent_instance;
  guint event_source = 0;
  SensorReaderFunc func;
  gpointer user_data = nullptr;
  RTIMU *imu = nullptr;
  // ### TODO: configurable settingsDirectory & productType
  RTIMUSettings *settings = nullptr;
};

G_DEFINE_TYPE(SensorReader, sensor_reader, g_object_get_type())

static void sensor_reader_dispose(GObject *object) {
  SensorReader *reader = SENSOR_READER(object);
  if (sensor_reader_is_active(reader)) {
    sensor_reader_stop(reader);
  }
  delete reader->settings;

  G_OBJECT_CLASS(sensor_reader_parent_class)->dispose(object);
}

static void sensor_reader_class_init(SensorReaderClass *klass) {
  G_OBJECT_CLASS(klass)->dispose = sensor_reader_dispose;
}

static void sensor_reader_init(SensorReader *self) {
  self->settings = new RTIMUSettings;
}

SensorReader *sensor_reader_new(SensorReaderFunc func, gpointer user_data) {
  g_return_val_if_fail(func, nullptr);

  SensorReader *reader =
      SENSOR_READER(g_object_new(sensor_reader_get_type(), nullptr));
  reader->func = func;
  reader->user_data = user_data;
  return reader;
}

static void sensor_reader_read(SensorReader *reader) {
  g_return_if_fail(reader);
  g_return_if_fail(reader->imu);

  while (reader->imu->IMURead()) {
    SensorReading reading;
    RTIMU_DATA imu_data = reader->imu->getIMUData();

#ifdef SENSOR_DEBUG
    std::cout << "GYRO"
              << " x: " << imu_data.gyro.x() << " y: " << imu_data.gyro.y()
              << " z: " << imu_data.gyro.z() << std::endl;

    std::cout << "ACCEL"
              << " x: " << imu_data.accel.x() << " y: " << imu_data.accel.y()
              << " z: " << imu_data.accel.z() << std::endl;
#endif  // SENSOR_DEBUG

    reading.has_accelerometer = imu_data.accelValid;
    reading.accelerometer[0] = imu_data.accel.x();
    reading.accelerometer[1] = imu_data.accel.y();
    reading.accelerometer[2] = imu_data.accel.z();

    reading.has_gyroscope = imu_data.gyroValid;
    reading.gyroscope[0] = imu_data.gyro.x();
    reading.gyroscope[1] = imu_data.gyro.y();
    reading.gyroscope[2] = imu_data.gyro.z();

    reader->func(reading, reader->user_data);
  }
}

gboolean timeout_cb(gpointer user_data) {
  SensorReader *reader = SENSOR_READER(user_data);
  sensor_reader_read(reader);
  return true;
}

gboolean sensor_reader_is_active(SensorReader *reader) {
  g_return_val_if_fail(reader, false);

  return reader->imu != nullptr;
}

void sensor_reader_start(SensorReader *reader) {
  g_return_if_fail(reader);
  g_return_if_fail(!reader->imu);

  reader->imu = RTIMU::createIMU(reader->settings);
  reader->imu->IMUInit();

  int interval = reader->imu->IMUGetPollInterval();
  reader->event_source = g_timeout_add_full(G_PRIORITY_HIGH, interval,
                                            timeout_cb, reader, nullptr);
}

void sensor_reader_stop(SensorReader *reader) {
  g_return_if_fail(reader);
  g_return_if_fail(reader->imu);

  delete reader->imu;
  reader->imu = nullptr;

  g_source_remove(reader->event_source);
  reader->event_source = 0;
}
