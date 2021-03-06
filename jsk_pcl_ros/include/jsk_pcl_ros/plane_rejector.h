// -*- mode: C++ -*-
/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2013, Ryohei Ueda and JSK Lab
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

#ifndef JSK_PCL_ROS_PLANE_REJECTOR_H_
#define JSK_PCL_ROS_PLANE_REJECTOR_H_


// ros
#include <ros/ros.h>
#include <ros/names.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/transform_broadcaster.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>
#include <tf/transform_listener.h>
#include <dynamic_reconfigure/server.h>
// pcl
#include <pcl_ros/pcl_nodelet.h>

#include <jsk_pcl_ros/PolygonArray.h>
#include <jsk_pcl_ros/ModelCoefficientsArray.h>
#include "jsk_pcl_ros/PlaneRejectorConfig.h"

namespace jsk_pcl_ros
{
  class PlaneRejector: public pcl_ros::PCLNodelet
  {
  public:
    typedef message_filters::sync_policies::ExactTime< jsk_pcl_ros::PolygonArray,
                                                       jsk_pcl_ros::ModelCoefficientsArray > SyncPolicy;
    typedef jsk_pcl_ros::PlaneRejectorConfig Config;
  protected:
    virtual void onInit();
    virtual void reject(const jsk_pcl_ros::PolygonArray::ConstPtr& polygons,
                        const jsk_pcl_ros::ModelCoefficientsArray::ConstPtr& coefficients);
    message_filters::Subscriber<jsk_pcl_ros::PolygonArray> sub_polygons_;
    message_filters::Subscriber<jsk_pcl_ros::ModelCoefficientsArray> sub_coefficients_;
    boost::shared_ptr<message_filters::Synchronizer<SyncPolicy> >sync_;
    bool use_tf2_;
    std::string processing_frame_id_;
    // axis
    Eigen::Vector3d reference_axis_;
    double angle_thr_;
    boost::shared_ptr<tf::TransformListener> listener_;
    boost::mutex mutex_;
    boost::shared_ptr <dynamic_reconfigure::Server<Config> > srv_;
    ros::Publisher polygons_pub_, coefficients_pub_;
    virtual void configCallback (Config &config, uint32_t level);
    virtual bool readVectorParam(const std::string& param_name);
    virtual double getXMLDoubleValue(XmlRpc::XmlRpcValue val);
  private:
    
  };
}

#endif 
