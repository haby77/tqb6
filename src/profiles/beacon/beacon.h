/**
 ****************************************************************************************
 *
 * @file beacon.h
 *
 * @brief BEACON Server Implementation.
 *
 * Copyright (C) RivieraWaves 2014
 *
 * Author : wbj@t-chip.com.cn
 *
 * Date : 2014-09-02
 *
 ****************************************************************************************
 */
 
#ifndef __BEACON_H__
#define __BEACON_H__


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#if (BLE_BEACON_SERVER)

#include "prf_types.h"
#include "attm.h"
#include "atts.h"
#include "atts_db.h"

#define BEACON_CFG_FLAG_MANDATORY_MASK       (0x07)

enum
{
    BEACON_SERVICE_UUID = 0xfff0,
    BEACON_DATA_UUID ,
};

/// Beacon Service Attributes Indexes
enum
{
    BEACON_IDX_SVC,

    BEACON_IDX_LVL_CHAR,
    BEACON_IDX_LVL_VAL,
    BEACON_VERSION_DATA_USER_DESP,

    BEACON_IDX_NB,
};

/// iBeacon 'Profile' Server environment variable
struct beacon_env_tag
{
    /// Connection Info
    struct prf_con_info con_info;
    /// Start Handles
    uint16_t shdl;
    /// Database features
    uint8_t features;
    ///Connection handle
    uint16_t conhdl;
};


/// Full beacon Database Description
extern const struct atts_desc beacon_att_db[BEACON_IDX_NB];

///Beacon Service
extern const atts_svc_desc_t beacon_svc;

extern const struct atts_char_desc beacon_level_char;
/// BEACON 'Profile' Server Environment
extern struct beacon_env_tag beacon_env;


extern void beacon_init(void);
extern void beacon_disable(void);

#endif //BLE_BEACON_SERVER
#endif
