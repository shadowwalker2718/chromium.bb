# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'chromium_code': 1,
  },
  'targets': [
    {
      'target_name': 'aura',
      'type': '<(component)',
      'dependencies': [
        '../../base/base.gyp:base',
        '../../base/base.gyp:base_i18n',
        '../../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../../gpu/gpu.gyp:gpu',
        '../../skia/skia.gyp:skia',
        '../base/ui_base.gyp:ui_base',
        '../compositor/compositor.gyp:compositor',
        '../events/events.gyp:events',
        '../events/events.gyp:events_base',
        '../gfx/gfx.gyp:gfx',
        '../gfx/gfx.gyp:gfx_geometry',
      ],
      'defines': [
        'AURA_IMPLEMENTATION',
      ],
      'sources': [
        'client/aura_constants.cc',
        'client/aura_constants.h',
        'client/capture_client.cc',
        'client/capture_client.h',
        'client/capture_delegate.h',
        'client/cursor_client.cc',
        'client/cursor_client.h',
        'client/cursor_client_observer.h',
        'client/cursor_client_observer.cc',
        'client/default_capture_client.cc',
        'client/default_capture_client.h',
        'client/event_client.cc',
        'client/event_client.h',
        'client/focus_change_observer.cc',
        'client/focus_change_observer.h',
        'client/focus_client.cc',
        'client/focus_client.h',
        'client/screen_position_client.cc',
        'client/screen_position_client.h',
        'client/visibility_client.cc',
        'client/visibility_client.h',
        'client/window_stacking_client.cc',
        'client/window_stacking_client.h',
        'client/window_tree_client.cc',
        'client/window_tree_client.h',
        'env.cc',
        'env.h',
        'env_observer.h',
        'input_state_lookup.cc',
        'input_state_lookup.h',
        'input_state_lookup_win.cc',
        'input_state_lookup_win.h',
        'layout_manager.cc',
        'layout_manager.h',
        'remote_window_tree_host_win.cc',	
        'remote_window_tree_host_win.h',
        'scoped_window_targeter.cc',
        'scoped_window_targeter.h',
        'window.cc',
        'window.h',
        'window_event_dispatcher.cc',
        'window_event_dispatcher.h',
        'window_delegate.h',
        'window_layer_type.h',
        'window_observer.h',
        'window_targeter.cc',
        'window_targeter.h',
        'window_tracker.cc',
        'window_tracker.h',
        'window_tree_host.cc',
        'window_tree_host.h',
        'window_tree_host_mac.mm',
        'window_tree_host_mac.h',
        'window_tree_host_observer.h',
        'window_tree_host_ozone.cc',
        'window_tree_host_ozone.h',
        'window_tree_host_win.cc',
        'window_tree_host_win.h',
        'window_tree_host_x11.cc',
        'window_tree_host_x11.h',
        '../wm/public/activation_change_observer.h',
        '../wm/public/activation_change_observer.cc',
        '../wm/public/activation_client.cc',
        '../wm/public/activation_client.h',
        '../wm/public/activation_delegate.cc',
        '../wm/public/activation_delegate.h',
        '../wm/public/animation_host.cc',
        '../wm/public/animation_host.h',
        '../wm/public/dispatcher_client.cc',
        '../wm/public/dispatcher_client.h',
        '../wm/public/drag_drop_client.cc',
        '../wm/public/drag_drop_client.h',
        '../wm/public/drag_drop_delegate.cc',
        '../wm/public/drag_drop_delegate.h',
        '../wm/public/scoped_tooltip_disabler.cc',
        '../wm/public/scoped_tooltip_disabler.h',
        '../wm/public/tooltip_client.cc',
        '../wm/public/tooltip_client.h',
        '../wm/public/transient_window_client.cc',
        '../wm/public/transient_window_client.h',
        '../wm/public/window_move_client.cc',
        '../wm/public/window_move_client.h',
      ],
      'conditions': [
        ['use_x11==1', {
          'dependencies': [
            '../../build/linux/system.gyp:x11',
            '../../build/linux/system.gyp:xrandr',
            '../../build/linux/system.gyp:xi',
          ],
        }],
        ['OS=="win"', {
          'dependencies': [
            '../metro_viewer/metro_viewer.gyp:metro_viewer_messages',
            '../../ipc/ipc.gyp:ipc',
          ],
          'sources!': [
            'input_state_lookup.cc',
          ],
        }],
        ['use_ozone==1', {
          'dependencies': [
            '../ozone/ozone.gyp:ozone',
          ],
        }],
      ],
    },
  ],
}
