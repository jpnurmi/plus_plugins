//
//  Generated file. Do not edit.
//

#include "generated_plugin_registrant.h"

#include <sensors_plus_linux/sensors_plus_linux_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) sensors_plus_linux_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "SensorsPlusLinuxPlugin");
  sensors_plus_linux_plugin_register_with_registrar(sensors_plus_linux_registrar);
}
