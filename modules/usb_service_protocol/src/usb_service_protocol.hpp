#ifndef USB_SERVICE_PROTOCOL_HPP
#define USB_SERVICE_PROTOCOL_HPP
#include <FreeRTOS.h>
#include <task.h>
namespace usb_service_protocol {

inline constexpr uint8_t kUSBProtoMaxAmountOfArguments = 10;

typedef struct {
char cmd_[kUSBProtoMaxAmountOfArguments];
uint8_t num_of_args_;
bool stream_cmd_;
const char* (*cmd_cb)(char **args, int num_of_args);
} USBServiceProtocolRegisters;

void Init(USBServiceProtocolRegisters * registers, uint8_t num_of_registers);
void SetPollingTask(TaskHandle_t *task_handle);

}

#endif