/// The Linux implementation of `share_plus`.
library share_plus_linux;

import 'dart:ui';

import 'package:url_launcher/url_launcher.dart';
import 'package:share_plus_platform_interface/share_plus_platform_interface.dart';

/// The Linux implementation of SharePlatform.
class ShareLinux extends SharePlatform {
  /// Share text.
  @override
  Future<void> share(
    String text, {
    String? subject,
    Rect? sharePositionOrigin,
  }) async {
    final queryParameters = {
      if (subject != null) 'subject': subject,
      'body': text,
    };

    // see https://github.com/dart-lang/sdk/issues/43838#issuecomment-823551891
    final uri = Uri(
      scheme: 'mailto',
      query: queryParameters.entries
          .map((e) =>
              '${Uri.encodeComponent(e.key)}=${Uri.encodeComponent(e.value)}')
          .join('&'),
    );

    if (await canLaunch(uri.toString())) {
      await launch(uri.toString());
    } else {
      throw Exception('Unable to share on linux');
    }
  }

  /// Share files.
  @override
  Future<void> shareFiles(
    List<String> paths, {
    List<String>? mimeTypes,
    String? subject,
    String? text,
    Rect? sharePositionOrigin,
  }) {
    throw UnimplementedError('shareFiles() has not been implemented on Linux.');
  }
}
