// usart.h
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#pragma once
#ifndef USART_H
#define USART_H
#include <stdbool.h>

void usart_init(void);
void usart_send(char c);
void usart_send_str (const char* str);
char usart_get(void);
bool usart_try_get(char *const holder);

#endif
