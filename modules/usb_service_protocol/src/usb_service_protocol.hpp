#ifndef USB_SERVICE_PROTOCOL_HPP
#define USB_SERVICE_PROTOCOL_HPP
#include <FreeRTOS.h>
#include <task.h>
namespace usb_service_protocol {

/* The max amount of arguments a command can take...
 * The default value is 10. */
inline constexpr uint8_t kUSBProtoMaxAmountOfArguments = 10;

/* The register struct..
 * - cmd contains the command in string format:
 * e.g. Command help would be "HELP".
 * - num_of_args contains the amount of arguments the command takes
 * - stream_cmd indicates if the command gives streaming output, 
 * meaning it will be run continuously until killed by an enter key press.
 * - cmd_cb will have the pointer to the CB to run when CMD requested from Console
 * The returned string of this cb will be printed to user in console. */
typedef struct {
  const char cmd_[kUSBProtoMaxAmountOfArguments];
  uint8_t num_of_args_;
  bool stream_cmd_;
  const char* (*cmd_cb)(char** args, int num_of_args);
} USBServiceProtocolRegisters;

/**
 * @brief Init the USB service protocol, this will set the internal registers according to the parameters
 * 
 * @param registers Pointer to registerdata array containing the register info and cb function
 * @param num_of_registers The total number of registers stored in the registerdata array
 */
void Init(USBServiceProtocolRegisters* registers, uint8_t num_of_registers);

/**
 * @brief Set background polling task for the USB service module.
 *        The task set will be monitoring the console character by character.
 *        Warning! Module needs to be initialized first!
 * 
 * @param task_handle Pointer to external task_handle to attach background task to
 */
void SetPollingTask(TaskHandle_t* task_handle);

}  // namespace usb_service_protocol

#endif