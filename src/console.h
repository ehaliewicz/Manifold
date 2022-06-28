#ifndef CONSOLE_H
#define CONSOLE_H

#include <genesis.h>

uint16_t console_init(uint16_t start_addr);
void console_tick();
void console_cleanup();
void console_push_message(char* msg, int len, uint16_t ticks);
void console_push_message_high_priority(char* msg, int len, uint16_t ticks);

#endif