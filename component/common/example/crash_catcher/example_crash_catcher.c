#include <platform_opts.h>

#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"


void vApplicationDaemonTaskStartupHook( void );

static void example_crash_catcher_thread(void *pvParameters)
{
#if !defined(configUSE_DAEMON_TASK_STARTUP_HOOK) || (configUSE_DAEMON_TASK_STARTUP_HOOK == 0)
    vApplicationDaemonTaskStartupHook();
#endif
    vTaskDelete(NULL);
    return;
}

void example_crash_catcher(void)
{
    if(xTaskCreate(example_crash_catcher_thread, ((const char*)"example_crash_catcher_thread"), 2048, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
        printf("\n\r%s xTaskCreate(example_crash_catcher_thread) failed", __FUNCTION__);
}

/*-----------------------------------------------------------*/
/* BHJ */
TaskHandle_t bhjHandle = NULL;

typedef struct {
    uint32_t type;
    const char* message;
} TaskArg_t;

TaskArg_t arg = { 1337, "Hello User button" };

void bhjCrash()
{
    int (*foo)(int i, int j, int k, int l, int m);
    foo = (int(*)(int,int,int,int,int))0xbadc0de;
    configPRINTF((" Crash!"));
    // Arg 1-4 in registers 5 on stack
    foo(2,3,4,5,6);
}

void devalert()
{
    bhjCrash();
}

void rocks()
{
    devalert();
}

void CrashTask(void* argument)
{
    configPRINTF(("CrashTask started\n"));
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    uint32_t count = 10;
    for (;;) {
        vTaskDelay(xDelay);
        if (count-- == 0)
        {
            rocks();
            count = 10;
        }
    }
}

void vApplicationDaemonTaskStartupHook( void )
{
    /* FIX ME: Perform any hardware initialization, that require the RTOS to be
     * running, here. */

    /* FIX ME: If your MCU is using Wi-Fi, delete surrounding compiler directives to
     * enable the unit tests and after MQTT, Bufferpool, and Secure Sockets libraries
     * have been imported into the project. If you are not using Wi-Fi, see the
     * vApplicationIPNetworkEventHook function. */
    //#if 0
        //if( SYSTEM_Init() == pdPASS )
        {
            /* Connect to the Wi-Fi before running the tests. */
            /*prvWifiConnect();*/

            /* Provision the device with AWS certificate and private key. */
/*            vDevModeKeyProvisioning();*/

            configPRINTF(("Starting crash Task\n"));
            BaseType_t xReturned = xTaskCreate(
                CrashTask,
                "DemoTask",
                512,
                (void*)&arg,
                tskIDLE_PRIORITY,
                &bhjHandle);

            /* Start the demo tasks. */
            /*DEMO_RUNNER_RunDemos();*/
        }
    //#endif
}
