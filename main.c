#define UART_STRUCTS_VERSION 1
#include <time.h>
#include <unistd.h>
#include <applibs/networking.h>
#include <azure_sphere_provisioning.h>
#include <applibs/gpio.h>
#include <applibs/uart.h>
static int BlueLED;
static int GPSUART = -1;
static uint8_t ReceiveBuffer[65];
static ssize_t BytesRead;
static bool isNetworkReady = false;
static IOTHUB_DEVICE_CLIENT_LL_HANDLE iothubClientHandle = NULL;
static void SendMessageCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void* context) {
	if (result == 0) for (int i = 1; i <= 2; i++)
	{
		GPIO_SetValue(BlueLED, (GPIO_Value_Type)(1 - i % 2));
		nanosleep((struct timespec[]) { {1, 0} }, NULL);
	}
}
int main(int argc, char* argv[]) {
	UART_Config UARTConfig;
	UART_InitConfig(&UARTConfig);
	UARTConfig.baudRate = 115200;
	UARTConfig.flowControl = 0;
	GPSUART = UART_Open(4, &UARTConfig);
	while (!isNetworkReady)	Networking_IsNetworkingReady(&isNetworkReady);
	BlueLED = GPIO_OpenAsOutput(10, GPIO_OutputMode_PushPull, 0);
	IoTHubDeviceClient_LL_CreateWithAzureSphereDeviceAuthProvisioning(argv[1], 10000, &iothubClientHandle);
	while (1) {
		BytesRead = read(GPSUART, ReceiveBuffer, 64);
		if (BytesRead > 0) {
			ReceiveBuffer[BytesRead] = 0;
			IoTHubDeviceClient_LL_SendEventAsync(iothubClientHandle, IoTHubMessage_CreateFromString((char*)ReceiveBuffer), SendMessageCallback, NULL);
			IoTHubDeviceClient_LL_DoWork(iothubClientHandle);
		}
		nanosleep((struct timespec[]) { {1, 0} }, NULL);
	}
}