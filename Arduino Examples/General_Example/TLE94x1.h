/*********************************************************************************************************************
 * Copyright (c) 2019, Infineon Technologies AG
 *
 * 
 * Distributed under the Boost Software License, Version 1.0.
 * 
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 *********************************************************************************************************************/


/****************************************************************************************************//**
 * @file        TLE94x1.h
 *
 * @brief       Main header declaration file for TLE94x1 SBC family device
 *
 * @version     V1.0.0
 * @date        15. April 2019
 * @author      Markus Noll / markus.noll@infineon.com
 * @author      Yannek Micha Rixen / Yannek.Rixen@infineon.com
 *******************************************************************************************************/



 
#ifndef TLE94x1_H
#define TLE94x1_H
 





/* ================================================================================ */
/* ============================   HEADER FILES     ================================ */
/* ================================================================================ */ 


 
#include "TLE94x1_DEFINES.h"
#include "TLE94x1_ISR.h"
#include "TLE94x1_SPI.h"
#include <stdint.h>




/* ================================================================================ */
/* ================================   MACROS     ================================== */
/* ================================================================================ */ 


/**
 * @def     SBC_Write_Bit
 * 
 * @brief   This bit has to be set in order to write to a register.
 */
#define SBC_Write_Bit   (0x80U)

/**
 * @def     SBC_Read_Mask
 * 
 * @brief   A mask to prevent the user from accidentally writing to a register.
 */
#define SBC_Read_Mask   (0x7FU)




/* ================================================================================ */
/* ===========================   Library Functions     ============================ */
/* ================================================================================ */ 

struct __SBC_ErrorCode;

/**
 * @brief  A structure for simple error readout.
 * 
 *         flippedBitsMask is greater than 0 if the value read from the register at SBC_Register differs from expectedValue.
 */
typedef struct __SBC_ErrorCode {
    uint8_t SBC_Register;       //!< The register where an error occurred.
    uint8_t flippedBitsMask;    //!< Masks the bits that differ from the expected value. Is 0 if readout is as expected.
    uint8_t expectedValue;      //!< Expected readout of the register.
} SBC_ErrorCode;

/**
 * @brief   Typedef for interrupt callbacks.
 */
typedef void (*SBC_Func_Callback)(uint8_t callbackHandler);






/* --------------------------------  Main Functions ------------------------------- */


/**
 * @brief   This method will trigger the watchdog.
 * 
 *          The function must be called periodically according to the configured watchdog-time.
 */
SBC_ErrorCode SBC_WD_Trigger(void);


/**
 * @brief   This method will proceed a readout of a register.
 * 
 * @param   SBC_Reg     Address of the register to be read out. See TLE94x1_DEFINES.h for definitions
 * @retval  A 16 bit value will be returned.
 *          Bit[15:8] is the Status-Information-Field, Bit [7:0] is the read register-value.
 *          For furhter information of the Status-Information-Field see chapter 13.3 in the datasheet.
 */
uint16_t SBC_Read_Command(uint8_t SBC_Reg);


/**
 * @brief   This method will proceed a readout of a dedicated bitfield within a register
 * 
 * @param   SBC_Reg         Address of the register to be readout. See TLE94x1_DEFINES.h for definitions
 * @param   SBC_FieldMsk    Bit mask of the field to be readout. See TLE94x1_DEFINES.h for definitions
 * @param   SBC_FieldPos    Bit position of the field to be readout. See TLE94x1_DEFINES.h for definitions
 * 
 * 
 * @retval  A 8 bit value will be returned and includes the data of the bitfield to be read out *
 */
uint8_t SBC_Read_RegField(uint8_t SBC_Reg, uint8_t SBC_FieldMsk, uint8_t SBC_FieldPos);



/**
 * @brief   Writes a whole byte to a register and verifies it.
 * 
 * @param   SBC_Reg         Address of the register to be manipulated. See TLE94x1_DEFINES.h for definitions
 * @param   SBC_Val         Byte to write to SBC_Reg
 * @param   *returnval      A 16 bit value will be returned.
 *                          Bit[15:8] is the Status-Information-Field, Bit [7:0] is the value of the manipulated register before write
 *                          For furhter information of the Status-Information-Field see chapter 13.3 in the datasheet.
 * 
 * @retval  See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Write_Reg(uint8_t SBC_Reg, uint8_t SBC_Val, uint16_t * returnval);


/**
 * @brief   This method can be used for manipulating a single bit-field in a control-register.
 *
 *          It will readout the old value of the registers, manipulate the desired bit-field and keep
 *          the other bit-configuration as it was.
 *          For usage examples have a look at the implementations of different API calls below.
 * 
 * @param   SBC_Reg         Address of the register to be manipulated. See TLE94x1_DEFINES.h for definitions
 * @param   SBC_FieldMsk    Bit mask of the field to manipulate. See TLE94x1_DEFINES.h for definitions
 * @param   SBC_FieldPos    Bit position of the field to manipulate. See TLE94x1_DEFINES.h for definitions
 * @param   SBC_FieldVal    New value which will be written to the bit-field. See TLE94x1_DEFINES.h for enumerations
 * @param   *returnval      A 16 bit value will be returned.
 *                          Bit[15:8] is the Status-Information-Field, Bit [7:0] is the value of the manipulated register before write
 *                          For furhter information of the Status-Information-Field see chapter 13.3 in the datasheet.
 * 
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 *
 */
SBC_ErrorCode SBC_Write_RegField(uint8_t SBC_Reg, uint8_t SBC_FieldMsk, uint8_t SBC_FieldPos, uint8_t SBC_FieldVal, uint16_t * returnval);


/**
 * @brief   This method must be called one time at startup of the microcontroller.
 *
 *          This method will initialize all registers of the SBC with the configuration-data of SBC_TLE94x1.h.
 *          After this, the SBC can be used as normal.
 * 
 * @retval  SBC_ErrorCode    See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Init(void);

/* --------------------------------  ISR Functions -------------------------------- */


/**
 * @brief   This function can register a self-defined function to a specific interrupt-event of the SBC.
 * 
 *          Everytime the SBC_ISR() method is called and the associated status-bit is set, it will consider to
 *          proceed a callback to this function later.
 *          See all the possible ISR_Vectors in the TLE94x1_ISR.h
 *
 * 
 * @param   ISR_Vector   Definition of the interrupt event. See all possible events in TLE94x1_ISR.h
 * 
 * @param   *Callback_Handler Pointer to the function which will be called back.
 *          The function must accept a uint8_t as first argument.
 */
void SBC_Register_Callback(uint32_t ISR_Vector, void (*Callback_Handler)(uint8_t callbackHandler));


/**
 * @brief   Interrupt Service Routine for handling interrupts.
 * 
 *          This method must be called automatically everytime a rising-edge on the INTN pin is recognized.
 *          In case, the INTN pin is not connected, this method can also be called periodically by the user during runtime.
 *          The ISR will proceed a readout of all registered interrupts. If a status-bit of a registered interrupt is set,
 *          it will initiate a callback to the registered function and give the registered function the status-register value
 *          as a parameter.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_ISR(void);



/* --------------------------------  API Calls  ----------------------------------- */

/**
 * @brief   Enters SBC normal mode
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Mode_Normal(void);


/**
 * @brief   Enters SBC stop mode
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Mode_Stop(void);


/**
 * @brief   Clears all wake status registers and enter SBC sleep mode. Depending on configuration also the selective-wake feature will be initialized
 *          before entering sleep mode. In case, the SWK option is enabled and the internal CAN protocol handler is not in sync when calling this function,
 *          the sleep mode will be not entered.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Mode_Sleep(void);


/**
 * @brief   Sets the duty-cycle of the internal PWM generator
 * 
 * @param   PWM_DC Set the duty-cycle with values of 0-255 for 0% to 100%
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Set_DutyCycle_PWM(uint8_t PWM_DC);


/**
 * @brief   Sets the OnTime of the internal timer.
 * 
 * @param   OnTime	On time which will be configured to the timer. See TLE94x1_DEFINES.h for enumerations.
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Set_Timer_On(uint8_t OnTime);

/**
 * @brief   Sets the Period time of the internal timer
 * 
 * @param   Period  Period time which will be configured to the timer. See TLE94x1_DEFINES.h for enumerations.
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Set_Timer_Period(uint8_t Period);


/**
 * @brief   FO output activated by software for testing. Only working if FO/GPIO is configured as FO
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_FO_Test_On(void);


/**
 * @brief   FO output deactivated by software for testing. Only working if FO/GPIO is configured as FO
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_FO_Test_Off(void);


/**
 * @brief   Charge-Pump (VCP-Pin) is enabled. See chapter 5.3 in datasheet.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_CP_On(void);


/**
 * @brief   Charge-Pump (VCP-Pin) is disabled. See chapter 5.3 in datasheet.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_CP_Off(void);


/**
 * @brief   Enable Voltage Sensing. No wake-up events are generated. See chapter 9.2.4 in datasheet.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_WK_MEAS_On(void);


/**
 * @brief   Disable Voltage Sensing and enable wake-up functionality. See chapter 9.2.4 in datasheet.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_WK_MEAS_Off(void);


/**
 * @brief   WK is enabled as wake source. See chapter 5.2.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_TIMER_WK_EN_On(void);


/**
 * @brief   WK wake-up disabled. See chapter 5.2.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_TIMER_WK_EN_Off(void);


/**
 * @brief   Reads System Status Control, both lower and higher bits. See page 138 in datasheet.
 * 
 * @retval  Upper 8 bit are read from register SBC_SYS_STATUS_CTRL_1 and the lower 8 bit from SBC_SYS_STATUS_CTRL_0
 */
uint16_t SBC_SYS_STAT_Read(void);


/**
 * @brief   Writes System Status Control, both lower and higher bits. See page 138 in datasheet.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_SYS_STAT_Write(uint16_t SystemStatus);


/**
 * @brief   Locks CP_EN, GPIO configuration and 'rwl'-bits.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Lock_Configuration(void);


/**
 * @brief   Unlocks CP_EN, GPIO configuration and 'rwl'-bits.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Unlock_Configuration(void);


/**
 * @brief   Clears both Wake Status registers.
 * 
 * @retval  SBC_ErrorCode   See description of SBC_ErrorCode.
 */
SBC_ErrorCode SBC_Clear_Wake_Status(void);

#endif  /*TLE94x1_H*/
