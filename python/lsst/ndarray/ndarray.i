// -*- lsst-c++ -*-
/* 
 * LSST Data Management System
 * Copyright 2008, 2009, 2010 LSST Corporation.
 * 
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the LSST License Statement and 
 * the GNU General Public License along with this program.  If not, 
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */
 
%{
#include "lsst/ndarray/python.h"
#include "lsst/ndarray/python/eigen.h"
#include <boost/scoped_ptr.hpp>
%}

#pragma SWIG nowarn=467

%define %declareNumPyConverters(TYPE...)
%typemap(out) TYPE {
    $result = lsst::ndarray::PyConverter< TYPE >::toPython($1);
}
%typemap(out) TYPE const &, TYPE &, TYPE const *, TYPE * {
    $result = lsst::ndarray::PyConverter< TYPE >::toPython(*$1);
}
%typemap(typecheck) TYPE, TYPE const *, TYPE const & {
    lsst::ndarray::PyPtr tmp($input,true);
    $1 = lsst::ndarray::PyConverter< TYPE >::fromPythonStage1(tmp);
    if (!($1)) PyErr_Clear();
}
%typemap(in) TYPE const & (TYPE val) {
    lsst::ndarray::PyPtr tmp($input,true);
    if (!lsst::ndarray::PyConverter< TYPE >::fromPythonStage1(tmp)) return NULL;
    if (!lsst::ndarray::PyConverter< TYPE >::fromPythonStage2(tmp, val)) return NULL;
    $1 = &val;
}
%typemap(in) TYPE {
    lsst::ndarray::PyPtr tmp($input,true);
    if (!lsst::ndarray::PyConverter< TYPE >::fromPythonStage1(tmp)) return NULL;
    if (!lsst::ndarray::PyConverter< TYPE >::fromPythonStage2(tmp, $1)) return NULL;
}
%enddef