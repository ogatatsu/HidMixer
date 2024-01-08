
#pragma once

#include "HidMixer/HidEngineDetail/HidEngine.h"
#include "HidMixer/HidEngineDetail/new_Action.h"

namespace HidMixer
{
  using HidEngineDetail::HidEngine;

  using HidEngineDetail::AngleSnap;
  using HidEngineDetail::Combo;
  using HidEngineDetail::ComboBehavior;
  using HidEngineDetail::Encoder;
  using HidEngineDetail::EncoderId;
  using HidEngineDetail::EncoderShift;
  using HidEngineDetail::EncoderShiftId;
  using HidEngineDetail::Gesture;
  using HidEngineDetail::GestureId;
  using HidEngineDetail::Key;
  using HidEngineDetail::KeymapOverlay;
  using HidEngineDetail::KeyShift;
  using HidEngineDetail::KeyShiftId;
  using HidEngineDetail::PointingDeviceId;
  using HidEngineDetail::PreAction;
  using HidEngineDetail::Timing;

  // clang-format off

  static constexpr HidEngineDetail::CharacterKeyCode A               = HidEngineDetail::CharacterKeyCode::A;
  static constexpr HidEngineDetail::CharacterKeyCode B               = HidEngineDetail::CharacterKeyCode::B;
  static constexpr HidEngineDetail::CharacterKeyCode C               = HidEngineDetail::CharacterKeyCode::C;
  static constexpr HidEngineDetail::CharacterKeyCode D               = HidEngineDetail::CharacterKeyCode::D;
  static constexpr HidEngineDetail::CharacterKeyCode E               = HidEngineDetail::CharacterKeyCode::E;
  static constexpr HidEngineDetail::CharacterKeyCode F               = HidEngineDetail::CharacterKeyCode::F;
  static constexpr HidEngineDetail::CharacterKeyCode G               = HidEngineDetail::CharacterKeyCode::G;
  static constexpr HidEngineDetail::CharacterKeyCode H               = HidEngineDetail::CharacterKeyCode::H;
  static constexpr HidEngineDetail::CharacterKeyCode I               = HidEngineDetail::CharacterKeyCode::I;
  static constexpr HidEngineDetail::CharacterKeyCode J               = HidEngineDetail::CharacterKeyCode::J;
  static constexpr HidEngineDetail::CharacterKeyCode K               = HidEngineDetail::CharacterKeyCode::K;
  static constexpr HidEngineDetail::CharacterKeyCode L               = HidEngineDetail::CharacterKeyCode::L;
  static constexpr HidEngineDetail::CharacterKeyCode M               = HidEngineDetail::CharacterKeyCode::M;
  static constexpr HidEngineDetail::CharacterKeyCode N               = HidEngineDetail::CharacterKeyCode::N;
  static constexpr HidEngineDetail::CharacterKeyCode O               = HidEngineDetail::CharacterKeyCode::O;
  static constexpr HidEngineDetail::CharacterKeyCode P               = HidEngineDetail::CharacterKeyCode::P;
  static constexpr HidEngineDetail::CharacterKeyCode Q               = HidEngineDetail::CharacterKeyCode::Q;
  static constexpr HidEngineDetail::CharacterKeyCode R               = HidEngineDetail::CharacterKeyCode::R;
  static constexpr HidEngineDetail::CharacterKeyCode S               = HidEngineDetail::CharacterKeyCode::S;
  static constexpr HidEngineDetail::CharacterKeyCode T               = HidEngineDetail::CharacterKeyCode::T;
  static constexpr HidEngineDetail::CharacterKeyCode U               = HidEngineDetail::CharacterKeyCode::U;
  static constexpr HidEngineDetail::CharacterKeyCode V               = HidEngineDetail::CharacterKeyCode::V;
  static constexpr HidEngineDetail::CharacterKeyCode W               = HidEngineDetail::CharacterKeyCode::W;
  static constexpr HidEngineDetail::CharacterKeyCode X               = HidEngineDetail::CharacterKeyCode::X;
  static constexpr HidEngineDetail::CharacterKeyCode Y               = HidEngineDetail::CharacterKeyCode::Y;
  static constexpr HidEngineDetail::CharacterKeyCode Z               = HidEngineDetail::CharacterKeyCode::Z;

  static constexpr HidEngineDetail::CharacterKeyCode _1              = HidEngineDetail::CharacterKeyCode::_1;
  static constexpr HidEngineDetail::CharacterKeyCode _2              = HidEngineDetail::CharacterKeyCode::_2;
  static constexpr HidEngineDetail::CharacterKeyCode _3              = HidEngineDetail::CharacterKeyCode::_3;
  static constexpr HidEngineDetail::CharacterKeyCode _4              = HidEngineDetail::CharacterKeyCode::_4;
  static constexpr HidEngineDetail::CharacterKeyCode _5              = HidEngineDetail::CharacterKeyCode::_5;
  static constexpr HidEngineDetail::CharacterKeyCode _6              = HidEngineDetail::CharacterKeyCode::_6;
  static constexpr HidEngineDetail::CharacterKeyCode _7              = HidEngineDetail::CharacterKeyCode::_7;
  static constexpr HidEngineDetail::CharacterKeyCode _8              = HidEngineDetail::CharacterKeyCode::_8;
  static constexpr HidEngineDetail::CharacterKeyCode _9              = HidEngineDetail::CharacterKeyCode::_9;
  static constexpr HidEngineDetail::CharacterKeyCode _0              = HidEngineDetail::CharacterKeyCode::_0;

  static constexpr HidEngineDetail::CharacterKeyCode Enter           = HidEngineDetail::CharacterKeyCode::Enter;
  static constexpr HidEngineDetail::CharacterKeyCode Escape          = HidEngineDetail::CharacterKeyCode::Escape;
  static constexpr HidEngineDetail::CharacterKeyCode Backspace       = HidEngineDetail::CharacterKeyCode::Backspace;
  static constexpr HidEngineDetail::CharacterKeyCode Tab             = HidEngineDetail::CharacterKeyCode::Tab;
  static constexpr HidEngineDetail::CharacterKeyCode Space           = HidEngineDetail::CharacterKeyCode::Space;
  static constexpr HidEngineDetail::CharacterKeyCode Minus           = HidEngineDetail::CharacterKeyCode::Minus;
  static constexpr HidEngineDetail::CharacterKeyCode Equal           = HidEngineDetail::CharacterKeyCode::Equal;
  static constexpr HidEngineDetail::CharacterKeyCode LeftBracket     = HidEngineDetail::CharacterKeyCode::LeftBracket;
  static constexpr HidEngineDetail::CharacterKeyCode RightBracket    = HidEngineDetail::CharacterKeyCode::RightBracket;
  static constexpr HidEngineDetail::CharacterKeyCode Backslash       = HidEngineDetail::CharacterKeyCode::Backslash;
  static constexpr HidEngineDetail::CharacterKeyCode NonUsNumberSign = HidEngineDetail::CharacterKeyCode::NonUsNumberSign;
  static constexpr HidEngineDetail::CharacterKeyCode Semicolon       = HidEngineDetail::CharacterKeyCode::Semicolon;
  static constexpr HidEngineDetail::CharacterKeyCode Quote           = HidEngineDetail::CharacterKeyCode::Quote;
  static constexpr HidEngineDetail::CharacterKeyCode Grave           = HidEngineDetail::CharacterKeyCode::Grave;
  static constexpr HidEngineDetail::CharacterKeyCode Comma           = HidEngineDetail::CharacterKeyCode::Comma;
  static constexpr HidEngineDetail::CharacterKeyCode Period          = HidEngineDetail::CharacterKeyCode::Period;
  static constexpr HidEngineDetail::CharacterKeyCode Slash           = HidEngineDetail::CharacterKeyCode::Slash;
  static constexpr HidEngineDetail::CharacterKeyCode CapsLock        = HidEngineDetail::CharacterKeyCode::CapsLock;

  static constexpr HidEngineDetail::CharacterKeyCode F1              = HidEngineDetail::CharacterKeyCode::F1;
  static constexpr HidEngineDetail::CharacterKeyCode F2              = HidEngineDetail::CharacterKeyCode::F2;
  static constexpr HidEngineDetail::CharacterKeyCode F3              = HidEngineDetail::CharacterKeyCode::F3;
  static constexpr HidEngineDetail::CharacterKeyCode F4              = HidEngineDetail::CharacterKeyCode::F4;
  static constexpr HidEngineDetail::CharacterKeyCode F5              = HidEngineDetail::CharacterKeyCode::F5;
  static constexpr HidEngineDetail::CharacterKeyCode F6              = HidEngineDetail::CharacterKeyCode::F6;
  static constexpr HidEngineDetail::CharacterKeyCode F7              = HidEngineDetail::CharacterKeyCode::F7;
  static constexpr HidEngineDetail::CharacterKeyCode F8              = HidEngineDetail::CharacterKeyCode::F8;
  static constexpr HidEngineDetail::CharacterKeyCode F9              = HidEngineDetail::CharacterKeyCode::F9;
  static constexpr HidEngineDetail::CharacterKeyCode F10             = HidEngineDetail::CharacterKeyCode::F10;
  static constexpr HidEngineDetail::CharacterKeyCode F11             = HidEngineDetail::CharacterKeyCode::F11;
  static constexpr HidEngineDetail::CharacterKeyCode F12             = HidEngineDetail::CharacterKeyCode::F12;

  static constexpr HidEngineDetail::CharacterKeyCode PrintScreen     = HidEngineDetail::CharacterKeyCode::PrintScreen;
  static constexpr HidEngineDetail::CharacterKeyCode ScrollLock      = HidEngineDetail::CharacterKeyCode::ScrollLock;
  static constexpr HidEngineDetail::CharacterKeyCode Pause           = HidEngineDetail::CharacterKeyCode::Pause;
  static constexpr HidEngineDetail::CharacterKeyCode Insert          = HidEngineDetail::CharacterKeyCode::Insert;
  static constexpr HidEngineDetail::CharacterKeyCode Home            = HidEngineDetail::CharacterKeyCode::Home;
  static constexpr HidEngineDetail::CharacterKeyCode PageUp          = HidEngineDetail::CharacterKeyCode::PageUp;
  static constexpr HidEngineDetail::CharacterKeyCode Delete          = HidEngineDetail::CharacterKeyCode::Delete;
  static constexpr HidEngineDetail::CharacterKeyCode End             = HidEngineDetail::CharacterKeyCode::End;
  static constexpr HidEngineDetail::CharacterKeyCode PageDown        = HidEngineDetail::CharacterKeyCode::PageDown;
  static constexpr HidEngineDetail::CharacterKeyCode Right           = HidEngineDetail::CharacterKeyCode::RightArrow;
  static constexpr HidEngineDetail::CharacterKeyCode Left            = HidEngineDetail::CharacterKeyCode::LeftArrow;
  static constexpr HidEngineDetail::CharacterKeyCode Down            = HidEngineDetail::CharacterKeyCode::DownArrow;
  static constexpr HidEngineDetail::CharacterKeyCode Up              = HidEngineDetail::CharacterKeyCode::UpArrow;
  static constexpr HidEngineDetail::CharacterKeyCode NumLock         = HidEngineDetail::CharacterKeyCode::NumLock;
  static constexpr HidEngineDetail::CharacterKeyCode KeypadDivide    = HidEngineDetail::CharacterKeyCode::KeypadDivide;
  static constexpr HidEngineDetail::CharacterKeyCode KeypadMultiply  = HidEngineDetail::CharacterKeyCode::KeypadMultiply;
  static constexpr HidEngineDetail::CharacterKeyCode KeypadSubtract  = HidEngineDetail::CharacterKeyCode::KeypadSubtract;
  static constexpr HidEngineDetail::CharacterKeyCode KeypadAdd       = HidEngineDetail::CharacterKeyCode::KeypadAdd;
  static constexpr HidEngineDetail::CharacterKeyCode KeypadEnter     = HidEngineDetail::CharacterKeyCode::KeypadEnter;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad1         = HidEngineDetail::CharacterKeyCode::Keypad1;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad2         = HidEngineDetail::CharacterKeyCode::Keypad2;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad3         = HidEngineDetail::CharacterKeyCode::Keypad3;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad4         = HidEngineDetail::CharacterKeyCode::Keypad4;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad5         = HidEngineDetail::CharacterKeyCode::Keypad5;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad6         = HidEngineDetail::CharacterKeyCode::Keypad6;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad7         = HidEngineDetail::CharacterKeyCode::Keypad7;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad8         = HidEngineDetail::CharacterKeyCode::Keypad8;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad9         = HidEngineDetail::CharacterKeyCode::Keypad9;
  static constexpr HidEngineDetail::CharacterKeyCode Keypad0         = HidEngineDetail::CharacterKeyCode::Keypad0;
  static constexpr HidEngineDetail::CharacterKeyCode KeypadPeriod    = HidEngineDetail::CharacterKeyCode::KeypadPeriod;
  static constexpr HidEngineDetail::CharacterKeyCode NonUsBackslash  = HidEngineDetail::CharacterKeyCode::NonUsBackslash;
  static constexpr HidEngineDetail::CharacterKeyCode Application     = HidEngineDetail::CharacterKeyCode::Application;
  static constexpr HidEngineDetail::CharacterKeyCode Power           = HidEngineDetail::CharacterKeyCode::Power;
  static constexpr HidEngineDetail::CharacterKeyCode KeypadEqual     = HidEngineDetail::CharacterKeyCode::KeypadEqual;
  static constexpr HidEngineDetail::CharacterKeyCode F13             = HidEngineDetail::CharacterKeyCode::F13;
  static constexpr HidEngineDetail::CharacterKeyCode F14             = HidEngineDetail::CharacterKeyCode::F14;
  static constexpr HidEngineDetail::CharacterKeyCode F15             = HidEngineDetail::CharacterKeyCode::F15;
  static constexpr HidEngineDetail::CharacterKeyCode F16             = HidEngineDetail::CharacterKeyCode::F16;
  static constexpr HidEngineDetail::CharacterKeyCode F17             = HidEngineDetail::CharacterKeyCode::F17;
  static constexpr HidEngineDetail::CharacterKeyCode F18             = HidEngineDetail::CharacterKeyCode::F18;
  static constexpr HidEngineDetail::CharacterKeyCode F19             = HidEngineDetail::CharacterKeyCode::F19;
  static constexpr HidEngineDetail::CharacterKeyCode F20             = HidEngineDetail::CharacterKeyCode::F20;
  static constexpr HidEngineDetail::CharacterKeyCode F21             = HidEngineDetail::CharacterKeyCode::F21;
  static constexpr HidEngineDetail::CharacterKeyCode F22             = HidEngineDetail::CharacterKeyCode::F22;
  static constexpr HidEngineDetail::CharacterKeyCode F23             = HidEngineDetail::CharacterKeyCode::F23;
  static constexpr HidEngineDetail::CharacterKeyCode F24             = HidEngineDetail::CharacterKeyCode::F24;
  static constexpr HidEngineDetail::CharacterKeyCode KeypadComma     = HidEngineDetail::CharacterKeyCode::KeypadComma;
  static constexpr HidEngineDetail::CharacterKeyCode Int1            = HidEngineDetail::CharacterKeyCode::Int1;
  static constexpr HidEngineDetail::CharacterKeyCode Int2            = HidEngineDetail::CharacterKeyCode::Int2;
  static constexpr HidEngineDetail::CharacterKeyCode Int3            = HidEngineDetail::CharacterKeyCode::Int3;
  static constexpr HidEngineDetail::CharacterKeyCode Int4            = HidEngineDetail::CharacterKeyCode::Int4;
  static constexpr HidEngineDetail::CharacterKeyCode Int5            = HidEngineDetail::CharacterKeyCode::Int5;
  static constexpr HidEngineDetail::CharacterKeyCode Int6            = HidEngineDetail::CharacterKeyCode::Int6;
  static constexpr HidEngineDetail::CharacterKeyCode Int7            = HidEngineDetail::CharacterKeyCode::Int7;
  static constexpr HidEngineDetail::CharacterKeyCode Int8            = HidEngineDetail::CharacterKeyCode::Int8;
  static constexpr HidEngineDetail::CharacterKeyCode Int9            = HidEngineDetail::CharacterKeyCode::Int9;
  static constexpr HidEngineDetail::CharacterKeyCode Lang1           = HidEngineDetail::CharacterKeyCode::Lang1;
  static constexpr HidEngineDetail::CharacterKeyCode Lang2           = HidEngineDetail::CharacterKeyCode::Lang2;
  static constexpr HidEngineDetail::CharacterKeyCode Lang3           = HidEngineDetail::CharacterKeyCode::Lang3;
  static constexpr HidEngineDetail::CharacterKeyCode Lang4           = HidEngineDetail::CharacterKeyCode::Lang4;
  static constexpr HidEngineDetail::CharacterKeyCode Lang5           = HidEngineDetail::CharacterKeyCode::Lang5;
  static constexpr HidEngineDetail::CharacterKeyCode Lang6           = HidEngineDetail::CharacterKeyCode::Lang6;
  static constexpr HidEngineDetail::CharacterKeyCode Lang7           = HidEngineDetail::CharacterKeyCode::Lang7;
  static constexpr HidEngineDetail::CharacterKeyCode Lang8           = HidEngineDetail::CharacterKeyCode::Lang8;
  static constexpr HidEngineDetail::CharacterKeyCode Lang9           = HidEngineDetail::CharacterKeyCode::Lang9;

  static constexpr HidEngineDetail::ModifiersCode Ctrl               = HidEngineDetail::ModifiersCode::LeftCtrl;
  static constexpr HidEngineDetail::ModifiersCode Shift              = HidEngineDetail::ModifiersCode::LeftShift;
  static constexpr HidEngineDetail::ModifiersCode Alt                = HidEngineDetail::ModifiersCode::LeftAlt;
  static constexpr HidEngineDetail::ModifiersCode Gui                = HidEngineDetail::ModifiersCode::LeftGui;
  static constexpr HidEngineDetail::ModifiersCode LeftCtrl           = HidEngineDetail::ModifiersCode::LeftCtrl;
  static constexpr HidEngineDetail::ModifiersCode LeftShift          = HidEngineDetail::ModifiersCode::LeftShift;
  static constexpr HidEngineDetail::ModifiersCode LeftAlt            = HidEngineDetail::ModifiersCode::LeftAlt;
  static constexpr HidEngineDetail::ModifiersCode LeftGui            = HidEngineDetail::ModifiersCode::LeftGui;
  static constexpr HidEngineDetail::ModifiersCode RightCtrl          = HidEngineDetail::ModifiersCode::RightCtrl;
  static constexpr HidEngineDetail::ModifiersCode RightShift         = HidEngineDetail::ModifiersCode::RightShift;
  static constexpr HidEngineDetail::ModifiersCode RightAlt           = HidEngineDetail::ModifiersCode::RightAlt;
  static constexpr HidEngineDetail::ModifiersCode RightGui           = HidEngineDetail::ModifiersCode::RightGui;
 
  static constexpr HidEngineDetail::PointingButtonsCode LeftButton      = HidEngineDetail::PointingButtonsCode::Left;
  static constexpr HidEngineDetail::PointingButtonsCode RightButton     = HidEngineDetail::PointingButtonsCode::Right;
  static constexpr HidEngineDetail::PointingButtonsCode MiddleButton    = HidEngineDetail::PointingButtonsCode::Middle;
  static constexpr HidEngineDetail::PointingButtonsCode BackwardButton  = HidEngineDetail::PointingButtonsCode::Backward;
  static constexpr HidEngineDetail::PointingButtonsCode ForwardButton   = HidEngineDetail::PointingButtonsCode::Forward;

  static constexpr HidEngineDetail::ConsumerControlCode LightUp         = HidEngineDetail::ConsumerControlCode::LightUp;
  static constexpr HidEngineDetail::ConsumerControlCode LightDown       = HidEngineDetail::ConsumerControlCode::LightDown;
  static constexpr HidEngineDetail::ConsumerControlCode PlayPause       = HidEngineDetail::ConsumerControlCode::PlayPause;
  static constexpr HidEngineDetail::ConsumerControlCode FastForward     = HidEngineDetail::ConsumerControlCode::FastForward;
  static constexpr HidEngineDetail::ConsumerControlCode Rewind          = HidEngineDetail::ConsumerControlCode::Rewind;
  static constexpr HidEngineDetail::ConsumerControlCode NextTrack       = HidEngineDetail::ConsumerControlCode::NextTrack;
  static constexpr HidEngineDetail::ConsumerControlCode PrevTrack       = HidEngineDetail::ConsumerControlCode::PrevTrack;
  static constexpr HidEngineDetail::ConsumerControlCode Mute            = HidEngineDetail::ConsumerControlCode::Mute;
  static constexpr HidEngineDetail::ConsumerControlCode VolumeUp        = HidEngineDetail::ConsumerControlCode::VolumeUp;
  static constexpr HidEngineDetail::ConsumerControlCode VolumeDown      = HidEngineDetail::ConsumerControlCode::VolumeDown;
  static constexpr HidEngineDetail::ConsumerControlCode LaunchMedia     = HidEngineDetail::ConsumerControlCode::LaunchMedia;
  static constexpr HidEngineDetail::ConsumerControlCode LaunchMail      = HidEngineDetail::ConsumerControlCode::LaunchMail;
  static constexpr HidEngineDetail::ConsumerControlCode LaunchApp1      = HidEngineDetail::ConsumerControlCode::LaunchApp1;
  static constexpr HidEngineDetail::ConsumerControlCode LaunchApp2      = HidEngineDetail::ConsumerControlCode::LaunchApp2;

  static constexpr HidEngineDetail::SystemControlCode SystemPowerDown   = HidEngineDetail::SystemControlCode::SystemPowerDown;
  static constexpr HidEngineDetail::SystemControlCode SystemSleep       = HidEngineDetail::SystemControlCode::SystemSleep;
  static constexpr HidEngineDetail::SystemControlCode SystemWakeUp      = HidEngineDetail::SystemControlCode::SystemWakeUp;

  // clang-format on

}

#define KC(key_code) (HidMixer::HidEngineDetail::new_KeyCode<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(key_code))

#define TAP(...) (HidMixer::HidEngineDetail::new_Tap<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(__VA_ARGS__))

#define TAP_R(...) (HidMixer::HidEngineDetail::new_TapWhenReleased<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(__VA_ARGS__))

#define TAP_DANCE(...) (HidMixer::HidEngineDetail::new_TapDance<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(__VA_ARGS__))

#define TAP_DANCE_DECIDE_MOVE_POINTER(...) (HidMixer::HidEngineDetail::new_TapDanceDecidMovePointer<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(__VA_ARGS__))

#define HOLD_TAP(...) (HidMixer::HidEngineDetail::new_HoldTap<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(__VA_ARGS__))

#define HOLD_TAP_DECIDE_MOVE_POINTER(...) (HidMixer::HidEngineDetail::new_HoldTapDecideMovePointer<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(__VA_ARGS__))

#define PT_MOVE(x, y) (HidMixer::HidEngineDetail::new_PointerMove<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(x, y))

#define PT_SCROLL(scroll, pan) (HidMixer::HidEngineDetail::new_PointingScroll<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(scroll, pan))

#define CONST_SPEED(action, interval_ms, delay_ms) (HidMixer::HidEngineDetail::new_ConstSpeed<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(action, interval_ms, delay_ms))

#define IF(func, true_action, false_action) (HidMixer::HidEngineDetail::new_If<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(func, true_action, false_action))

#define MLT(...) (HidMixer::HidEngineDetail::new_Multi<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(__VA_ARGS__))

#define TGL(action) (HidMixer::HidEngineDetail::new_Toggle<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(action))

#define RPT(action, delay_ms, interval_ms) (HidMixer::HidEngineDetail::new_Repeat<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(action, delay_ms, interval_ms))

#define NOP() (HidMixer::HidEngineDetail::new_NoOperation<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>())

#define MAP_SHIFT(...) (HidMixer::HidEngineDetail::new_MapShift<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(__VA_ARGS__))

#define KEYMAP(...) (HidMixer::HidEngineDetail::KEYMAP_impl<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>(__VA_ARGS__))
