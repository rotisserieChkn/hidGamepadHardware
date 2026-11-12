

#define LOAD_CELL_NUM_AVG 5

#define LOAD_CELL_1_SCALE_FACTOR 4
#define LOAD_CELL_2_SCALE_FACTOR 3
#define LOAD_CELL_3_SCALE_FACTOR 3

//this is not an include, but this is the device descriptor for my USB HID game pad device that has a racing Encdr and three loadCells
//i am leaving this here because cubeIDE loves to delete it when it regenerates code. this belongs in Middlewares/ST/STM32_USB_Device_Library/Class/HID/Src/usbd_hid.c
//descriptor length is 64, this define value also needs updated every cubeMX update
/*


0x05, 0x01,                    // Usage Page (Generic Desktop)
0x09, 0x05,                    // Usage (Game Pad)
0xA1, 0x01,                    // Collection (Application)
    // Analog Axes (-512 to 512, e.g., X and Y)
    0x05, 0x01,                //   Usage Page (Generic Desktop)
    0x09, 0x30,                //   Usage (X)
    0x09, 0x31,                //   Usage (Y)
    0x15, 0x00, 0xFE,          //   Logical Minimum (-512)
    0x25, 0x00, 0x02,          //   Logical Maximum (512)
    0x16, 0x00, 0xFE,          //   Physical Minimum (-512)
    0x26, 0x00, 0x02,          //   Physical Maximum (512)
    0x75, 0x10,                //   Report Size (16 bits)
    0x95, 0x02,                //   Report Count (2)
    0x81, 0x02,                //   Input (Data, Variable, Absolute)

    // Analog Inputs (0 to 1023, e.g., Z, Rx, Ry)
    0x05, 0x01,                //   Usage Page (Generic Desktop)
    0x09, 0x32,                //   Usage (Z)
    0x09, 0x33,                //   Usage (Rx)
    0x09, 0x34,                //   Usage (Ry)
    0x15, 0x00,                //   Logical Minimum (0)
    0x25, 0xFF, 0x03,          //   Logical Maximum (1023)
    0x16, 0x00, 0x00,          //   Physical Minimum (0)
    0x26, 0xFF, 0x03,          //   Physical Maximum (1023)
    0x75, 0x10,                //   Report Size (16 bits)
    0x95, 0x03,                //   Report Count (3)
    0x81, 0x02,                //   Input (Data, Variable, Absolute)

    // Buttons (8 buttons in one 8-bit field)
    0x05, 0x09,                //   Usage Page (Button)
    0x19, 0x01,                //   Usage Minimum (Button 1)
    0x29, 0x08,                //   Usage Maximum (Button 8)
    0x15, 0x00,                //   Logical Minimum (0)
    0x25, 0x01,                //   Logical Maximum (1)
    0x75, 0x01,                //   Report Size (1 bit)
    0x95, 0x08,                //   Report Count (8)
    0x81, 0x02,                //   Input (Data, Variable, Absolute)
0xC0                           // End Collection


 */




void SendGamepadReport(int16_t s1, int16_t s2, uint16_t p1, uint16_t p2, uint16_t p3, uint8_t buttons);
uint16_t flip_endian_16(uint16_t value);
void myAppInit();
void myAppRun();



extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;






///////////////////////////////////////////////////
//ENCODER VARS
extern uint16_t myEncoderCounts1;
extern int16_t myEncdrPosition1_TOUSB;
extern char encoderChar1[];
extern int numCharWrittenEncoder1;
extern uint16_t myEncoderCounts2;
extern int16_t myEncdrPosition2_TOUSB;
extern char encoderChar2[];
extern int numCharWrittenEncoder2;
///////////////////////////////////////////////////




///////////////////////////////////////////////////
//LOAD_CELL VARS
extern uint16_t myAdcValues[3];
extern char adc1char[];
extern char adc2char[];
extern char adc3char[];
extern int numCharWritten1;
extern int numCharWritten2;
extern int numCharWritten3;
extern int16_t loadCell1CalVal;
extern int16_t loadCell2CalVal;
extern int16_t loadCell3CalVal;
extern int16_t loadCell1Val;
extern int16_t loadCell2Val;
extern int16_t loadCell3Val;
extern int16_t loadCell1Val_TOUSB;
extern int16_t loadCell2Val_TOUSB;
extern int16_t loadCell3Val_TOUSB;
extern int16_t past_loadCell1Val[LOAD_CELL_NUM_AVG];
extern int16_t past_loadCell2Val[LOAD_CELL_NUM_AVG];
extern int16_t past_loadCell3Val[LOAD_CELL_NUM_AVG];
extern int average_int_loadCell1Val;
extern int average_int_loadCell2Val;
extern int average_int_loadCell3Val;
extern int loadCellArrayIndexCounter;
///////////////////////////////////////////////////





///////////////////////////////////////////////////
//BUTTON VARS

extern uint8_t buttonStates_TOUSB;
extern char buttonStatesChar[];
extern int numCharWrittenButton;
///////////////////////////////////////////////////




typedef struct {

    uint8_t steering1_low;
    uint8_t steering1_high;
    uint8_t steering2_low;
    uint8_t steering2_high;
    uint8_t loadCell1_low;
    uint8_t loadCell1_high;
    uint8_t loadCell2_low;
    uint8_t loadCell2_high;
    uint8_t loadCell3_low;
    uint8_t loadCell3_high;
    uint8_t buttons;


} __packed HID_GamepadReport_t;
