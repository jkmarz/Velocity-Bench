/*
 * Modifications Copyright (C) 2023 Intel Corporation
 *
 * This Program is subject to the terms of the GNU Lesser General Public License v3.0 or later
 * 
 * If a copy of the license was not distributed with this file, you can obtain one at 
 * https://www.gnu.org/licenses/lgpl-3.0-standalone.html
 * 
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */


//
// Created by zeyad-osama on 07/02/2021.
//

#ifndef OPERATIONS_LIB_TEST_UTILS_DUMMY_TRACE_GENERATOR_HPP
#define OPERATIONS_LIB_TEST_UTILS_DUMMY_TRACE_GENERATOR_HPP

#include <operations/data-units/concrete/holders/GridBox.hpp>

namespace operations {
    namespace testutils {

        float *generate_dummy_trace(const std::string &aFileName,
                                    dataunits::GridBox *apGridBox,
                                    int trace_stride_x,
                                    int trace_stride_y);

    } //namespace testutils
} //namespace operations

#endif //OPERATIONS_LIB_TEST_UTILS_DUMMY_TRACE_GENERATOR_HPP
