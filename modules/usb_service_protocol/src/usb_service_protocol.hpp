#ifndef USB_SERVICE_PROTOCOL
#include <FreeRTOS.h>
#include <task.h>
namespace usb_service_protocol {

inline constexpr uint8_t CMDSize = 10;

typedef struct {
char CMD[CMDSize];
uint8_t NumOfArgs;
const char* (*CMD_CB)(char **args, int num_of_args);
}ServiceRegisters;

void init(ServiceRegisters *regs, uint8_t num_of_registers);
void set_polling_task(TaskHandle_t *task_handle);

}

#endif