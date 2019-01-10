//
// Author: Joshua Cohen, Bryan Riel, Liang Yu
// Copyright 2017-2018
//

#ifndef ISCE_CORE_INTERPOLATOR_H
#define ISCE_CORE_INTERPOLATOR_H

#include <complex>
#include <valarray>

// isce::core
#include "Constants.h"
#include "Matrix.h"

// Declaration
namespace isce {
    namespace core {
        // The interpolator classes
        template <typename U> class Interpolator;
        template <typename U> class BilinearInterpolator;
        template <typename U> class BicubicInterpolator;
        template <typename U> class NearestNeighborInterpolator;
        template <typename U> class Spline2dInterpolator;
        template <typename U> class Sinc2dInterpolator;
    }
}

// Definition of parent Interpolator
template <typename U>
class isce::core::Interpolator {

    // Public interface
    public:
        /** Virtual destructor (allow destruction of base Interpolator pointer) */
        virtual ~Interpolator() {}

        /** Interpolate at a given coordinate for an input isce::core::Matrix */
        virtual U interpolate(double x, double y, const Matrix<U> & z) = 0;

        /** Return interpolation method. */
        isce::core::dataInterpMethod method() const { return _method; }

    // Protected constructor and data to be used by derived classes
    protected:
        inline Interpolator(isce::core::dataInterpMethod method) :
            _method{method} {}
        isce::core::dataInterpMethod _method;
};

// Definition of BilinearInterpolator
template <typename U>
class isce::core::BilinearInterpolator : public isce::core::Interpolator<U> {

    public:
        /** Default constructor */
        inline BilinearInterpolator() : 
            isce::core::Interpolator<U>(isce::core::BILINEAR_METHOD) {}

        /** Interpolate at a given coordinate. */
        U interpolate(double x, double y, const Matrix<U> & z);

        /** Interpolate at a given coordinate for data passed as a valarray */
        U interpolate(double x, double y, std::valarray<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }

        /** Interpolate at a given coordinate for data passed as a vector */
        U interpolate(double x, double y, std::vector<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }
};

// Definition of BicubicInterpolator
template <typename U>
class isce::core::BicubicInterpolator : public isce::core::Interpolator<U> {

    public:
        /** Default constructor */
        BicubicInterpolator();

        /** Interpolate at a given coordinate. */
        U interpolate(double x, double y, const Matrix<U> & z);

        /** Interpolate at a given coordinate for data passed as a valarray */
        U interpolate(double x, double y, std::valarray<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }

        /** Interpolate at a given coordinate for data passed as a vector */
        U interpolate(double x, double y, std::vector<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }

    // Kernel weights
    private:
        std::valarray<double> _weights;
};

// Definition of NearestNeighborInterpolator
template <typename U>
class isce::core::NearestNeighborInterpolator : public isce::core::Interpolator<U> {

    public:
        /** Default constructor */
        inline NearestNeighborInterpolator() : 
            isce::core::Interpolator<U>(isce::core::NEAREST_METHOD) {}

        /** Interpolate at a given coordinate. */
        U interpolate(double x, double y, const Matrix<U> & z);

        /** Interpolate at a given coordinate for data passed as a valarray */
        U interpolate(double x, double y, std::valarray<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }

        /** Interpolate at a given coordinate for data passed as a vector */
        U interpolate(double x, double y, std::vector<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }
};

// Definition of Spline2dInterpolator
template <typename U>
class isce::core::Spline2dInterpolator : public isce::core::Interpolator<U> {

    public:
        using isce::core::Interpolator<U>::interpolate; 

    public:
        /** Default constructor. */
        Spline2dInterpolator(size_t order);

        /** Interpolate at a given coordinate. */
        U interpolate(double x, double y, const Matrix<U> & z);

        /** Interpolate at a given coordinate for data passed as a valarray */
        U interpolate(double x, double y, std::valarray<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }

        /** Interpolate at a given coordinate for data passed as a vector */
        U interpolate(double x, double y, std::vector<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }

    // Data members
    private:
        size_t _order;

    // Utility spline functions
    private:
        void _initSpline(const std::valarray<double> &,
                         int,
                         std::valarray<double> &,
                         std::valarray<double> &);

        double _spline(double,
                       const std::valarray<double> &,
                       int,
                       const std::valarray<double> &);
};

// Definition of Sinc2dInterpolator
template <typename U>
class isce::core::Sinc2dInterpolator : public isce::core::Interpolator<U> {

    public:
        /** Default constructor. */
        Sinc2dInterpolator(int sincLen, int sincSub);

        /** Interpolate at a given coordinate. */
        U interpolate(double x, double y, const Matrix<U> & z);

        /** Interpolate at a given coordinate for data passed as a valarray */
        U interpolate(double x, double y, std::valarray<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }

        /** Interpolate at a given coordinate for data passed as a vector */
        U interpolate(double x, double y, std::vector<U> & z_data, size_t width) {
            isce::core::Matrix<U> z(z_data, width);
            return interpolate(x, y, z);
        }

    private:
        // Compute sinc coefficients 
        void _sinc_coef(double beta, double relfiltlen, int decfactor, double pedestal,
                        int weight, std::valarray<double> & filter);

        // Evaluate sinc
        U _sinc_eval_2d(const isce::core::Matrix<U> & z, int intpx, int intpy,
                        double frpx, double frpy, int xlen, int ylen);

    private:
        isce::core::Matrix<double> _kernel;
        int _kernelLength, _kernelWidth, _sincHalf;
}; 

// Extra interpolation and utility functions
namespace isce {
    namespace core {

        /** Utility function to create interpolator pointer given an interpolator enum type */
        template <typename U>
        inline Interpolator<U> * createInterpolator(
            isce::core::dataInterpMethod method, size_t order = 6
            ) {
            if (method == isce::core::BILINEAR_METHOD) {
                return new isce::core::BilinearInterpolator<U>();
            } else if (method == isce::core::BICUBIC_METHOD) {
                return new isce::core::BicubicInterpolator<U>();
            } else if (method == isce::core::BIQUINTIC_METHOD) {
                return new isce::core::Spline2dInterpolator<U>(order);
            } else {
                return new isce::core::BilinearInterpolator<U>();
            }
        }

        // Sinc evaluation in 1D 
        template <class U, class V>
        U sinc_eval(const Matrix<U> &, const Matrix <V> &, int, int, double, int);

    }
}

#endif

// end of file
