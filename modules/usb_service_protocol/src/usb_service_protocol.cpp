#include <Arduino.h>
#include <usb_service_protocol.hpp>

namespace usb_service_protocol {
USBServiceProtocolRegisters* serviceProtocolRegisters = NULL;
uint8_t NumRegisters = 0;

USBServiceProtocolRegisters* LastRegister;
inline constexpr char kTerminalEntryCharacter = '>';
inline constexpr char kNewLineCharacter = '\n';
inline constexpr uint8_t kBackspaceAsciiCode = 127;
inline constexpr uint8_t kCarriageReturnCharacter = '\r';
inline constexpr uint8_t kStrTerminationCharacter = '\0';
inline constexpr uint8_t kSpaceCharacter = ' ';
inline constexpr int kNoNewCharacter = -1;
inline constexpr char kPrintOnNextLine[] = "\r\n";
inline constexpr uint8_t kReadBufferSize = 100;
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

/* This function replaces spaces in the input string with terminated \0 character
 * It returns a pointer to the place where it has terminated+1 (new substring)
 * or NULL (if no space was found or end of string!). */
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

/* This function takes the readbuffer and parses its entered command and arguments from it
 * The arguments are parsed first then the command.. 
 * Parsing the arguments terminates the readbuffer at the first argument place 
 * (if arguments entered, spaces will be replaced with \0), making reading the command easier
 * Then a linear search is done to lookup the command in the register descriptions array (ServiceProtocolRegisters).
 * If the command name matches the command argument number is evaluated and the cb is executed! */
const char* Runcmd(char* buffer) {
  ParsedArgs args = Parsearg(buffer);
  for (uint8_t command_index = 0; command_index < NumRegisters; command_index++) {
    const bool command_found = (strcmp(serviceProtocolRegisters[command_index].cmd_, buffer) == 0);
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

/* ParseInput takes the character, read_buffer and stream command parameters from the ReadTask
 * and decides on basis of the character coming in: 
 * - To run the input command from the readbuffer (Carriage return character)
 * - Remove character from the readbuffer (Backspace character)
 * - Add character to readbuffer (any other character)
 * 
 * Edge case: Previous command is stream command.. When stream command is interrupted
 * an enter has to be entered in the serial console while running. 
 * But if the carriage return is parsed like normal, it would run an empty command.
 * Resulting in an command unrecognized error!
 * Thats why an extra if statement is added... To catch this specific condition*/
inline void ParseInput(char* read_buffer, int character,  const bool last_command_was_stream_cmd) {
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

/* This task is launched and will continuously monitor
 * the serial console for incoming characters.
 * Or run the previous command if the command is stream command.
 * (Stream commands are commands that run continuously until enter key pressed)*/
void ReadTask(void* pvParameters) {
  static char read_buffer[kReadBufferSize];
  int character = kNoNewCharacter;
  // Buffers are never terminated when uninitialized.
  // Therefore it needs to be memset. 
  // If not memset our parsing functions will not work!
  // Because it also will contain random data
  memset(read_buffer, kStrTerminationCharacter, kReadBufferSize);
  // This gives our lovely '>' character at the beginning of terminal
  Serial.write(kTerminalEntryCharacter);
  for (;;) {
    character = Serial.read();
    const bool last_command_was_stream_cmd = LastRegister->stream_cmd_;
    const bool new_character = character != kNoNewCharacter;
    if (new_character) {
      ParseInput(read_buffer, character, last_command_was_stream_cmd);
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