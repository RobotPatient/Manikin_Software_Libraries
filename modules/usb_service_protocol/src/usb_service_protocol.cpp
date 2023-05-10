#include <Arduino.h>
#include <usb_service_protocol.hpp>

namespace usb_service_protocol {
USBServiceProtocolRegisters* serviceProtocolRegisters = NULL;
uint8_t NumRegisters = 0;

USBServiceProtocolRegisters* LastRegister;
inline constexpr char kVt100MoveCharacterBack1Pos[] = "\33[2D";
inline constexpr char kTerminalEntryCharacter = '>';
inline constexpr char kNewLineCharacter = '\n';
inline constexpr uint8_t kBackspaceAsciiCode = 127;
inline constexpr uint8_t kCarriageReturnCharacter = '\r';
inline constexpr uint8_t kStrTerminationCharacter = '\0';
inline constexpr uint8_t kSpaceCharacter = ' ';
inline constexpr int kNoNewCharacter = -1;
inline constexpr char kPrintOnNextLine[] = "\r\n";
inline constexpr uint8_t kReadBufferSize = 100;
static char read_buffer[kReadBufferSize];
static uint8_t read_index = 0;
/* Dimensions of the buffer that the task being created will use as its stack.
    NOTE:  This is the number of words the stack will hold, not the number of
    bytes.  For example, if each stack item is 32-bits, and this is set to 100,
    then 400 bytes (100 * 32-bits) will be allocated. */
inline constexpr uint8_t kReadTaskStackSize = 200;
/* The priority the task will have, 0 highest priority, 32 lowest priority*/
inline constexpr uint8_t kReadTaskPriority = 5;
/* Structure that will hold the TCB of the task being created. */
StaticTask_t staticReadTask;
/* Buffer that the task being created wil l use as its stack.  Note this is
    an array of StackType_t variables.  The size of StackType_t is dependent on
    the RTOS port. */
StackType_t ReadTaskStack[kReadTaskStackSize];

typedef struct {
  char* argsBuffer[kUSBProtoMaxAmountOfArguments];
  uint8_t argNum;
} ParsedArgs;

constexpr char* Getarg(char* input) {
  if (input == NULL)
    return NULL;
  // Loop over the passed in string,
  // till space-, termination- or carriage-return character encountered
  do {
    input++;
  } while (*input != kSpaceCharacter && 
           *input != kStrTerminationCharacter &&
           *input != kCarriageReturnCharacter);
  // Termination char means end of the string
  if (*input == '\0')
    return NULL;
  // The reason for inserting the STR termination char
  // Is to force other c functions like printf to stop reading the
  // string at the place where the space was..
  // This way we get a nice formatted string of which we only have to
  // save the pointers of the places in the string where arguments are saved :)
  *input = kStrTerminationCharacter;
  // This is the pointer to a new possible argument, not the one we found..
  return input + 1;
}

ParsedArgs Parsearg(char* buffer) {
  ParsedArgs Args;
  Args.argNum = 0;
  Args.argsBuffer[0] = Getarg(buffer);
  const bool more_than_one_argument_entered = Args.argsBuffer[0] != NULL;
  if (more_than_one_argument_entered) {
    // There are more arguments.. Loop over the readbuffer searching for more arguments (whitespaces)
    for (uint8_t argNum = 1; argNum < kUSBProtoMaxAmountOfArguments; argNum++) {
      Args.argsBuffer[argNum] = Getarg(Args.argsBuffer[argNum - 1]);
      if (Args.argsBuffer[argNum] == NULL)
        break;
      else
        Args.argNum++;
    }
  }
  return Args;
}

const char* Runcmd(char* buffer) {
  ParsedArgs args = Parsearg(buffer);
  for (uint8_t command_index = 0; command_index < NumRegisters; command_index++) {
    const bool command_found =
        (strcmp(serviceProtocolRegisters[command_index].cmd_, buffer) == 0);
    if (command_found) {
      const bool too_many_arguments =
          args.argNum > serviceProtocolRegisters[command_index].num_of_args_;
      const bool too_few_arguments =
          (args.argNum < serviceProtocolRegisters[command_index].num_of_args_);
      if (too_many_arguments)
        return "!E Too many arguments!";
      else if (too_few_arguments)
        return "!E Too few arguments!";
      else {
        LastRegister = &serviceProtocolRegisters[command_index];
        return serviceProtocolRegisters[command_index].cmd_cb(args.argsBuffer, args.argNum);
      }
      break;
    }
  }
  return "!E Command unrecognized!";
}

inline void ParseInput(int character, const bool last_command_was_stream_cmd) {
  switch (character) {
    case kBackspaceAsciiCode: {
      read_buffer[read_index] = kStrTerminationCharacter;
      const bool buffer_not_empty = (read_index != 0);
      if (buffer_not_empty) {
        Serial.write(character);
        read_index--;
      }
      break;
    }
    case kCarriageReturnCharacter: {
      Serial.write(kNewLineCharacter);
      read_buffer[read_index++] = character;
      Serial.write(character);
      if (last_command_was_stream_cmd) {
        LastRegister->stream_cmd_ = false;
      } else {
        const char* command_output = Runcmd(read_buffer);
        Serial.write(command_output);
        Serial.write(kNewLineCharacter);
        Serial.write(character);
      }
      Serial.write(kTerminalEntryCharacter);
      memset(read_buffer, kStrTerminationCharacter, read_index + 1);
      read_index = 0;
      break;
    }
    default: {
      Serial.write(character);
      read_buffer[read_index++] = character;
      break;
    }
  }
}

void ReadTask(void* pvParameters) {
  int character = kNoNewCharacter;
  memset(read_buffer, kStrTerminationCharacter, kReadBufferSize);
  Serial.write(kTerminalEntryCharacter);
  for (;;) {
    character = Serial.read();
    const bool last_command_was_stream_cmd = LastRegister->stream_cmd_;
    const bool new_character = character != kNoNewCharacter;
    if (new_character) {
      ParseInput(character, last_command_was_stream_cmd);
    } else if (last_command_was_stream_cmd) {
      const char* stream_cmd_output = LastRegister->cmd_cb(NULL, 0);
      Serial.write(stream_cmd_output);
      Serial.write(kPrintOnNextLine);
    }
  }
}

void Init(USBServiceProtocolRegisters* registers, uint8_t num_of_registers) {
  serviceProtocolRegisters = registers;
  NumRegisters = num_of_registers;
}

void SetPollingTask(TaskHandle_t* task_handle) {
  /* Create the task without using any dynamic memory allocation. */
  *task_handle = xTaskCreateStatic(
      ReadTask,           /* Function that implements the task. */
      "USBProtoReadTask", /* Text name for the task. */
      kReadTaskStackSize, /* Number of indexes in the xStack array. */
      (void*)1,           /* Parameter passed into the task. */
      (kReadTaskPriority |
       portPRIVILEGE_BIT), /* Priority at which the task is created. */
      ReadTaskStack,       /* Array to use as the task's stack. */
      &staticReadTask);    /* Variable to hold the task's data structure. */
}

}  // namespace usb_service_protocol