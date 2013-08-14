#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include "Message.h"

enum class MESSAGE
{
  CMD_FORWARD,
  CMD_BACKWARD,
  CMD_RIGHT,
  CMD_LEFT,
  CMD_UP,
  CMD_DOWN,
  CMD_MOUSELOOK,
  QUIT,
  NOCLIP
};

// eventually replace bool with message
typedef void (*CallbackType)(Message*);

namespace messenger
{
  int RegisterReceiver(MESSAGE msg, CallbackType callback);
  int UnregisterReceiver();

  int BroadcastMessage(MESSAGE type, Message* msg);
}

#endif