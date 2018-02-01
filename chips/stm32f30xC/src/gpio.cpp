/*
 * gpio.cpp - Class which abstracts GPIO functionality on the STM32
 * Copyright (c) 2016 James Jackson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "gpio.h"

void GPIO::init(GPIO_TypeDef* BasePort, uint16_t pin, gpio_mode_t mode)
{
  pin_ = pin;
  port_ = BasePort;
  set_mode(mode);
}

void GPIO::write(gpio_write_t state)
{
  if (mode_ == GPIO::OUTPUT)
  {
    if (state == GPIO::LOW)
      GPIO_ResetBits(port_, pin_);
    else
      GPIO_SetBits(port_, pin_);
  }
}

void GPIO::toggle()
{
  if (mode_ == GPIO::OUTPUT)
  {
    if (GPIO_ReadOutputDataBit(port_, pin_))
      GPIO_ResetBits(port_, pin_);
    else
      GPIO_SetBits(port_, pin_);
  }
}

bool GPIO::read()
{
  // If it's an input pin, use the read input data
  if(mode_ == GPIO::INPUT)
    return port_->IDR & pin_;
  else
    return port_->ODR & pin_;
}

void GPIO::set_mode(gpio_mode_t mode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = pin_;

  switch (mode)
  {
  case INPUT:
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    break;
  case OUTPUT:
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    break;
  case PERIPH_OUT:
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    break;
  case PERIPH_IN:
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    break;
  case PERIPH_IN_OUT:
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    break;
  case ANALOG:
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    break;
  }

  // Who cares about power usage?  Go as fast as possible.
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  // Initialize the GPIO
  GPIO_Init(port_, &GPIO_InitStruct);
  mode_ = mode;
  write(GPIO::LOW);
}
