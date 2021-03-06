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
#ifndef LearnedBehavior_H_
#define LearnedBehavior_H_
#include <scrimmage/autonomy/Autonomy.h>

class LearnedBehavior : public scrimmage::Autonomy {
public:
    LearnedBehavior();
    virtual void init(std::map<std::string,std::string> &params);
    virtual bool step_autonomy(double t, double dt);
protected:
	double w_align_;
    double w_avoid_;
    double w_centroid_;
    double desired_vel;
private:     
};

#endif
