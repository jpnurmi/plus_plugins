// Mocks generated by Mockito 5.0.0-nullsafety.7 from annotations
// in connectivity_plus_linux/test/connectivity_plus_linux_test.dart.
// Do not manually edit this file.

import 'dart:async' as _i7;

import 'package:connectivity_plus_linux/src/network_manager.dart' as _i4;
import 'package:dbus/src/dbus_client.dart' as _i2;
import 'package:dbus/src/dbus_introspect.dart' as _i5;
import 'package:dbus/src/dbus_method_response.dart' as _i6;
import 'package:dbus/src/dbus_remote_object.dart' as _i8;
import 'package:dbus/src/dbus_value.dart' as _i3;
import 'package:mockito/mockito.dart' as _i1;

// ignore_for_file: comment_references
// ignore_for_file: unnecessary_parenthesis

class _FakeDBusClient extends _i1.Fake implements _i2.DBusClient {}

class _FakeDBusObjectPath extends _i1.Fake implements _i3.DBusObjectPath {}

class _FakeNMConnection extends _i1.Fake implements _i4.NMConnection {}

class _FakeDBusIntrospectNode extends _i1.Fake
    implements _i5.DBusIntrospectNode {}

class _FakeDBusValue extends _i1.Fake implements _i3.DBusValue {}

class _FakeDBusMethodResponse extends _i1.Fake
    implements _i6.DBusMethodResponse {}

/// A class which mocks [NetworkManager].
///
/// See the documentation for Mockito's code generation for more information.
class MockNetworkManager extends _i1.Mock implements _i4.NetworkManager {
  @override
  _i2.DBusClient get client => (super.noSuchMethod(Invocation.getter(#client),
      returnValue: _FakeDBusClient()) as _i2.DBusClient);
  @override
  String get destination =>
      (super.noSuchMethod(Invocation.getter(#destination), returnValue: '')
          as String);
  @override
  _i3.DBusObjectPath get path => (super.noSuchMethod(Invocation.getter(#path),
      returnValue: _FakeDBusObjectPath()) as _i3.DBusObjectPath);
  @override
  _i7.Future<String> getPath() =>
      (super.noSuchMethod(Invocation.method(#getPath, []),
          returnValue: Future.value('')) as _i7.Future<String>);
  @override
  _i7.Future<String> getType() =>
      (super.noSuchMethod(Invocation.method(#getType, []),
          returnValue: Future.value('')) as _i7.Future<String>);
  @override
  _i7.Stream<String> subscribeTypeChanged() =>
      (super.noSuchMethod(Invocation.method(#subscribeTypeChanged, []),
          returnValue: Stream<String>.empty()) as _i7.Stream<String>);
  @override
  _i7.Future<_i4.NMConnection?> createConnection() =>
      (super.noSuchMethod(Invocation.method(#createConnection, []),
              returnValue: Future.value(_FakeNMConnection()))
          as _i7.Future<_i4.NMConnection?>);
  @override
  _i7.Future<_i5.DBusIntrospectNode> introspect() =>
      (super.noSuchMethod(Invocation.method(#introspect, []),
              returnValue: Future.value(_FakeDBusIntrospectNode()))
          as _i7.Future<_i5.DBusIntrospectNode>);
  @override
  _i7.Future<_i3.DBusValue> getProperty(String? interface, String? name) =>
      (super.noSuchMethod(Invocation.method(#getProperty, [interface, name]),
              returnValue: Future.value(_FakeDBusValue()))
          as _i7.Future<_i3.DBusValue>);
  @override
  _i7.Future<Map<String, _i3.DBusValue>> getAllProperties(String? interface) =>
      (super.noSuchMethod(Invocation.method(#getAllProperties, [interface]),
              returnValue: Future.value(<String, _i3.DBusValue>{}))
          as _i7.Future<Map<String, _i3.DBusValue>>);
  @override
  _i7.Future<void> setProperty(
          String? interface, String? name, _i3.DBusValue? value) =>
      (super.noSuchMethod(
          Invocation.method(#setProperty, [interface, name, value]),
          returnValue: Future.value(null),
          returnValueForMissingStub: Future.value()) as _i7.Future<void>);
  @override
  _i7.Stream<_i8.DBusPropertiesChangedSignal> subscribePropertiesChanged() =>
      (super.noSuchMethod(Invocation.method(#subscribePropertiesChanged, []),
              returnValue: Stream<_i8.DBusPropertiesChangedSignal>.empty())
          as _i7.Stream<_i8.DBusPropertiesChangedSignal>);
  @override
  _i7.Future<_i6.DBusMethodResponse> callMethod(
          String? interface, String? member, List<_i3.DBusValue>? values) =>
      (super.noSuchMethod(
              Invocation.method(#callMethod, [interface, member, values]),
              returnValue: Future.value(_FakeDBusMethodResponse()))
          as _i7.Future<_i6.DBusMethodResponse>);
  @override
  _i7.Stream<_i2.DBusSignal> subscribeSignal(
          String? interface, String? member) =>
      (super.noSuchMethod(
              Invocation.method(#subscribeSignal, [interface, member]),
              returnValue: Stream<_i2.DBusSignal>.empty())
          as _i7.Stream<_i2.DBusSignal>);
  @override
  _i7.Future<Map<_i3.DBusObjectPath, Map<String, Map<String, _i3.DBusValue>>>>
      getManagedObjects() => (super.noSuchMethod(
          Invocation.method(#getManagedObjects, []),
          returnValue: Future.value(<_i3.DBusObjectPath,
              Map<String, Map<String, _i3.DBusValue>>>{})) as _i7.Future<
          Map<_i3.DBusObjectPath, Map<String, Map<String, _i3.DBusValue>>>>);
  @override
  _i7.Stream<_i2.DBusSignal> subscribeObjectManagerSignals() =>
      (super.noSuchMethod(Invocation.method(#subscribeObjectManagerSignals, []),
              returnValue: Stream<_i2.DBusSignal>.empty())
          as _i7.Stream<_i2.DBusSignal>);
  @override
  String toString() =>
      (super.noSuchMethod(Invocation.method(#toString, []), returnValue: '')
          as String);
}
