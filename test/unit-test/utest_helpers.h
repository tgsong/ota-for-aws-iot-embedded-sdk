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

#ifndef _UTEST_HELPERS_
#define _UTEST_HELPERS_

#include <cbor.h>

#define CBOR_TEST_GETSTREAMRESPONSE_MESSAGE_ITEM_COUNT    4
#define CBOR_TEST_CLIENTTOKEN_VALUE                       "ThisIsAClientToken"
#define CBOR_TEST_FILEIDENTITY_VALUE                      0

CborError createOtaStreammingMessage( uint8_t * pMessageBuffer,
                                      size_t messageBufferSize,
                                      int blockIndex,
                                      uint8_t * pBlockPayload,
                                      size_t blockPayloadSize,
                                      size_t * pEncodedSize );

#endif /* ifndef _UTEST_HELPERS_ */
