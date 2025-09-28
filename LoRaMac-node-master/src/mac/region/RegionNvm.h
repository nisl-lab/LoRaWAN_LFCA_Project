/*!
 * \file      RegionNvm.h
 *
 * \brief     Region independent non-volatile data.
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author     Miguel Luis ( Semtech )
 *
 * \author     Daniel Jaeckle ( STACKFORCE )
 *
 * \addtogroup REGIONCOMMON
 *
 * \{
 */
#ifndef __REGIONNVM_H__
#define __REGIONNVM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "LoRaMacTypes.h"

#define REGION_NVM_MAX_NB_CHANNELS                 16
#define REGION_NVM_MAX_NB_BANDS                    1
#define REGION_NVM_CHANNELS_MASK_SIZE              1

/*!
 * Region specific data which must be stored in the NVM.
 */
typedef struct sRegionNvmDataGroup1
{
    uint32_t Crc32;
}RegionNvmDataGroup1_t;

/*!
 * Region specific data which must be stored in the NVM.
 * Parameters which do not change very frequently.
 */
typedef struct sRegionNvmDataGroup2
{
    /*!
     * LoRaMAC channels
     */
    ChannelParams_t Channels[ REGION_NVM_MAX_NB_CHANNELS ];
    /*!
     * LoRaMac channels mask
     */
    uint16_t ChannelsMask[ REGION_NVM_CHANNELS_MASK_SIZE ];
    /*!
     * LoRaMac channels default mask
     */
    uint16_t ChannelsDefaultMask[ REGION_NVM_CHANNELS_MASK_SIZE ];

    /*!
    * RSSI threshold for a free channel [dBm]
    */
    int16_t RssiFreeThreshold;

    /*!
    * Specifies the time the node performs a carrier sense
    */
    uint32_t CarrierSenseTime;


    /*!
     * CRC32 value of the Region data structure.
     */
    uint32_t Crc32;
}RegionNvmDataGroup2_t;

/*! \} addtogroup REGIONCOMMON */

#ifdef __cplusplus
}
#endif

#endif // __REGIONNVM_H__
