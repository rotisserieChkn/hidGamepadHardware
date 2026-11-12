#include "main.h"
#include "usb_device.h"
#include "app.h"

//#define UART_DEBUG 1

extern USBD_HandleTypeDef hUsbDeviceFS;

///////////////////////////////////////////////////
//ENCODER VARS

uint16_t myEncoderCounts1 = 0;
int16_t myEncdrPosition1_TOUSB = 0;

char encoderChar1[] = "ENC hello I am a char";
int numCharWrittenEncoder1 = 0;

uint16_t myEncoderCounts2 = 0;
int16_t myEncdrPosition2_TOUSB = 0;

char encoderChar2[] = "ENC hello I am a char";
int numCharWrittenEncoder2 = 0;
///////////////////////////////////////////////////


///////////////////////////////////////////////////
//LOAD_CELL VARS
uint16_t myAdcValues[3] = {0};

char adc1char[] = "1 hello I am a char";
char adc2char[] = "2 hello I am a char";
char adc3char[] = "3 hello I am a char";

int numCharWritten1 = 0;
int numCharWritten2 = 0;
int numCharWritten3 = 0;

int16_t loadCell1CalVal = 0;
int16_t loadCell2CalVal = 0;
int16_t loadCell3CalVal = 0;

int16_t loadCell1Val = 0;
int16_t loadCell2Val = 0;
int16_t loadCell3Val = 0;

int16_t loadCell1Val_TOUSB = 0;
int16_t loadCell2Val_TOUSB = 0;
int16_t loadCell3Val_TOUSB = 0;

int16_t past_loadCell1Val[LOAD_CELL_NUM_AVG] = {0};
int16_t past_loadCell2Val[LOAD_CELL_NUM_AVG] = {0};
int16_t past_loadCell3Val[LOAD_CELL_NUM_AVG] = {0};

int average_int_loadCell1Val = 0;
int average_int_loadCell2Val = 0;
int average_int_loadCell3Val = 0;

int loadCellArrayIndexCounter = 0;
///////////////////////////////////////////////////



///////////////////////////////////////////////////
//BUTTON VARS
uint8_t buttonStates_TOUSB = 0;

char buttonStatesChar[] = "I am a char!!!";

int numCharWrittenButton = 0;
///////////////////////////////////////////////////




void myAppInit(){
	
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);

	HAL_ADC_Start_DMA(&hadc1, myAdcValues, 3);

	HAL_Delay(1000);

	loadCell1CalVal = myAdcValues[0];
	loadCell2CalVal = myAdcValues[1];
	loadCell3CalVal = myAdcValues[2];
}


void myAppRun(){
		  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	  //ENCODER DATA COLLECTION
	  //TODO make this tolerant of turning the Encdr too far
	    myEncoderCounts1 = 2*(__HAL_TIM_GET_COUNTER(&htim1));
	    if(myEncoderCounts1 >= 30000){
	    	myEncdrPosition1_TOUSB = -1*(65535 - myEncoderCounts1);
	    }else{
	    	myEncdrPosition1_TOUSB = myEncoderCounts1;
	    }

	    myEncoderCounts2 = 2*(__HAL_TIM_GET_COUNTER(&htim3));
	    if(myEncoderCounts2 >= 30000){
	    	myEncdrPosition2_TOUSB = -1*(65535 - myEncoderCounts2);
	    }else{
	    	myEncdrPosition2_TOUSB = myEncoderCounts2;
	    }
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//LOAD_CELL DATA COLLECTION
		loadCell1Val = myAdcValues[0];
		loadCell2Val = myAdcValues[1];
		loadCell3Val = myAdcValues[2];

		loadCell1Val = LOAD_CELL_1_SCALE_FACTOR*(loadCell1CalVal - loadCell1Val);
		loadCell2Val = LOAD_CELL_2_SCALE_FACTOR*(loadCell2Val - loadCell2CalVal);
		loadCell3Val = LOAD_CELL_3_SCALE_FACTOR*(loadCell3Val - loadCell3CalVal);

		if(loadCell1Val < 0){
			loadCell1Val = 0;
		}else if(loadCell1Val > 1023){
			loadCell1Val = 1023;
		}

		if(loadCell2Val < 0){
			loadCell2Val = 0;
		}else if(loadCell2Val > 1023){
			loadCell2Val = 1023;
		}

		if(loadCell3Val < 0){
			loadCell3Val = 0;
		}else if(loadCell3Val > 1023){
			loadCell3Val = 1023;
		}

		//take an average of the past LOAD_CELL_NUM_AVG samples
		past_loadCell1Val[loadCellArrayIndexCounter] = loadCell1Val;
		past_loadCell2Val[loadCellArrayIndexCounter] = loadCell2Val;
		past_loadCell3Val[loadCellArrayIndexCounter] = loadCell3Val;

		loadCellArrayIndexCounter++;
		if(loadCellArrayIndexCounter == LOAD_CELL_NUM_AVG){
		  loadCellArrayIndexCounter = 0;
		}

		average_int_loadCell1Val = 0;
		average_int_loadCell2Val = 0;
		average_int_loadCell3Val = 0;

		for(int x=0; x<LOAD_CELL_NUM_AVG; x++){
			average_int_loadCell1Val = average_int_loadCell1Val + past_loadCell1Val[x];
			average_int_loadCell2Val = average_int_loadCell2Val + past_loadCell2Val[x];
			average_int_loadCell3Val = average_int_loadCell3Val + past_loadCell3Val[x];
		}

		average_int_loadCell1Val = average_int_loadCell1Val / LOAD_CELL_NUM_AVG;
		average_int_loadCell2Val = average_int_loadCell2Val / LOAD_CELL_NUM_AVG;
		average_int_loadCell3Val = average_int_loadCell3Val / LOAD_CELL_NUM_AVG;

		loadCell1Val_TOUSB = average_int_loadCell1Val;
		loadCell2Val_TOUSB = average_int_loadCell2Val;
		loadCell3Val_TOUSB = average_int_loadCell3Val;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//BUTTON DATA COLLECTION

		buttonStates_TOUSB = 0;

		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET){
			buttonStates_TOUSB = buttonStates_TOUSB | 0b00000001;
		}
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET){
			buttonStates_TOUSB = buttonStates_TOUSB | 0b00000010;
		}
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) == GPIO_PIN_RESET){
			buttonStates_TOUSB = buttonStates_TOUSB | 0b00000100;
		}
		if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_RESET){
			buttonStates_TOUSB = buttonStates_TOUSB | 0b00001000;
		}
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == GPIO_PIN_RESET){
			buttonStates_TOUSB = buttonStates_TOUSB | 0b00010000;
		}

#ifdef UART_DEBUG

		//transmit encoder data
	    numCharWrittenEncoder1 = snprintf(encoderChar1, 5, "%04d", (int32_t)myEncdrPosition1_TOUSB);
	    numCharWrittenEncoder2 = snprintf(encoderChar2, 5, "%04d", (int32_t)myEncdrPosition2_TOUSB);

		HAL_UART_Transmit(&huart1, "Encoder 1 Counts: ", 18, 1000);
		HAL_UART_Transmit(&huart1, encoderChar1, 4, 1000);
		HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);
		HAL_UART_Transmit(&huart1, "Encoder 2 Counts: ", 18, 1000);
		HAL_UART_Transmit(&huart1, encoderChar2, 4, 1000);
		HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);

		//transmit ADC data
		numCharWritten1 = snprintf(adc1char, 5, "%04d", (uint32_t)loadCell1Val_TOUSB);
		numCharWritten2 = snprintf(adc2char, 5, "%04d", (uint32_t)loadCell2Val_TOUSB);
		numCharWritten3 = snprintf(adc3char, 5, "%04d", (uint32_t)loadCell3Val_TOUSB);

		HAL_UART_Transmit(&huart1, "ADC 1 Reading: ", 15, 1000);
		HAL_UART_Transmit(&huart1, adc1char, 4, 1000);
		HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);
		HAL_UART_Transmit(&huart1, "ADC 2 Reading: ", 15, 1000);
		HAL_UART_Transmit(&huart1, adc2char, 4, 1000);
		HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);
		HAL_UART_Transmit(&huart1, "ADC 3 Reading: ", 15, 1000);
		HAL_UART_Transmit(&huart1, adc3char, 4, 1000);
		HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);

		//transmit button states
		numCharWrittenButton = snprintf(buttonStatesChar, 5, "%04d", (uint32_t)buttonStates_TOUSB);

		HAL_UART_Transmit(&huart1, "Button State Value: ", 20, 1000);
		HAL_UART_Transmit(&huart1, buttonStatesChar, 4, 1000);
		HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);

		HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);
		HAL_UART_Transmit(&huart1, "\r\n", 2, 1000);

		HAL_Delay(400);
#endif

		SendGamepadReport(myEncdrPosition1_TOUSB, myEncdrPosition2_TOUSB, loadCell1Val_TOUSB, loadCell2Val_TOUSB, loadCell3Val_TOUSB, buttonStates_TOUSB);
		HAL_Delay(1);
}




void SendGamepadReport(int16_t s1, int16_t s2, uint16_t p1, uint16_t p2, uint16_t p3, uint8_t buttons){

	HID_GamepadReport_t gamepad = {0};
	gamepad.steering1_low = (uint8_t)s1;
	gamepad.steering1_high = s1>>8;
	gamepad.steering2_low = (uint8_t)s2;
	gamepad.steering2_high = s2>>8;
	gamepad.loadCell1_low = (uint8_t)p1;
	gamepad.loadCell1_high = p1>>8;
	gamepad.loadCell2_low = (uint8_t)p2;
	gamepad.loadCell2_high = p2>>8;
	gamepad.loadCell3_low = (uint8_t)p3;
	gamepad.loadCell3_high = p3>>8;
	gamepad.buttons = buttons;
    USBD_HID_SendReport(&hUsbDeviceFS, &gamepad, 11);
}










uint16_t flip_endian_16(uint16_t value) {
    // Shift the lower 8 bits to the upper 8 bits position
    // and the upper 8 bits to the lower 8 bits position, then combine them.
    return (value >> 8) | (value << 8);
}
