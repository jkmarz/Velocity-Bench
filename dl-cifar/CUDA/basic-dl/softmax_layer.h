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

#ifndef DL_CIFAR_SOFTMAX_LAYER_H_
#define DL_CIFAR_SOFTMAX_LAYER_H_

#include <cudnn.h>
//#include "utils.h"
#include "timing.h"
#include "tracing.h"
#include "handle.h"
#include "error_handling.h"

using namespace dl_cifar::common;

class SoftmaxLayer {
    private:
        Timer* timer_;
        LangHandle *langHandle_;

        cudnnTensorDescriptor_t  softMaxInputDesc_, softMaxOutputDesc_;
        int *softmaxInputTensorDims_, *softmaxOutputTensorDims_;
        int softmaxInputStrideDims_[4], softmaxOutputStrideDims_[4];
        
        float alpha = 1.0f; 
        float beta  = 0.0f; 

        float *d_input_, *d_d_input_, *d_output_, *d_d_output_;

    public: 
        SoftmaxLayer(LangHandle *langHandle, Timer* timer,
                        int softmaxInputTensorDims[4], int softmaxOutputTensorDims[4],
                        float *d_input, float *d_d_input, float *d_output, float *d_d_output);

        void doFw();
        void doBw();

        ~SoftmaxLayer();

    
};


#endif