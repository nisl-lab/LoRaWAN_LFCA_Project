/*!
 * \file      Region.c
 *
 * \brief     Region implementation.
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *        ______                  _
 *           / _____)         _          | |
 *          ( (____  _____ ____ _| |_ _____  ____| |__
 *           \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *           _____) ) ____| | | || |_| ____( (___| | | |
 *          (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *          (C)2013-2017 Semtech
 *
 *           ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *          / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *          \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *          |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *          embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 */
#include "LoRaMac.h"

// Setup regions
#include "RegionKR920.h"
#define KR920_IS_ACTIVE( )                 return true;
#define KR920_GET_PHY_PARAM( )                 return RegionKR920GetPhyParam( getPhy );
#define KR920_SET_BAND_TX_DONE( )              RegionKR920SetBandTxDone( txDone );
#define KR920_INIT_DEFAULTS( )                 RegionKR920InitDefaults( params );
#define KR920_VERIFY( )                    return RegionKR920Verify( verify, phyAttribute );
#define KR920_APPLY_CF_LIST( )                 RegionKR920ApplyCFList( applyCFList );
#define KR920_CHAN_MASK_SET( )                 return RegionKR920ChanMaskSet( chanMaskSet );
#define KR920_COMPUTE_RX_WINDOW_PARAMETERS( )          RegionKR920ComputeRxWindowParameters( datarate, minRxSymbols, rxError, rxConfigParams );
#define KR920_RX_CONFIG( )                 return RegionKR920RxConfig( rxConfig, datarate );
#define KR920_TX_CONFIG( )                 return RegionKR920TxConfig( txConfig, txPower, txTimeOnAir );
#define KR920_LINK_ADR_REQ( )                  return RegionKR920LinkAdrReq( linkAdrReq, drOut, txPowOut, nbRepOut, nbBytesParsed );
#define KR920_RX_PARAM_SETUP_REQ( )            return RegionKR920RxParamSetupReq( rxParamSetupReq );
#define KR920_NEW_CHANNEL_REQ( )               return RegionKR920NewChannelReq( newChannelReq );
#define KR920_TX_PARAM_SETUP_REQ( )            return RegionKR920TxParamSetupReq( txParamSetupReq );
#define KR920_DL_CHANNEL_REQ( )                return RegionKR920DlChannelReq( dlChannelReq );
#define KR920_ALTERNATE_DR( )                  return RegionKR920AlternateDr( currentDr, type );
#define KR920_NEXT_CHANNEL( )                  return RegionKR920NextChannel( nextChanParams, channel, time, aggregatedTimeOff );
#define KR920_CHANNEL_ADD( )                   return RegionKR920ChannelAdd( channelAdd );
#define KR920_CHANNEL_REMOVE( )                return RegionKR920ChannelsRemove( channelRemove );
#define KR920_APPLY_DR_OFFSET( )               return RegionKR920ApplyDrOffset( downlinkDwellTime, dr, drOffset );
#define KR920_RX_BEACON_SETUP( )               RegionKR920RxBeaconSetup( rxBeaconSetup, outDr );

bool RegionIsActive( LoRaMacRegion_t region )
{
    KR920_IS_ACTIVE( );
    
}

PhyParam_t RegionGetPhyParam( LoRaMacRegion_t region, GetPhyParams_t* getPhy )
{
    PhyParam_t phyParam = { 0 };
    KR920_GET_PHY_PARAM( );
}

void RegionSetBandTxDone( LoRaMacRegion_t region, SetBandTxDoneParams_t* txDone )
{
    KR920_SET_BAND_TX_DONE( );
}

void RegionInitDefaults( LoRaMacRegion_t region, InitDefaultsParams_t* params )
{
    KR920_INIT_DEFAULTS( );
}

bool RegionVerify( LoRaMacRegion_t region, VerifyParams_t* verify, PhyAttribute_t phyAttribute )
{
    KR920_VERIFY( );
}

void RegionApplyCFList( LoRaMacRegion_t region, ApplyCFListParams_t* applyCFList )
{
    KR920_APPLY_CF_LIST( );
}

bool RegionChanMaskSet( LoRaMacRegion_t region, ChanMaskSetParams_t* chanMaskSet )
{
    KR920_CHAN_MASK_SET( );
}

void RegionComputeRxWindowParameters( LoRaMacRegion_t region, int8_t datarate, uint8_t minRxSymbols, uint32_t rxError, RxConfigParams_t *rxConfigParams )
{
    KR920_COMPUTE_RX_WINDOW_PARAMETERS( );
}

bool RegionRxConfig( LoRaMacRegion_t region, RxConfigParams_t* rxConfig, int8_t* datarate )
{
    KR920_RX_CONFIG( );
    
}

bool RegionTxConfig( LoRaMacRegion_t region, TxConfigParams_t* txConfig, int8_t* txPower, TimerTime_t* txTimeOnAir )
{
    KR920_TX_CONFIG( );
}

uint8_t RegionLinkAdrReq( LoRaMacRegion_t region, LinkAdrReqParams_t* linkAdrReq, int8_t* drOut, int8_t* txPowOut, uint8_t* nbRepOut, uint8_t* nbBytesParsed )
{
    KR920_LINK_ADR_REQ( );
}

uint8_t RegionRxParamSetupReq( LoRaMacRegion_t region, RxParamSetupReqParams_t* rxParamSetupReq )
{
    KR920_RX_PARAM_SETUP_REQ( );
}

int8_t RegionNewChannelReq( LoRaMacRegion_t region, NewChannelReqParams_t* newChannelReq )
{
    KR920_NEW_CHANNEL_REQ( );
}

int8_t RegionTxParamSetupReq( LoRaMacRegion_t region, TxParamSetupReqParams_t* txParamSetupReq )
{
    KR920_TX_PARAM_SETUP_REQ( );
}

int8_t RegionDlChannelReq( LoRaMacRegion_t region, DlChannelReqParams_t* dlChannelReq )
{
    KR920_DL_CHANNEL_REQ( );
}

int8_t RegionAlternateDr( LoRaMacRegion_t region, int8_t currentDr, AlternateDrType_t type )
{
    KR920_ALTERNATE_DR( );
}

LoRaMacStatus_t RegionNextChannel( LoRaMacRegion_t region, NextChanParams_t* nextChanParams, uint8_t* channel, TimerTime_t* time, TimerTime_t* aggregatedTimeOff )
{
    KR920_NEXT_CHANNEL( );
}

LoRaMacStatus_t RegionChannelAdd( LoRaMacRegion_t region, ChannelAddParams_t* channelAdd )
{
    KR920_CHANNEL_ADD( );
}

bool RegionChannelsRemove( LoRaMacRegion_t region, ChannelRemoveParams_t* channelRemove )
{
    KR920_CHANNEL_REMOVE( );
}

uint8_t RegionApplyDrOffset( LoRaMacRegion_t region, uint8_t downlinkDwellTime, int8_t dr, int8_t drOffset )
{
    KR920_APPLY_DR_OFFSET( );
}

void RegionRxBeaconSetup( LoRaMacRegion_t region, RxBeaconSetup_t* rxBeaconSetup, uint8_t* outDr )
{
    KR920_RX_BEACON_SETUP( );
}

Version_t RegionGetVersion( void )
{
    Version_t version;

    version.Value = REGION_VERSION;

    return version;
}

