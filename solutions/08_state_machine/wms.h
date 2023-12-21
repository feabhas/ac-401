// wms.h
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#pragma once
#ifndef WMS_H
#define WMS_H
#include <stdbool.h>

typedef enum wms_wash_type {
    WASH_NONE, WASH_WHITE, WASH_COLOR, WASH_MIXED, WASH_ECONOMY,
    WASH_USER1, WASH_USER2, WASH_TYPE_SIZE
} WmsWashType;

extern void wms_init(void);
extern void wms_select_wash_type(WmsWashType);
extern bool wms_next_wash_step(void);

#endif
