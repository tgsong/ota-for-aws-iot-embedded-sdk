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

#ifndef __OTA_HTTP__H__
#define __OTA_HTTP__H__

/* OTA includes. */
#include "ota.h"
#include "ota_private.h"


/**
 * @brief Initialize file transfer over HTTP.
 *
 * This function initializes the file transfer after the OTA job is parsed and accepted
 * by initializing the http component with pre-signed url.
 *
 * @param[in] pAgentCtx The OTA agent context.
 *
 * @return The OTA error code. See OTA Agent error codes information in ota.h.
 */
OtaErr_t initFileTransfer_Http( OtaAgentContext_t * pAgentCtx );


/**
 * @brief Request File block over HTTP.
 *
 * This function is used for requesting a file block over HTTP using the
 * file context.
 *
 * @param[in] pAgentCtx The OTA agent context.
 *
 * @return The OTA PAL layer error code combined with the MCU specific error code. See OTA Agent
 * error codes information in ota.h.
 */
OtaErr_t requestDataBlock_Http( OtaAgentContext_t * pAgentCtx );


/**
 * @brief Stub for decoding the file block.
 *
 * File block received over HTTP does not require decoding, only increment the number
 * of blocks received.
 *
 * @param[in] pMessageBuffer The message to be decoded.
 * @param[in] messageSize     The size of the message in bytes.
 * @param[out] pFileId        The server file ID.
 * @param[out] pBlockId       The file block ID.
 * @param[out] pBlockSize     The file block size.
 * @param[out] pPayload     The payload.
 * @param[out] pPayloadSize   The payload size.
 *
 * @return The OTA PAL layer error code combined with the MCU specific error code. See OTA Agent
 * error codes information in ota.h.
 */
OtaErr_t decodeFileBlock_Http( uint8_t * pMessageBuffer,
                               size_t messageSize,
                               int32_t * pFileId,
                               int32_t * pBlockId,
                               int32_t * pBlockSize,
                               uint8_t ** pPayload,
                               size_t * pPayloadSize );

/**
 * @brief Cleanup related to OTA data plane over HTTP.
 *
 * This function performs cleanup by resetting the number of blocks received
 * and deinit the http component.
 *
 * @param[in] pAgentCtx The OTA agent context.
 *
 * @return The OTA error code. See OTA Agent error codes information in ota.h.
 */

OtaErr_t cleanupData_Http( OtaAgentContext_t * pAgentCtx );

#endif /* ifndef __OTA_HTTP__H__ */