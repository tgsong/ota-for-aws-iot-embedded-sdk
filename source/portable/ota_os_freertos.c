/*
 * FreeRTOS OTA V1.2.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"

/* OTA OS POSIX Interface Includes.*/
#include "ota_os_freertos.h"

/* OTA Event queue attributes.*/
#define MAX_MESSAGES    10
#define MAX_MSG_SIZE    sizeof( OtaEventMsg_t )

/* Array containing pointer to the OTA event structures used to send events to the OTA task. */
static OtaEventMsg_t queueData[ MAX_MESSAGES ];

/* The queue control structure.  .*/
static StaticQueue_t staticQueue;

/* The queue control handle.  .*/
static QueueHandle_t otaEventQueue;

/* The timer handle.  .*/
static TimerHandle_t timer;

OtaErr_t OtaInitEvent_FreeRTOS( OtaEventContext_t * pEventCtx )
{
    ( void ) pContext;

    OtaErr_t otaErrRet = OTA_ERR_UNINITIALIZED;

    otaEventQueue = xQueueCreateStatic( ( UBaseType_t ) OTA_NUM_MSG_Q_ENTRIES,
                                        ( UBaseType_t ) MAX_MSG_SIZE,
                                        ( uint8_t * ) queueData,
                                        &staticQueue );

    if( otaEventQueue == NULL )
    {
        otaErrRet = OTA_ERR_EVENT_Q_CREATE_FAILED;

        LogError( ( "Failed to create OTA Event Queue: "
                    "xQueueCreateStatic returned error: "
                    "otaErrRet=%i ",
                    otaErrRet ) );
    }
    else
    {
        otaErrRet = OTA_ERR_NONE;

        LogDebug( ( "OTA Event Queue created." ) );
    }

    return otaErrRet;
}

OtaErr_t OtaSendEvent_FreeRTOS( OtaEventContext_t * pContext,
                                const void * pEventMsg,
                                unsigned int timeout )
{
    ( void ) pContext;
    ( void ) timeout;

    OtaErr_t otaErrRet = OTA_ERR_UNINITIALIZED;

    BaseType_t retVal = pdFALSE;

    /* Send the event to OTA event queue.*/
    retVal = xQueueSendToBack( otaEventQueue, pxEventMsg, ( TickType_t ) 0 );

    if( retVal == pdTRUE )
    {
        otaErrRet = OTA_ERR_NONE;

        LogDebug( ( "OTA Event Sent." ) );
    }
    else
    {
        otaErrRet = OTA_ERR_EVENT_Q_SEND_FAILED;

        LogError( ( "Failed to send event to OTA Event Queue: "
                    "xQueueSendToBack returned error: "
                    "otaErrRet=%i ",
                    otaErrRet ) );
    }

    return otaErrRet;
}

OtaErr_t OtaReceiveEvent_FreeRTOS( OtaEventContext_t * pContext,
                                   void * pEventMsg,
                                   uint32_t timeout )
{
    ( void ) pContext;

    OtaErr_t otaErrRet = OTA_ERR_UNINITIALIZED;

    BaseType_t retVal = pdFALSE;

    /* Temp buffer.*/
    char buff[ MAX_MSG_SIZE ];

    retVal = xQueueReceive( otaEventQueue, &buff, portMAX_DELAY );

    if( retVal == pdTRUE )
    {
        /* copy the data from local buffer.*/
        memcpy( pEventMsg, buff, MAX_MSG_SIZE );

        otaErrRet = OTA_ERR_NONE;

        LogDebug( ( "OTA Event received" ) );
    }
    else
    {
        otaErrRet = OTA_ERR_EVENT_Q_RECEIVE_FAILED;

        LogError( ( "Failed to receive event from OTA Event Queue: "
                    "xQueueReceive returned error: "
                    "otaErrRet=%i ",
                    otaErrRet ) );
    }

    return otaErrRet;
}

void OtaDeinitEvent_FreeRTOS( OtaEventContext_t * pContext )
{
    ( void ) pContext;

    /* Remove the event queue.*/
    if( otaEventQueue != NULL )
    {
        vQueueDelete( otaEventQueue );

        LogDebug( ( "OTA Event Queue Deleted." ) );
    }
}

static void timerCallback( TimerHandle_t timer )
{
    OtaEventMsg_t xEventMsg = { 0 };

    xEventMsg.eventId = OtaAgentEventRequestTimer;

    /* Send timer event. */
    OTA_SignalEvent( &xEventMsg );
}

OtaErr_t OtaStartTimer_FreeRTOS( OtaTimerContext_t * pTimerCtx,
                                 const char * const pTimerName,
                                 const uint32_t timeout,
                                 void ( * callback )( void * ) )
{
    ( void ) pTimerCtx;

    OtaErr_t otaErrRet = OTA_ERR_UNINITIALIZED;

    BaseType_t retVal = pdFALSE;

    /* If timer is not created.*/
    if( timer == NULL )
    {
        /* Create the timer. */
        timer = timerCreate( pTimerName,
                             pdMS_TO_TICKS( timeout ),
                             pdFALSE,
                             NULL,
                             timerCallback );

        if( timer == NULL )
        {
            otaErrRet = OTA_ERR_TIMER_CREATE_FAILED;

            LogError( ( "Failed to create OTA timer: "
                        "timerCreate returned NULL "
                        "otaErrRet=%i "
                        otaErrRet ) );
        }
        else
        {
            otaErrRet = OTA_ERR_NONE;

            LogDebug( ( "OTA Timer created." ) );

            /* Start the timer. */
            retVal = timerStart( timer, portMAX_DELAY );

            if( retVal == pdTRUE )
            {
                otaErrRet = OTA_ERR_NONE;

                LogDebug( ( "OTA Timer started." ) );
            }
            else
            {
                otaErrRet = OTA_ERR_TIMER_START_FAILED;

                LogError( ( "Failed to start OTA timer: "
                            "timerStart returned error." ) );
            }
        }
    }
    else
    {
        /* Reset the timer. */
        retVal = timerReset( timer, portMAX_DELAY );

        if( retVal == pdTRUE )
        {
            otaErrRet = OTA_ERR_NONE;

            LogDebug( ( "OTA Timer restarted." ) );
        }
        else
        {
            otaErrRet = OTA_ERR_TIMER_START_FAILED;

            LogError( ( "Failed to restart OTA timer: "
                        "timerReset returned error." ) );
        }
    }

    return otaErrRet;
}

OtaErr_t OtaStopTimer_FreeRTOS( OtaTimerContext_t * pTimerCtx )
{
    ( void ) pTimerCtx;

    OtaErr_t otaErrRet = OTA_ERR_UNINITIALIZED;

    if( timer != NULL )
    {
        /* Stop the timer. */
        retVal = timerStop( timer, portMAX_DELAY );

        if( retVal == pdTRUE )
        {
            otaErrRet = OTA_ERR_NONE;

            LogDebug( ( "OTA Timer restarted." ) );
        }
        else
        {
            otaErrRet = OTA_ERR_TIMER_STOP_FAILED;

            LogError( ( "Failed to stop OTA timer: "
                        "timerStop returned error." ) );
        }
    }
    else
    {
        LogError( ( "Failed to stop OTA timer: "
                    "Timer does not exist." ) );
    }

    return otaErrRet;
}

OtaErr_t ota_DeleteTimer( OtaTimerContext_t * pTimerCtx )
{
    ( void ) pTimerCtx;

    OtaErr_t otaErrRet = OTA_ERR_UNINITIALIZED;

    if( timer != NULL )
    {
        /* Stop the timer. */
        retVal = timerDelete( timer, portMAX_DELAY );

        if( retVal == pdTRUE )
        {
            otaErrRet = OTA_ERR_NONE;

            LogDebug( ( "OTA Timer deleted." ) );
        }
        else
        {
            otaErrRet = OTA_ERR_TIMER_DELETE_FAILED;

            LogError( ( "Failed to delete OTA timer: "
                        "timerDelete returned error." ) );
        }
    }
    else
    {
        otaErrRet = OTA_ERR_TIMER_DELETE_FAILED;

        LogError( ( "Failed to delete OTA timer: "
                    "Timer does not exist." ) );
    }

    return otaErrRet;
}

void * Malloc_FreeRTOS( size_t size )
{
    return vPortMalloc( size );
}

void Malloc_Free( void * ptr )
{
    vPortFree( ptr );
}
