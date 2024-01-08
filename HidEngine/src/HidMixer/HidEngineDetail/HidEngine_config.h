
#pragma once

// タップのスピード
#ifndef HID_ENGINE_DEFAULT_TAP_SPEED_MS
#define HID_ENGINE_DEFAULT_TAP_SPEED_MS 1
#endif

// TapDanceのタップ判定時間
#ifndef HID_ENGINE_DEFAULT_TAPPING_TERM_MS
#define HID_ENGINE_DEFAULT_TAPPING_TERM_MS 200
#endif

// GestureでPreAction::InsteadOfFirstActionを設定した時にPreActionが実行された後、次のジェスチャが実行されないようにする時間
#ifndef HID_ENGINE_INSTEAD_OF_FIRST_GESTURE_AFTER_WAIT_TIME_MS
#define HID_ENGINE_INSTEAD_OF_FIRST_GESTURE_AFTER_WAIT_TIME_MS 250
#endif

// KeyboardReport,MouseClickReport,MouseScrollReportはこの値より早く次のレポートを送らない
#ifndef HID_ENGINE_KEY_REPORT_MIN_INTERVAL_MS
#define HID_ENGINE_KEY_REPORT_MIN_INTERVAL_MS 8
#endif

// コンボ判定中にこの回数ポインタが動いたらコンボを中断する
#ifndef HID_ENGINE_COMBO_INTERRUPTION_MOVE_POINTER_COUNT
#define HID_ENGINE_COMBO_INTERRUPTION_MOVE_POINTER_COUNT 3
#endif

// ActionTapper内部で使われているキューの最大サイズ
#ifndef HID_ENGINE_ACTION_TAPPER_QUEUE_SIZE
#define HID_ENGINE_ACTION_TAPPER_QUEUE_SIZE 32
#endif

// TimerMixinで使用できる最大タイマー数
#ifndef HID_ENGINE_TIMER_MIXIN_MAX_TIMER_COUNT
#define HID_ENGINE_TIMER_MIXIN_MAX_TIMER_COUNT 8
#endif

// HidEngine内部で使用しているイベントキューのサイズ
#ifndef HID_ENGINE_EVENT_QUEUE_SIZE
#define HID_ENGINE_EVENT_QUEUE_SIZE 8
#endif

// HidEngine task
#ifndef HID_ENGINE_TASK_STACK_SIZE
#define HID_ENGINE_TASK_STACK_SIZE 512
#endif
