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

#ifndef DEEP_LEARNING_TENSORS_H_
#define DEEP_LEARNING_TENSORS_H_

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <list>
#include <cudnn.h>
#include "../common/timing.h"
#include "../common/workload_params.h"


using namespace std;

namespace dl_infra {
    namespace common {
        class Tensor {
            
            enum LocationType {
                HOST,
                DEVICE
            };

            string tensor_id_;
            int access_count_;
            int timestamp_;
            int status_;
            LocationType location;

            //for recomputation
            // vector<Tensor*> input_tensors_;    
            // string operation_name_;   //the op that produced this tensor
            float* tensor_data_;
        };

        class TensorBag {
            private:
                Timer* timer_;
            public:
            cudnnTensorDescriptor_t      cudnn_input_desc_;
            cudnnFilterDescriptor_t      cudnn_filter_desc_;
            cudnnTensorDescriptor_t      cudnn_output_desc_;

            float* ooc_input_host_ptr_   = NULL;
            float* ooc_filter_host_ptr_  = NULL;
            float* ooc_output_host_ptr_  = NULL;

            float* input_dev_ptr_   = NULL;
            float* filter_dev_ptr_  = NULL;
            float* output_dev_ptr_  = NULL;
                
            void *workSpace_;
            size_t workSpaceSize_;

            public:
                TensorBag(Timer* timer);
                ~TensorBag();
                
        };


        class TensorMgr {
            private:
                Timer* timer_, *dataFileReadTimer_;
                int no_of_layers_;
                std::vector<TensorBag*> tensorBags;
                WorkloadParams* workloadParams_;
                bool dryRun_ = false;
            public:
                TensorMgr(WorkloadParams* workloadParams, Timer* timer, Timer* dataFileReadTimer, int no_of_layers);
                TensorBag* setupTensorsForConvLayer(int conv_layer_index, int* input_tensor_dims_, int* filter_tensor_dims_, int* output_tensor_dims_, int input_stride_dims_[], int output_stride_dims_[]);
                TensorBag* createAndSetupTensorBag(int conv_layer_index, int* input_tensor_dims_, int* filter_tensor_dims_, int* output_tensor_dims_, int input_stride_dims_[], int output_stride_dims_[]);
                TensorBag* getTensorBagAt(int index) { 
                    return tensorBags.at(index);}
                float* readMnistDataFiles2(int no_of_images);
                void createTensorDescriptors(int conv_layer_index, int* input_tensor_dims_, int* filter_tensor_dims_, int* output_tensor_dims_, 
                    int input_stride_dims_[], int output_stride_dims_[]);
                void createIOTensors(int conv_layer_index, int* input_tensor_dims_, int* output_tensor_dims_);
                void createWeightsTensor(int conv_layer_index, int* filter_tensor_dims_);
                void setDryRun(bool dryRun) { dryRun_ = dryRun; }

                ~TensorMgr();

            
        };
    };
};


#endif
