/// ---------------------------------------------------------------------------
/// @section LICENSE
///  
/// Copyright (c) 2016 Georgia Tech Research Institute (GTRI) 
///               All Rights Reserved
///  
/// The above copyright notice and this permission notice shall be included in 
/// all copies or substantial portions of the Software.
///  
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
/// DEALINGS IN THE SOFTWARE.
/// ---------------------------------------------------------------------------
/// @file filename.ext
/// @author Kevin DeMarco <kevin.demarco@gtri.gatech.edu> 
/// @author Eric Squires <eric.squires@gtri.gatech.edu>
/// @version 1.0
/// ---------------------------------------------------------------------------
/// @brief A brief description.
/// 
/// @section DESCRIPTION
/// A long description.
/// ---------------------------------------------------------------------------
#ifndef SIMPLEAIRCRAFT_3D_H_
#define SIMPLEAIRCRAFT_3D_H_
#include <scrimmage/math/State.h>
#include <scrimmage/motion/MotionModel.h>
#include <scrimmage/motion/Controller.h>
#include <scrimmage/common/PID.h>

class RigidBody6DOF : public scrimmage::MotionModel{
 public:
    RigidBody6DOF();     

    virtual std::tuple<int,int,int> version();
     
    virtual bool init(std::map<std::string, std::string> &info,
                      std::map<std::string, std::string> &params);          
    virtual bool step(double time, double dt);     

    virtual void model(const vector_t &x , vector_t &dxdt , double t);

    virtual void teleport(scrimmage::StatePtr &state);     
     
    class Controller : public scrimmage::Controller {
     public:
        virtual std::shared_ptr<Eigen::Vector4d> u() = 0; 
    };

    void set_u(std::shared_ptr<Eigen::Vector4d> u) {u_ = u;}

 protected:
    scrimmage::PID heading_pid_;
    scrimmage::PID alt_pid_;
    scrimmage::PID vel_pid_;

    double length_;
    std::shared_ptr<Eigen::Vector4d> u_;

    double min_velocity_;
    double max_velocity_;
    double max_roll_;
    double max_pitch_;

    scrimmage::Quaternion quat_world_;
    scrimmage::Quaternion quat_local_;
    
private:     
};

#endif
