#include <cstring>
#include <pfs/math.hpp>
#include <pfs/math/kalman_filter.hpp>
#include "../catch.hpp"

//
// Tests are based on examples from
// https://www.kalmanfilter.net/kalman1d.html
//

////////////////////////////////////////////////////////////////////////////////
// ESTIMATING THE BUILDING HEIGHT
////////////////////////////////////////////////////////////////////////////////
TEST_CASE("Estimateing the building height") {
    static double EPSILON = 0.01;
    static double const ALTIMETER_ERROR = 5.0;

    double true_height = 50.0;
    double height_measurements[] = {48.54, 47.11, 55.01, 55.15, 49.89
            , 40.85, 46.72, 50.05, 51.27, 49.95};

    // Estimate the building height simply by looking on it.
    double estimated_building_height = 60.0;

    // Human’s estimation error (standard deviation) is about 15 meters: sigma = 15.
    // Consequently the variance is 225: sigma * sigma = 225.
    double estimation_error = 15;

    pfs::kalman_filter_1d<double, pfs::constant_dynamic_model<double> > kf;

    // Step 0 - Initialization
    kf.initialize(estimated_building_height, estimation_error);

    ////////////////////////////////////////////////////////////////////////////
    // First iteration

    // Step 1 - Measure
    double y = height_measurements[0];

    // Measurement error (the standard deviation of the altimeter)
    double const measurement_error = ALTIMETER_ERROR; // Constant

    // Step 2 - Update
    kf.update(y, measurement_error);

    // Step 3 - Predict
    kf.predict();

    CHECK(pfs::abs(kf.state() - double(49.69)) < EPSILON);

    ////////////////////////////////////////////////////////////////////////////
    // Second iteration
    y = height_measurements[1];
    kf.update(y, measurement_error);
    kf.predict();
    CHECK(pfs::abs(kf.state() - double(48.47)) < EPSILON);

    ////////////////////////////////////////////////////////////////////////////
    // Iterations 3 - 10
    y = height_measurements[2];
    kf.update(y, measurement_error);
    kf.predict();
    CHECK(pfs::abs(kf.state() - double(50.57)) < EPSILON);

    y = height_measurements[3];
    kf.update(y, measurement_error);
    kf.predict();
    CHECK(pfs::abs(kf.state() - double(51.68)) < EPSILON);

    y = height_measurements[4];
    kf.update(y, measurement_error);
    kf.predict();
    CHECK(pfs::abs(kf.state() - double(51.33)) < EPSILON);

    y = height_measurements[5];
    kf.update(y, measurement_error);
    kf.predict();
    CHECK(pfs::abs(kf.state() - double(49.62)) < EPSILON);

    y = height_measurements[6];
    kf.update(y, measurement_error);
    kf.predict();
    CHECK(pfs::abs(kf.state() - double(49.21)) < EPSILON);

    y = height_measurements[7];
    kf.update(y, measurement_error);
    kf.predict();
    CHECK(pfs::abs(kf.state() - double(49.31)) < EPSILON);

    y = height_measurements[8];
    kf.update(y, measurement_error);
    kf.predict();
    CHECK(pfs::abs(kf.state() - double(49.53)) < EPSILON);

    y = height_measurements[9];
    kf.update(y, measurement_error);
    kf.predict();
    CHECK(pfs::abs(kf.state() - double(49.57)) < EPSILON);
}

////////////////////////////////////////////////////////////////////////////////
// ESTIMATING THE TEMPERATURE OF THE LIQUID IN A TANK
////////////////////////////////////////////////////////////////////////////////
struct tank_temp_model : pfs::constant_dynamic_model<double>
{
    double noise () { return 0.0001; }
};

TEST_CASE("Estimateing the temperature of the liquid in a tank") {
    static double EPSILON = 0.001;
    static double const TERMOMETER_ERROR = 0.1; // degrees Celsius

//     double true_temps[] = { 49.979, 50.025, 50.0, 50.003, 49.994
//             , 50.002, 49.999, 50.006, 49.998, 49.991 };

    double temp_measurements[] = { 49.95, 49.967, 50.1, 50.106, 49.992
                , 49.819, 49.933, 50.007, 50.023, 49.99 };

    double temp_sample_values[] = { 49.95, 49.959, 50.007, 50.032, 50.023
                , 49.987, 49.978, 49.983, 49.988, 49.988 };

    int measurement_count = sizeof(temp_measurements) / sizeof(temp_measurements[0]);

    double const measurement_error = TERMOMETER_ERROR; // Constant

    double estimation_error = 100;

    pfs::kalman_filter_1d<double, tank_temp_model> kf;

    // Step 0 - Initialization
    kf.initialize(10.0, estimation_error);

    for (int i = 0; i < measurement_count; i++) {
        // Step 1 - Measure
        double y = temp_measurements[i];

        // Step 2 - Update
        kf.update(y, measurement_error);

        // Step 3 - Predict
        kf.predict();

        CHECK(pfs::abs(kf.state() - temp_sample_values[i]) < EPSILON);
    }

    CHECK(pfs::abs(kf.deviation() - 0.036));
}

////////////////////////////////////////////////////////////////////////////////
// ESTIMATING THE TEMPERATURE OF THE HEATING LIQUID
////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Estimateing the temperature of the heating liquid") {
    static double EPSILON = 0.001;
    static double const TERMOMETER_ERROR = 0.1; // degrees Celsius

    double temp_measurements[] = { 50.45, 50.967, 51.6, 52.106, 52.492
            , 52.819, 53.433, 54.007, 54.523, 54.99 };

    double temp_sample_values[] = { 50.45, 50.71, 51.011, 51.295, 51.548
            , 51.779, 52.045, 52.331, 52.626, 52.925 };

    int measurement_count = sizeof(temp_measurements) / sizeof(temp_measurements[0]);

    double const measurement_error = TERMOMETER_ERROR; // Constant

    double estimation_error = 100;

    pfs::kalman_filter_1d<double, tank_temp_model> kf;

    // Step 0 - Initialization
    kf.initialize(10.0, estimation_error);

    for (int i = 0; i < measurement_count; i++) {
        // Step 1 - Measure
        double y = temp_measurements[i];

        // Step 2 - Update
        kf.update(y, measurement_error);

        // Step 3 - Predict
        kf.predict();

        CHECK(pfs::abs(kf.state() - temp_sample_values[i]) < EPSILON);
    }

    CHECK(pfs::abs(kf.deviation() - 0.036));
}
