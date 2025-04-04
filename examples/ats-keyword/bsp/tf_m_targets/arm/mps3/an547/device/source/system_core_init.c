/*
 * Copyright (c) 2009-2021 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is derivative of CMSIS V5.6.0 system_ARMv81MML.c
 * Git SHA: b5f0603d6a584d1724d952fd8b0737458b90d62b
 */

#include "cmsis.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
 #define  XTAL             (32000000UL)
 #define  SYSTEM_CLOCK     (XTAL)
 #define  PERIPHERAL_CLOCK (25000000UL)

/*----------------------------------------------------------------------------
  Externals
 *----------------------------------------------------------------------------*/
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    extern uint32_t __VECTOR_TABLE;
#endif

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;
uint32_t PeripheralClock = PERIPHERAL_CLOCK;

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
    SystemCoreClock = SYSTEM_CLOCK;
    PeripheralClock = PERIPHERAL_CLOCK;
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{

#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
  SCB->VTOR = (uint32_t)(&__VECTOR_TABLE);
#endif

#if (defined (__FPU_USED) && (__FPU_USED == 1U)) || \
    (defined (__ARM_FEATURE_MVE) && (__ARM_FEATURE_MVE == 1U))
#if (TFM_SYSTEM_FP >= 1) || (TFM_SYSTEM_FP_NS >= 1)
    SCB->CPACR |= ((3U << 10U*2U) |           /* enable CP10 Full Access */
                   (3U << 11U*2U)  );         /* enable CP11 Full Access */

#if defined(TFM_LAZY_STACK_NS) || defined(TFM_LAZY_STACK)
  /* Enable Lazy FP */
  FPU->FPCCR |= FPU_FPCCR_LSPEN_Msk;
#else
  /* Disable Lazy FP */
  FPU->FPCCR &= ~FPU_FPCCR_LSPEN_Msk;
#endif /* TFM_LAZY_FP_NS */

#endif /* TFM_SYSTEM_FP >= 1 || (TFM_SYSTEM_FP_NS >= 1) */
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif
}
