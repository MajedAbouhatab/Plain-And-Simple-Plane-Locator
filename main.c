#define UART_STRUCTS_VERSION 1
#include <time.h>
#include <unistd.h>
#include <azure_sphere_provisioning.h>
#include <applibs/log.h>
#include <applibs/gpio.h>
#include <applibs/uart.h>
static int BlueLED;
static int uartFd = -1;
static uint8_t receiveBuffer[65];
static ssize_t bytesRead;
static IOTHUB_DEVICE_CLIENT_LL_HANDLE iothubClientHandle = NULL;
static void SendMessageCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* context) { Log_Debug("INFO: Message received by IoT Hub. Result is: %d\n", result); }
int main(int argc, char* argv[])
{
	BlueLED = GPIO_OpenAsOutput(10, GPIO_OutputMode_PushPull, 0);
	UART_Config UARTConfig;
	UART_InitConfig(&UARTConfig);
	UARTConfig.baudRate = 115200;
	UARTConfig.flowControl = 0;
	uartFd = UART_Open(4, &UARTConfig);
	IoTHubDeviceClient_LL_CreateWithAzureSphereDeviceAuthProvisioning(argv[1], 10000, &iothubClientHandle);
	while (1) {
		nanosleep((struct timespec[]) { { 1, 0 } }, NULL);
		bytesRead = read(uartFd, receiveBuffer, 64);
		if (bytesRead > 0) {
			for (int i = 1; i <= 4; i++)
			{
				GPIO_SetValue(BlueLED, (GPIO_Value_Type)(1 - i % 2));
				nanosleep((struct timespec[]) { { 0, 50000000 } }, NULL);
			}
			receiveBuffer[bytesRead] = 0;
			IoTHubDeviceClient_LL_SendEventAsync(iothubClientHandle, IoTHubMessage_CreateFromString((char*)receiveBuffer), SendMessageCallback, /*&callback_param*/ 0);
			IoTHubDeviceClient_LL_DoWork(iothubClientHandle);
		}
	}
}