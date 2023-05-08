#include <Arduino.h>
#include <usb_service_protocol.hpp>

namespace usb_service_protocol {
ServiceRegisters* registers = NULL;
uint8_t NumRegisters = 0;

inline constexpr char VT100_MOVE_CHARACTER_BACK_1_POS[] = "\33[2D";
inline constexpr char TERMINAL_ENTRY_CHARACTER = '>';
inline constexpr uint8_t BACKSPACE_ASCII_CODE = 127;
inline constexpr uint8_t CARRIAGE_RETURN_CHARACTER = '\r';
inline constexpr uint8_t STR_TERMINATION_CHARACTER = '\0';
inline constexpr uint8_t SPACE_CHARACTER = ' ';
inline constexpr int NO_NEW_CHARACTER = -1;
inline constexpr char COMMAND_OK[] = "!OK ";
inline constexpr char COMMAND_ERR[] = "!ERR ";
inline constexpr uint8_t READ_BUFFER_SIZE = 100;
inline constexpr uint8_t MAX_AMOUNT_OF_ARGUMENTS = 10;

typedef struct {
  char* argsBuffer[MAX_AMOUNT_OF_ARGUMENTS];
  uint8_t argNum;
} ParsedArgs;

char* GETARG(char* input) {
  if (input == NULL) return NULL;
  // Loop over the passed in string, 
  // till space-, termination- or carriage-return character encountered
  do {
    input++;
  } while (*input != SPACE_CHARACTER && 
           *input != STR_TERMINATION_CHARACTER &&
           *input != CARRIAGE_RETURN_CHARACTER);
  // Termination char means end of the string
  if (*input == '\0')  return NULL;
  // The reason for inserting the STR termination char
  // Is to force other c functions like printf to stop reading the
  // string at the place where the space was..
  // This way we get a nice formatted string of which we only have to
  // save the pointers of the places in the string where arguments are saved :)
  *input = STR_TERMINATION_CHARACTER;
  // This is the pointer to a new possible argument, not the one we found..
  return input + 1;
}

ParsedArgs PARSEARG(char* buffer) {
  ParsedArgs Args;
  Args.argNum = 0;
  Args.argsBuffer[0] = GETARG(buffer);
  if (Args.argsBuffer[0] != NULL) {
    for (uint8_t i = 1; i < MAX_AMOUNT_OF_ARGUMENTS; i++) {
      Args.argsBuffer[i] = GETARG(Args.argsBuffer[i - 1]);
      if (Args.argsBuffer[i] == NULL)
        break;
      else
        Args.argNum++;
    }
  }
  return Args;
}

const char* RUNCMD(char* buffer) {
  ParsedArgs args = PARSEARG(buffer);
  for (uint8_t i = 0; i < NumRegisters; i++) {
    if (strcmp(registers[i].CMD, buffer) == 0) {
      if (args.argNum > registers[i].NumOfArgs)
        return "!E Too many arguments!";
      else if (args.argNum < registers[i].NumOfArgs)
        return "!E Too few arguments!";
      else
        return registers[i].CMD_CB(args.argsBuffer, args.argNum);
      break;
    }
  }
  return "!E Command unrecognized!";
}

void POLL_READ(void* pvParameters) {
  uint8_t read_index = 0;
  char buffer[READ_BUFFER_SIZE];
  memset(buffer, '\0', READ_BUFFER_SIZE);
  Serial.print(TERMINAL_ENTRY_CHARACTER);
  for (;;) {
    int Character = Serial.read();
    if (Character != NO_NEW_CHARACTER) {
      switch (Character) {
        case NO_NEW_CHARACTER: {
          break;
        }
        case BACKSPACE_ASCII_CODE: {
          buffer[read_index] = STR_TERMINATION_CHARACTER;
          if (read_index != 0) {
            Serial.printf("%c", Character);
            read_index--;
          }
          break;
        }
        case CARRIAGE_RETURN_CHARACTER: {
          Serial.print('\n');
          buffer[read_index++] = Character;
          Serial.printf("%c", Character);
          Serial.print(RUNCMD(buffer));
          Serial.print('\n');
          Serial.printf("%c", Character);
          Serial.print(TERMINAL_ENTRY_CHARACTER);
          memset(buffer, STR_TERMINATION_CHARACTER, READ_BUFFER_SIZE);
          read_index = 0;
          break;
        }
        default: {
          Serial.printf("%c", Character);
          buffer[read_index++] = Character;
          break;
        }
      }
    }
  }
}

void init(ServiceRegisters* regs, uint8_t num_of_registers) {
  registers = regs;
  NumRegisters = num_of_registers;
}

/* Dimensions of the buffer that the task being created will use as its stack.
    NOTE:  This is the number of words the stack will hold, not the number of
    bytes.  For example, if each stack item is 32-bits, and this is set to 100,
    then 400 bytes (100 * 32-bits) will be allocated. */
#define STACK_SIZE 200

/* Structure that will hold the TCB of the task being created. */
StaticTask_t xTaskBuffer;

/* Buffer that the task being created will use as its stack.  Note this is
    an array of StackType_t variables.  The size of StackType_t is dependent on
    the RTOS port. */
StackType_t xStack[STACK_SIZE];

void set_polling_task(TaskHandle_t* task_handle) {
  BaseType_t xReturned;
  /* Create the task without using any dynamic memory allocation. */
  *task_handle = xTaskCreateStatic(
      POLL_READ,               /* Function that implements the task. */
      "pollread",              /* Text name for the task. */
      STACK_SIZE,              /* Number of indexes in the xStack array. */
      (void*)1,                /* Parameter passed into the task. */
      (2 | portPRIVILEGE_BIT), /* Priority at which the task is created. */
      xStack,                  /* Array to use as the task's stack. */
      &xTaskBuffer);           /* Variable to hold the task's data structure. */

  if (xReturned == pdPASS) {
    Serial.println("Task created succesfully!");
  }
}

}  // namespace usb_service_protocol