/**
 * MIT License
 * Copyright (c) 2018 Patrick Geneva @ University of Delaware (Robot Perception & Navigation Group)
 * Copyright (c) 2018 Kevin Eckenhoff @ University of Delaware (Robot Perception & Navigation Group)
 * Copyright (c) 2018 Guoquan Huang @ University of Delaware (Robot Perception & Navigation Group)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef VICONGRAPHSOLVER_H
#define VICONGRAPHSOLVER_H


#include <vector>
#include <fstream>
#include <Eigen/Eigen>
#include <ros/ros.h>

#include <gtsam/nonlinear/Values.h>
#include <gtsam/inference/Symbol.h>
#include <gtsam/nonlinear/Marginals.h>
#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <gtsam/nonlinear/LevenbergMarquardtOptimizer.h>
#include <gtsam/nonlinear/ISAM2.h>

#include "cpi/CpiV1.h"
#include "gtsam/ViconPoseFactor.h"
#include "gtsam/JPLNavState.h"
#include "gtsam/ImuFactorCPIv1.h"
#include "meas/Propagator.h"
#include "meas/Interpolator.h"
#include "utils/quat_ops.h"


using namespace std;
using namespace gtsam;


using gtsam::symbol_shorthand::X; // X: our JPL states
using gtsam::symbol_shorthand::C; // C: calibration (c(0)=rot, c(1)=pos)
using gtsam::symbol_shorthand::G; // G: global gravity in the vicon frame




class ViconGraphSolver
{

public:

    // Default constuctor
    ViconGraphSolver(ros::NodeHandle& nh, Propagator* propagator,
                     Interpolator* interpolator, std::vector<double> timestamp_cameras);


    // Build the graph and solve it
    void build_and_solve();

    // TODO: return functions to get final data variables
    // void get_states(std::vector<double>& timestamps, std::vector<Eigen::Matrix<double,15,1>>& imustates);



private:

    // Timing variables
    boost::posix_time::ptime rT1, rT2, rT3, rT4, rT5, rT6, rT7;

    // Measurement data from the rosbag
    Propagator* propagator;
    Interpolator* interpolator;
    std::vector<double> timestamp_cameras;

    // Initial estimates of our variables
    Eigen::Matrix<double,3,1> init_grav_inV;
    Eigen::Matrix<double,3,3> init_R_BtoI;
    Eigen::Matrix<double,3,1> init_p_BinI;

    // Master non-linear GTSAM graph, all created factors
    // Also have all nodes in the graph
    gtsam::NonlinearFactorGraph* graph;
    gtsam::Values values;

    // New factors that have not been optimized yet
    // New nodes that have not been optimized
    // This is used when we are using ISAM2
    gtsam::NonlinearFactorGraph* graph_new;
    gtsam::Values values_new;

    // Map between state timestamp and their IDs
    std::map<double,size_t> map_states;



};




#endif /* VICONGRAPHSOLVER_H */