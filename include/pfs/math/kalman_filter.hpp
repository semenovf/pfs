#pragma once
#include <pfs/limits.hpp>
#include <pfs/math.hpp>

//
// Usefull references
//------------------------------------------------------------------------------
// [Kalman filter tutorial](https://www.kalmanfilter.net)
// [Kalman filter](https://en.wikipedia.org/wiki/Kalman_filter)
//

namespace pfs
{

template <typename T>
struct constant_dynamic_model
{
    // State extrapolation
    T predicted_state (T const & x) { return x; }

    // Covariance extrapolation
    double predicted_covariance (double p) { return p; }

    // Noise variance
    double noise () { return 0.0; }
};

//
// One-dimensional Kalman filter without the process noise
//
template <typename T, typename Model>
class kalman_filter_1d
{
public:
    typedef T value_type;
    typedef T & reference;
    typedef T const & const_reference;
    typedef Model model_type; // system's dynamic model

public:
    kalman_filter_1d (Model const & model = Model())
        : _model(model)
    {}

    /**
     * @param x Initial state
     * @param sigma Initial deviation (the covariance is a @a sigma power of 2)
     */
    void initialize (const_reference x
            , double sigma = numeric_limits<double>::max())
    {
        _x[0] = x;
        _p[0] = sigma * sigma;

        // Predict the next state based on the initialization values.
        _x[1] = _model.predicted_state(_x[0]);
        _p[1] = _model.predicted_covariance(_p[0]) + _model.noise();
    }

    /**
     * @param sigma Measure deviation
     */
    void update (const_reference y, double sigma)
    {
        // Measurement uncertainty
        _r = sigma * sigma;

        // Kalman gain
        _K = _p[1] / (_p[1] + _r);

        // Update state
        _x[0] = _x[1] + _K * (y - _x[1]);

        // Update covariance
        _p[0] = _p[1] * (1 - _K);
    }

    void predict ()
    {
        _x[1] = _model.predicted_state(_x[0]);
        _p[1] = _model.predicted_covariance(_p[0]) + _model.noise();
    }

    const_reference state () const
    {
        return _x[0];
    }

    double deviation () const
    {
        return sqrt(_p[0]);
    }

private:
    model_type _model;

    // State value
    // [0] - estimated
    // [1] - predicted
    value_type _x[2];

    // Estimate uncertainty
    // [0] - estimated
    // [1] - extrapolated
    double     _p[2];

    double     _r;    // measurement uncertainty
    double     _K;    // Kalman gain
};

} // namespace pfs
