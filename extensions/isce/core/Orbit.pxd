#cython: language_level=3
#
# Author: Joshua Cohen
# Copyright 2017
#

from libcpp.vector cimport vector
from Cartesian cimport cartesian_t

cdef extern from "isce/core/Constants.h" namespace "isce::core":
    cdef enum orbitInterpMethod:
        HERMITE_METHOD = 0
        SCH_METHOD = 1
        LEGENDRE_METHOD = 2

cdef extern from "isce/core/Orbit.h" namespace "isce::core":
    cdef cppclass Orbit:
        int basis
        int nVectors
        vector[double] position
        vector[double] velocity
        vector[double] UTCtime

        Orbit() except +
        Orbit(int,int) except +
        Orbit(const Orbit&) except +
        void getPositionVelocity(double, cartesian_t &, cartesian_t &)
        void getStateVector(int,double&,cartesian_t&,cartesian_t&)
        void setStateVector(int,double,cartesian_t&,cartesian_t&)
        void addStateVector(double,cartesian_t&,cartesian_t&)
        int interpolate(double,cartesian_t&,cartesian_t&,orbitInterpMethod)
        int interpolateWGS84Orbit(double,cartesian_t&,cartesian_t&)
        int interpolateLegendreOrbit(double,cartesian_t&,cartesian_t&)
        int interpolateSCHOrbit(double,cartesian_t&,cartesian_t&)
        int computeAcceleration(double,cartesian_t&)
        void printOrbit()
        void loadFromHDR(const char*,int)
        void dumpToHDR(const char*)
