/** @file hdlErrors.h
** Copyright 2005-2007 Novint Technologies, Inc. All rights reserved.
** Available only under license from Novint Technologies, Inc.
**
**/

/**
** HDAL API Errors
**
** Client application queries HDAL errors using hdlGetError().
** hdlGetError() returns an error type.
**/

#ifndef HDLERRORS_H
#define HDLERRORS_H

typedef int HDLError;

/* Possible future enhancements
#define HDL_MAJOR_ERROR_MASK 0x0000ffff
#define HDL_MINOR_ERROR_MASK 0xffff0000
 */

#define HDL_NO_ERROR 0x0                  /**< No errors on error stack */

#define HDL_ERROR_STACK_OVERFLOW 0x01     /**< Overflow of error stack */
#define HDL_ERROR_INIT_FAILED    0x10     /**< Device initialization error */

#if 0 // in progress
#define HDL_INIT_ERROR_MASK                               0x1F
#define HDL_INIT_INI_NOT_FOUND                            0x11
#define HDL_INIT_INI_DLL_STRING_NOT_FOUND                 0x12
#define HDL_INIT_INI_MANUFACTURER_NAME_STRING_NOT_FOUND   0x13
#define HDL_INIT_DLL_LOAD_ERROR                           0x14
#define HDL_INIT_DEVICE_FAILURE                           0x15
#define HDL_INIT_DEVICE_ALREADY_INITED                    0x16
#endif

#endif // HDLERRORS_H






