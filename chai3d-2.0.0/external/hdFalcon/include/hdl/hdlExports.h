/*
** Copyright 2005 Novint Technologies, Inc. All rights reserved.
** Available only under license from Novint Technologies, Inc.
**
** hdlExports.h
**
** Haptic Device Application Layer API
** Exports
*/
#ifndef HDL_EXPORTS_H
#define HDL_EXPORTS_H

#ifdef WIN32
#ifdef HDL_EXPORTS
#define HDLAPI __declspec(dllexport)
#else
#define HDLAPI __declspec(dllimport)
#endif

#define HDLAPIENTRY  __stdcall

#endif /* WIN32 */
#endif /* HDL_EXPORTS_H */
