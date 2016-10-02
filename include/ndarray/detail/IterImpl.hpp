// -*- c++ -*-
/*
 * Copyright (c) 2010-2016, Jim Bosch
 * All rights reserved.
 *
 * ndarray is distributed under a simple BSD-like license;
 * see the LICENSE file that should be present in the root
 * of the source distribution, or alternately available at:
 * https://github.com/ndarray/ndarray
 */
#ifndef NDARRAY_IterImpl_hpp_INCLUDED
#define NDARRAY_IterImpl_hpp_INCLUDED

#include "ndarray/common.hpp"
#include "ndarray/DType.hpp"
#include "ndarray/detail/CompressedPair.hpp"
#include "ndarray/detail/ArrayImpl.hpp"
#include "ndarray/detail/Array_1.hpp"
#include "ndarray/detail/ArrayRef_1.hpp"
#include "ndarray/detail/ArrayTraits_1.hpp"

namespace ndarray {
namespace detail {

template <typename T, size_t N, offset_t C>
class IterImpl {
    template <typename U, size_t M, offset_t D> friend struct IterTraits;
    static_assert(N != 1, "IterImpl not defined for N==1 unless C==0; use T*");
    typedef typename ArrayTraits<T,N,C>::reference internal_t;
public:

    typedef DType<T> dtype_t;

    IterImpl() : _current(), _stride() {}

    IterImpl(internal_t current, offset_t stride) :
        _current(std::move(current)), _stride(stride)
    {}

    IterImpl(IterImpl const & other) = default;

    IterImpl(IterImpl && other) = default;

    IterImpl & operator=(IterImpl const & other) {
        this->ptr() = other;
        _stride = other._stride;
        return *this;
    }

    IterImpl & operator=(IterImpl && other) {
        this->ptr() = std::move(other);
        _stride = other._stride;
        return *this;
    }

    byte_t * buffer() const { return _current._impl.buffer; }

    dtype_t const & dtype() const { return _current.dtype(); }

    offset_t stride() const { return _stride; }

    void advance(offset_t n) {
        _current._impl.buffer += n*_stride;;
    }

private:
    internal_t _current;
    offset_t _stride;
};

template <typename T>
class IterImpl<T,1,0> {
    template <typename U, size_t M, offset_t D> friend struct IterTraits;
public:

    typedef DType<T> dtype_t;

    IterImpl() : _holder(dtype_t(), std::make_pair(nullptr, 0)) {}

    IterImpl(byte_t * buffer, dtype_t dtype_, offset_t stride) :
        _holder(std::move(dtype_), std::make_pair(buffer, stride))
    {}

    IterImpl(IterImpl const & other) = default;

    IterImpl(IterImpl && other) = default;

    IterImpl & operator=(IterImpl const & other) = default;

    IterImpl & operator=(IterImpl && other) = default;

    byte_t * buffer() const { return _holder.second().first; }

    dtype_t const & dtype() const { return _holder.first(); }

    offset_t stride() const { return _holder.second().second; }

    void advance(offset_t n) {
        _holder.second().first += n*stride();
    }

private:
    CompressedPair<dtype_t,std::pair<byte_t*, offset_t>> _holder;
};

} // namespace detail
} // namespace ndarray

#endif // !NDARRAY_IterImpl_hpp_INCLUDED