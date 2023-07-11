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

#include "vit_transformer.h"
namespace dl_cifar::common {

    VitTransformer::VitTransformer(LangHandle *langHandle, Timer* timer, int noOfEncoders, int batchSize, 
    int embSize, int embByNoOfHeadsSize, int noOfEmbs, int noOfHeads, float *d_patchEmbs, float *d_d_patchEmbs)
        : noOfEncoders_(noOfEncoders) {

        Tracer::func_begin("VitTransformer::VitTransformer");    
        

        for(int i=0; i<noOfEncoders_; i++) {  
            encoders_.push_back(new VitTransformerEncoder(langHandle, timer, batchSize, embSize, embByNoOfHeadsSize, noOfEmbs, 
                                                            noOfHeads, d_patchEmbs, d_d_patchEmbs));
        }

        Tracer::func_end("VitTransformer::VitTransformer");    
    }

    void VitTransformer::doFw() {
        Tracer::func_begin("VitTransformer::doFw");    
        for(int i=0; i<noOfEncoders_; i++) {  
            encoders_[i]->doFw();
        }

        Tracer::func_end("VitTransformer::doFw");    
    }

    void VitTransformer::doBw() {
        Tracer::func_begin("VitTransformer::doBw");    
        for(int i=0; i<noOfEncoders_; i++) {  
            encoders_[i]->doBw();
        }

        Tracer::func_end("VitTransformer::doBw");    
    }

    VitTransformer::~VitTransformer() {
        Tracer::func_begin("VitTransformer::~VitTransformer");    
        
        for(int i=0; i<noOfEncoders_; i++) {  
            delete encoders_[i];
        }
        Tracer::func_end("VitTransformer::~VitTransformer");    
    }
};