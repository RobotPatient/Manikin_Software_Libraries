#include <Arduino.h>
#include <usb_service_protocol.hpp>

namespace usb_service_protocol {
inline constexpr char VT100_MOVE_CHARACTER_BACK_1_POS[] = "\33[2D";
inline constexpr char TERMINAL_ENTRY_CHARACTER = '>';
inline constexpr uint8_t BACKSPACE_ASCII_CODE = 127;
inline constexpr uint8_t CARRIAGE_RETURN_CHARACTER = '\r';
inline constexpr int NO_NEW_CHARACTER = -1;
inline constexpr char COMMAND_OK[] = "!OK ";
inline constexpr char COMMAND_ERR[] = "!ERR ";
uint8_t read_index = 0;

ServiceRegisters* registers = NULL;
uint8_t NumRegisters = 0;

char* GETARG(char* input) {
  if (input == NULL)
    return NULL;
  do {
    input++;
  } while (*input != ' ' && *input != '\0' && *input != '\r');
  if (*input == '\0')
    return NULL;
  *input = '\0';
  return input + 1;
}

uint8_t ValidateCMD(char* buffer) {
  uint8_t command_recognized = 0;
  uint8_t argNum;
  static char* args[10];
  args[0] = GETARG(buffer);
  for (uint8_t i = 0; i < NumRegisters; i++) {
    if (strcmp(registers[i].CMD, buffer) == 0) {
      argNum = 0;
      command_recognized = 1;
      if (args[0] != NULL) {
        args[1] = GETARG(args[0]);
        Serial.println("There are more args!");
        Serial.printf("num_of_args: %d \r\n", argNum);
        Serial.printf("Arg0: %s, Arg1: %s \r\n", args[0], args[1]);
      }
      registers[i].CMD_CB(args, argNum);

      break;
    }
  }
  return command_recognized;
}

/* Task to be created. */
void POLL_READ(void* pvParameters) {
  char buffer[100];
  memset(buffer, '\0', 100);
  Serial.print(TERMINAL_ENTRY_CHARACTER);
  for (;;) {
    int Character = Serial.read();
    if (Character != NO_NEW_CHARACTER) {
      switch (Character) {
        case NO_NEW_CHARACTER:
          break;
        case BACKSPACE_ASCII_CODE: {
          buffer[read_index] = '\0';
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
          if(!ValidateCMD(buffer)) {
            Serial.print("!E Command unrecognized!");
          }
          Serial.print('\n');
          Serial.printf("%c", Character);
          Serial.print(TERMINAL_ENTRY_CHARACTER);
          memset(buffer, '\0', 100);
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