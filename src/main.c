#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>

#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

#include "Descriptors.h"
#include "keyboard_matrix.h"

void SetupHardware(void);
void EVENT_USB_Device_Connect(void);
void EVENT_USB_Device_Disconnect(void);
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
void EVENT_USB_Device_StartOfFrame(void);

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo,
                                         uint8_t *const ReportID,
                                         const uint8_t ReportType,
                                         void *ReportData,
                                         uint16_t *const ReportSize);
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void *ReportData,
                                          const uint16_t ReportSize);

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
{
   .Config =
   {
      .InterfaceNumber        = INTERFACE_ID_Keyboard,
      .PrevReportINBuffer     = PrevKeyboardHIDReportBuffer,
      .PrevReportINBufferSize = sizeof(PrevKeyboardHIDReportBuffer),
      .ReportINEndpoint =
      {
         .Address = KEYBOARD_EPADDR,
         .Size    = KEYBOARD_EPSIZE,
         .Banks   = 1,
      }
   }
};

int main(void)
{
   key_matrix_init();

   /* for now overwrite the LED GPIO's to we can see stuff */
   struct gpio_cfg_s gpio_cfg = {.direction = GPIO_OUTPUT, .pup = GPIO_PDOWN};

   /* LED's HIGH and LOW are INVERTED on these LEDs */
   gpio_init(GPIOB, PB0, gpio_cfg);
   gpio_init(GPIOD, PD5, gpio_cfg);

   gpio_write(GPIOB, PB0, !GPIO_LOW);
   gpio_write(GPIOD, PD5, !GPIO_LOW);

   SetupHardware();

   /* Resolves to sei() avr call, enables global interrupt mask */
   GlobalInterruptEnable();

   while(1)
   {
      key_matrix_scan();
   	HID_Device_USBTask(&Keyboard_HID_Interface);
   	USB_USBTask();
   }
}

void SetupHardware()
{
   /* Disable watchdog if enabled by bootloader/fuses */
   MCUSR &= ~(1 << WDRF);
   wdt_disable();

   /* Disable clock division */
   clock_prescale_set(clock_div_1);

   USB_Init();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
   for (uint8_t i = 0; i < 12; i++)
   {
      gpio_write(GPIOD, PD5, !gpio_read(GPIOD, PD5));
      _delay_ms(100);
   }
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
   ;
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
   bool ConfigSuccess = true;

   ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);

   USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
   HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
   HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
}

/******************************************************************************
 *  @name CALLBACK_HID_Device_CreateHIDReport
 *
 *  @brief class driver callback function for the creation of HID reports to the host.
 *
 *  @param[in]     HIDInterfaceInfo  Pointer to the HID class interface configuration
 *                                   structure being referenced
 *  @param[in,out] ReportID          Report ID requested by the host if non-zero, otherwise
 *                                   callback should set to the generated report ID
 *  @param[in]     ReportType        Type of the report to create, either HID_REPORT_ITEM_In
 *                                   or HID_REPORT_ITEM_Feature
 *  @param[out]    ReportData        Pointer to a buffer where the created report should
 *                                   be stored
 *  @param[out]    ReportSize        Number of bytes written in the report (or zero if no
 *                                   report is to be sent)
 *
 *  @return Boolean \c true to force the sending of the report, \c false to let the
 * library determine if it needs to be sent
 *
 **************************************************** **************************/
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo,
                                         uint8_t *const ReportID,
                                         const uint8_t ReportType,
                                         void *ReportData,
                                         uint16_t *const ReportSize)
{
   USB_KeyboardReport_Data_t *keyboard_report = (USB_KeyboardReport_Data_t *)ReportData;
   memset(keyboard_report, 0, sizeof(USB_KeyboardReport_Data_t));

   key_matrix_report(&keyboard_report);

   *ReportSize = sizeof(USB_KeyboardReport_Data_t);
   return false;
}

/******************************************************************************
 *
 *  @name CALLBACK_HID_Device_ProcessHIDReport
 *
 *  @brief HID class driver callback function for the processing of HID reports
 *         from the host.
 *
 *  @param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration
 *                               structure being referenced
 *  @param[in] ReportID          Report ID of the received report from the host
 *  @param[in] ReportType        The type of report that the host has sent, either
 *                               HID_REPORT_ITEM_Out or HID_REPORT_ITEM_Feature
 *  @param[in] ReportData        Pointer to a buffer where the received report has been
 *                               stored
 *  @param[in] ReportSize        Size in bytes of the received HID report
 *
 * @return none
 *
 ******************************************************************************/
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void *ReportData,
                                          const uint16_t ReportSize)
{
   gpio_write(GPIOB, PB0, !gpio_read(GPIOB, PB0));;
}
