/**
 ****************************************************************************************
 *
 * @file qpp_common.h
 *
 * @brief Header File - Quintic private profile common types.
 *
 * Copyright (C) Quintic 2013-2014
 *
 * $Rev: $
 *
 ****************************************************************************************
 */

#ifndef _QPP_COMMON_H_
#define _QPP_COMMON_H_

/**
 ****************************************************************************************
 * @addtogroup Quintic private Profile
 * @ingroup PROFILE
 * @brief Quintic private Profile
 *
 *****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#if (BLE_QPP_CLIENT || BLE_QPP_SERVER)

#include "prf_types.h"

/*
 * DEFINES
 ****************************************************************************************
 */

//#define QPP_SVC_PRIVATE_UUID        "\xFB\x34\x9B\x5F\x80\x00\x00\x80\x00\x10\x00\x00\xE9\xFE\x00\x00"

//<<<<<<<<<<<<<<<<tchip<<<<<<<<<<<<<<<<<<<<<
#define QPP_SVC_PRIVATE_UUID      0xcc07	//"\xcc\x07\x9B\x5F\x80\x00\x00\x80\x00\x10\x00\x00\xE9\xFE\x00\x00"		//0xcc07

enum
{
	QPP_CHAR_VAL_UUID 		    = 0xCD01,
	QPP_CHAR_INPUT_DATA_UUID    = 0xCD20,
    FIRMWARE_VERSION_DATA_UUID  = 0xCD30,
};
//>>>>>>>>>>>>>>>>tchip>>>>>>>>>>>>>>>>>>>>>

// Derek, used as max data length
#define QPP_DATA_MAX_LEN         (20)

// error code
#define QPPS_ERR_RX_DATA_NOT_SUPPORTED      (0x80)
#define QPPS_ERR_RX_DATA_EXCEED_MAX_LENGTH  (0x81)
#define QPPS_ERR_INVALID_PARAM              (0x82)

enum
{
    QPPS_VALUE_NTF_CFG  = 0x01,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

#endif /* #if (BLE_QPP_CLIENT || BLE_QPP_SERVER) */

/// @} qpp_common

#endif /* _QPP_COMMON_H_ */
