/* Copyright (c) 2006-2012 Filip Wasilewski <http://en.ig.ma/> 
 * Copyright (c) 2012-2016 The PyWavelets Developers
 *                         <https://github.com/PyWavelets/pywt>
 * See COPYING for license details.
 */

/* Common constants, typedefs and functions */

#pragma once

#include <stdlib.h>
#include <memory.h>

/* ##### Typedefs ##### */

#ifdef PY_EXTENSION
    /* another declaration is in .c file generated by Pyrex */
    #ifndef PY_SSIZE_T_CLEAN
        #define PY_SSIZE_T_CLEAN
        #include "Python.h"
        #include "structmember.h"
        #ifndef PY_LONG_LONG
            #define PY_LONG_LONG LONG_LONG
        #endif
    #else
        #include "Python.h"
    #endif

    /* on Solaris/SmartOS system, index_t is used in sys/types.h, so use pytw_index_t */
    typedef Py_ssize_t pywt_index_t;

    /* using Python's memory manager */
    #define wtmalloc(size)      PyMem_Malloc(size)
    #define wtfree(ptr)         PyMem_Free(ptr)
    void *wtcalloc(size_t, size_t);
#else
    typedef int pywt_index_t;
    /* standard c memory management */
    #define wtmalloc(size)      malloc(size)
    #define wtfree(ptr)         free(ptr)
    #define wtcalloc(len, size) calloc(len, size)
#endif

#ifdef _MSC_VER
    #include <intrin.h>
#endif

typedef struct {
    size_t * shape;
    pywt_index_t * strides;
    size_t ndim;
} ArrayInfo;

typedef enum {
    COEF_APPROX = 0,
    COEF_DETAIL = 1,
} Coefficient;

/* Signal extension modes */
typedef enum {
       MODE_INVALID = -1,
       MODE_ZEROPAD = 0,   /* default, signal extended with zeros */
       MODE_SYMMETRIC,     /* signal extended symmetrically (mirror)
                            * For extensions greater than signal length,
                            * mirror back and forth:
                            * 2 3 3 2 1 | 1 2 3 | 3 2 1 1 2
                            */
       MODE_CONSTANT_EDGE, /* signal extended with the border value */
       MODE_SMOOTH,        /* linear extrapolation (first derivative) */
       MODE_PERIODIC,      /* signal is treated as being periodic */
       MODE_PERIODIZATION, /* signal is treated as being periodic, minimal output length */
       MODE_REFLECT,       /* signal extended symmetrically (reflect)
                            * For extensions greater than signal length,
                            * reflect back and forth without repeating edge values:
                            * 1 2 3 2 | 1 2 3 | 2 1 2 3
                            */
       MODE_MAX,
} MODE;


/* ##### Calculating buffer lengths for various operations ##### */

/*
 * Length of DWT coeffs for specified input data length, filter length and
 * signal extension mode.
 */
size_t dwt_buffer_length(size_t input_len, size_t filter_len, MODE mode);

/*
 * Length of reconstructed signal for specified input coeffs length and filter
 * length. It is used for direct reconstruction from coefficients (normal
 * convolution of upsampled coeffs with filter).
 */
size_t reconstruction_buffer_length(size_t coeffs_len, size_t filter_len);

/*
 * Length of IDWT reconstructed signal for specified input coeffs length, filter
 * length and extension mode.
 */
size_t idwt_buffer_length(size_t coeffs_len, size_t filter_len, MODE mode);

/* Length of SWT coefficients for specified input signal length (== input_len) */
size_t swt_buffer_length(size_t input_len);

/* Maximum useful level of DWT decomposition. */
unsigned char dwt_max_level(size_t input_len, size_t filter_len);

/* Maximum useful level of SWT decomposition. */
unsigned char swt_max_level(size_t input_len);
