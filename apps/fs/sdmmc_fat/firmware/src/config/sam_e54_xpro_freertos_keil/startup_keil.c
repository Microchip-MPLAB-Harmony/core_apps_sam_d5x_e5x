// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#include "definitions.h" /* for potential custom handler names */
#include <stdbool.h>

/* Initialize segments */
extern const DeviceVectors __Vectors;

/* C library initialization routine */
extern void __main(void);

/* Declaration of Reset handler (may be custom) */
void Reset_Handler(void);

/* Device Vector information is available in interrupt.c file */

__STATIC_INLINE void FPU_Enable(void);

#if (__FPU_USED == 1)

/* Enable FPU */
__STATIC_INLINE void FPU_Enable(void)
{
    uint32_t prim;
    prim = __get_PRIMASK();
    __disable_irq();

     SCB->CPACR |= (0xFu << 20);
    __DSB();
    __ISB();

    if (!prim)
    {
        __enable_irq();
    }
}
#endif /* (__FPU_USED == 1) */
__STATIC_INLINE void TCM_Disable(void);
__STATIC_INLINE void TCM_Enable(void);
__STATIC_INLINE void TCM_Configure(uint32_t tcmSize);
__STATIC_INLINE void ICache_Enable(void);
__STATIC_INLINE void DCache_Enable(void);

/** Program CMCC CSIZESW bits for TCM and cache configuration */
__STATIC_INLINE void TCM_Configure(uint32_t tcmSize)
{
        CMCC_REGS->CMCC_CFG = CMCC_CFG_CSIZESW(tcmSize);
}

/** Enable TCM memory */
__STATIC_INLINE void  TCM_Enable(void)
{
    /* TCM cannot be enabled or disabled in SAME5x/SAMD5x family*/
}

/* Disable TCM memory */
__STATIC_INLINE void  TCM_Disable(void)
{
    /* TCM cannot be enabled or disabled in SAME5x/SAMD5x family*/
}

__STATIC_INLINE void ICache_Enable(void)
{
    CMCC_REGS->CMCC_CTRL &= ~(CMCC_CTRL_CEN_Msk);
    while((CMCC_REGS->CMCC_SR & CMCC_SR_CSTS_Msk) == CMCC_SR_CSTS_Msk)
    {
        /*Wait for the operation to complete*/
    }
    CMCC_REGS->CMCC_CFG |= (CMCC_CFG_DCDIS_Msk);
    CMCC_REGS->CMCC_CTRL = (CMCC_CTRL_CEN_Msk);
}

__STATIC_INLINE void DCache_Enable(void)
{
}
/* Optional application-provided functions */
extern void __attribute__((weak)) _on_reset(void);
extern void __attribute__((weak)) _on_bootstrap(void);

/**
 * \brief This is the code that gets called on processor reset.
 * To initialize the device, and call the main() routine.
 */
void __attribute__((section(".text.Reset_Handler"))) Reset_Handler(void)
{
    uint32_t *pSrc;

    /* Call the optional application-provided _on_reset() function. */
    if (_on_reset)
    {
        _on_reset();
    }

#if (__FPU_USED == 1)
    /* Enable the FPU if the application is built with --fpu options */
    FPU_Enable();
#endif
	TCM_Configure(2);
    /* Enable TCM   */
    TCM_Enable();

#  ifdef SCB_VTOR_TBLOFF_Msk
    /*  Set the vector-table base address in FLASH */
    pSrc = (uint32_t *) &__Vectors;
    SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);
#  endif /* SCB_VTOR_TBLOFF_Msk */


    /* Enable Instruction Cache */
    ICache_Enable();


    /* Call the optional application-provided _on_bootstrap() function. */
    if (_on_bootstrap)
    {
        _on_bootstrap();
    }

    /* Execute entry point to the C library initialization routine,
       which eventually executes application's main function */
    __main();

    /* Infinite loop */
    while (1) {}
}
