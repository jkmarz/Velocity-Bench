/* Copyright (C) 2023 Intel Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
 * OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef DL_CIFAR_CUDNN_ERROR_HANDLING_LAYER_HIP_H_
#define DL_CIFAR_CUDNN_ERROR_HANDLING_LAYER_HIP_H_

#include <stdio.h>
#include <iostream>
#include <exception>
#include "hip/hip_runtime.h"
#include <miopen/miopen.h>
#include <rocblas.h>


#define assertDevApiInvar(...)                                                                                    \
    if(int ret_val = __VA_ARGS__) {                                                                            \
        std::cout << "\nHIP API call failed:" << std::endl;                                                    \
        std::cout << "\tFuction call:   " << #__VA_ARGS__  << std::endl;                                       \
        std::cout << "\tError code:     " << ret_val << std::endl;                                             \
        std::cout << "\tError msg:      " << hipGetErrorString((hipError_t)ret_val) << std::endl;              \
        exit(1);                                                                                               \
    }       

#define assertDlApiInvar(...)                                                                                 \
    if(int ret_val = __VA_ARGS__) {                                                                            \
        std::cout << "\nMIOpen API call failed:" << std::endl;                                                 \
        std::cout << "\tFuction call:   " << #__VA_ARGS__  << std::endl;                                       \
        std::cout << "   Location:       " << __FILE__ << ":" << __LINE__ << std::endl;                        \
        std::cout << "\tError code:     " << ret_val << std::endl;                                             \
        std::cout << "\tError msg:      " << miopenGetErrorString((miopenStatus_t)ret_val) << std::endl;       \
        exit(2);                                                                                               \
    }   

#define assertBlasInvar(...)                                                                                \
    if(int ret_val = __VA_ARGS__) {                                                                            \
        std::cout << "\nROCBLAS API call failed:" << std::endl;                                                \
        std::cout << "\tFuction call:      " << #__VA_ARGS__  << std::endl;                                    \
        std::cout << "   Location:       " << __FILE__ << ":" << __LINE__ << std::endl;                        \
        std::cout << "\tError code:     " << ret_val << std::endl;                                             \
        std::cout << "\tError msg:      " << rocblas_status_to_string((rocblas_status_)ret_val) << std::endl;  \
        exit(3);                                                                                               \
    }      


#endif