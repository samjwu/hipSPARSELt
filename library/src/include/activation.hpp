/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2019-2022 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/

#pragma once
#include "hipsparselt-export.h"
#include <iostream>
#include <string>

enum class hipsparselt_activation_type
{
    none = 0,
    abs,
    clippedrelu,
    gelu,
    leakyrelu,
    relu,
    sigmoid,
    tanh,
    all,
    exp,
};

HIPSPARSELT_EXPORT
constexpr const hipsparselt_activation_type
    string_to_hipsparselt_activation_type(const std::string& value)
{
    return value == "none"          ? hipsparselt_activation_type::none
           : value == "abs"         ? hipsparselt_activation_type::abs
           : value == "clippedrelu" ? hipsparselt_activation_type::clippedrelu
           : value == "gelu"        ? hipsparselt_activation_type::gelu
           : value == "leakyrelu"   ? hipsparselt_activation_type::leakyrelu
           : value == "relu"        ? hipsparselt_activation_type::relu
           : value == "sigmoid"     ? hipsparselt_activation_type::sigmoid
           : value == "tanh"        ? hipsparselt_activation_type::tanh
           : value == "all"         ? hipsparselt_activation_type::all
           : value == "exp"         ? hipsparselt_activation_type::exp
                                    : static_cast<hipsparselt_activation_type>(-1);
}

// Convert hipsparselt_activation_type to string
HIPSPARSELT_EXPORT
constexpr const char* hipsparselt_activation_type_to_string(hipsparselt_activation_type type)
{
    switch(type)
    {
    case hipsparselt_activation_type::abs:
        return "abs";
    case hipsparselt_activation_type::clippedrelu:
        return "clippedrelu";
    case hipsparselt_activation_type::exp:
        return "exp";
    case hipsparselt_activation_type::gelu:
        return "gelu";
    case hipsparselt_activation_type::leakyrelu:
        return "leakyrelu";
    case hipsparselt_activation_type::relu:
        return "relu";
    case hipsparselt_activation_type::sigmoid:
        return "sigmoid";
    case hipsparselt_activation_type::tanh:
        return "tanh";
    case hipsparselt_activation_type::all:
        return "all";
    case hipsparselt_activation_type::none:
        return "none";
    default:
        return "invalid";
    }
}
