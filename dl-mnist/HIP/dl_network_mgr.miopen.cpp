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

#include "dl_network_mgr.miopen.h"

#include <miopen.h>
#include "conv_layer.miopen.h"
#include "../common/timing.h"
#include "../common/tracer.h"
#include "../common/workload_params.h"
#include <vector>
#include <map>

using namespace dl_infra::common;
using Time = std::chrono::time_point<std::chrono::high_resolution_clock>;

namespace dl_infra {
    namespace miopen {
        std::vector<ConvLayer*> DlNetworkMgr::createAllLayers(string networkName, int no_of_conv_layers, int *conv_dims, TensorMgr* tensoMgr) {
            Tracer::func_begin("DlNetworkMgr::createAllLayers");  

            std::vector<ConvLayer*> convLayers(no_of_conv_layers);
            auto conv_dims_arr = (int(*)[no_of_params_][no_of_tensor_dims_]) (conv_dims);

            for(int i=0; i<no_of_conv_layers; i++) {
                convLayers[i] = new ConvLayer(workloadParams_, i, no_of_conv_layers, timer_, tensoMgr, handle_,  
                                conv_dims_arr[i][0], conv_dims_arr[i][1], conv_dims_arr[i][2]); 
            }

            Tracer::func_end("DlNetworkMgr::createAllLayers");  
            return convLayers;
        }

        void DlNetworkMgr::initializeNetwork(string networkName) {
            Tracer::func_begin("DlNetworkMgr::initializeNetwork");  

            DlNetwork* dlNetwork = networkMap[networkName];
            
            // ---do network-wide init---
            // 1. call stage1Init() on each layer
            // 2. do parent level config/init

            for(int i=0; i<dlNetwork->convLayers_.size(); i++) {
                dlNetwork->convLayers_[i]->initialize();
            }
            
            if(workloadParams_->getDryRun()==true) {
                for(int i=0; i<dlNetwork->convLayers_.size(); i++) {
                    dlNetwork->convLayers_[i]->setDryRun(true);
                }
                tensorMgr->setDryRun(true);

                executeInferenceRun(networkName);

                for(int i=0; i<dlNetwork->convLayers_.size(); i++) {
                    dlNetwork->convLayers_[i]->setDryRun(false);
                }
                tensorMgr->setDryRun(false);
            }  
            Tracer::func_end("DlNetworkMgr::initializeNetwork");  
        }


        void DlNetworkMgr::createDLNetwork(string networkName, int no_of_conv_layers, int *conv_dims) {
            Tracer::func_begin("DlNetworkMgr::createDLNetwork");  

            tensorMgr = new TensorMgr(workloadParams_, timer_, dataFileReadTimer_, no_of_conv_layers);
            std::vector<ConvLayer*> convLayers = createAllLayers(networkName, no_of_conv_layers, conv_dims, tensorMgr);
            DlNetwork* dlNetwork = new DlNetwork(networkName, no_of_conv_layers, conv_dims, convLayers, tensorMgr);

            networkMap.insert({networkName, dlNetwork});
            initializeNetwork(networkName);
            Tracer::func_end("DlNetworkMgr::initializeNetwork");  
        }

        void DlNetworkMgr::executeInferenceRun(string networkName) {
            Tracer::func_begin("DlNetworkMgr::executeInferenceRun");  

            DlNetwork* dlNetwork = networkMap[networkName];

            //std::cout << "Starting new run" << std::endl;
            if(workloadParams_->getTensorMemPolicy()==WorkloadParams::TensorMemPolicy::ALL_MEM_ALLOC_AT_START) {
                for(int i=0; i<dlNetwork->noOfConvLayers_; i++) {
                    dlNetwork->convLayers_[i]->doIOTensorAndWSAllocs();
                    if(workloadParams_->getConvAlgo() == WorkloadParams::ConvAlgo::MIOPEN_FIND_BEST_ALGO) {
                        dlNetwork->convLayers_[i]->findBestAlgo();  
                    }
                }
                for(int i=0; i<dlNetwork->noOfConvLayers_; i++) {
                    dlNetwork->convLayers_[i]->doConv(0.0);
                }
                for(int i=0; i<dlNetwork->noOfConvLayers_; i++) {
                    dlNetwork->convLayers_[i]->doTensorAndWSDeallocs();
                } 
            } else if(workloadParams_->getTensorMemPolicy()==WorkloadParams::TensorMemPolicy::MEM_ALLOC_DEALLOC_EVERY_CONV) {
                for(int i=0; i<dlNetwork->noOfConvLayers_; i++) {
                    dlNetwork->convLayers_[i]->doIOTensorAndWSAllocs();
                    if(workloadParams_->getConvAlgo() == WorkloadParams::ConvAlgo::MIOPEN_FIND_BEST_ALGO) {
                        dlNetwork->convLayers_[i]->findBestAlgo();  
                    }
                    dlNetwork->convLayers_[i]->doConv(0.0);
                    dlNetwork->convLayers_[i]->doTensorAndWSDeallocs();
                }
            } else {
                throw std::runtime_error("TensorMemPolicy is not ALL_MEM_ALLOC_AT_START or MEM_ALLOC_DEALLOC_EVERY_CONV"); 
            }

            Tracer::func_end("DlNetworkMgr::initializeNetwork");  
        }

    }
}