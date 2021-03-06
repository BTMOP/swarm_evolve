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
#ifndef SIMCONTROL_H_
#define SIMCONTROL_H_
#include <string>
#include <thread>
#include <map>
#include <list>
#include <mutex>
#include <vector>

#include <scrimmage/fwd_decl.h>
#include <scrimmage/network/Interface.h>
#include <scrimmage/common/Timer.h>

#include <future>
#include <memory>
#include <deque>

#include <scrimmage/proto/Shape.pb.h>
#include <scrimmage/proto/Visual.pb.h>

namespace scrimmage {

typedef std::shared_ptr<scrimmage_proto::Shape> ShapePtr;

typedef std::shared_ptr<scrimmage_proto::ContactVisual> ContactVisualPtr;

enum class EndConditionFlags {
    TIME          = 1 << 0,
    ONE_TEAM      = 1 << 1,
    NONE          = 1 << 2,
    ALL_DEAD      = 1 << 3
};

inline EndConditionFlags operator|(EndConditionFlags a, EndConditionFlags b)
{
    return static_cast<EndConditionFlags>(static_cast<int>(a) |
                                            static_cast<int>(b));
}

class SimControl {
 public:
    SimControl();
    bool init();
    void start();
    void display_progress(bool enable);
    void run();
    void force_exit();
    bool external_exit();
    void join();

    bool finished();
    void set_finished(bool finished);

    void get_contacts(std::unordered_map<int, Contact> &contacts);
    void set_contacts(ContactMapPtr &contacts);

    void get_contact_visuals(std::map<int, ContactVisualPtr> &contact_visuals);
    void set_contact_visuals(std::map<int, ContactVisualPtr> &contact_visuals);

    bool generate_entities(double t);

    void set_mission_parse(MissionParsePtr mp);
    MissionParsePtr mp();
    void set_log(std::shared_ptr<Log> &log);

    bool enable_gui();

    void set_time(double t);
    double t();

    void setup_timer(double rate, double time_warp);
    void start_overall_timer();
    void start_loop_timer();
    void loop_wait();
    void inc_warp();
    void dec_warp();
    void pause(bool pause);
    bool paused();
    double time_warp();
    double actual_time_warp();

    void single_step(bool value);
    bool single_step();

    bool end_condition_reached(double t, double dt);

    Timer &timer();

    std::list<MetricsPtr> & metrics();
    PluginManagerPtr &plugin_manager();
    FileSearch &file_search();

    struct Task {
        EntityPtr ent;
        std::promise<bool> prom;
    };
    
    bool take_step();

    void step_taken();

    void set_incoming_interface(InterfacePtr &incoming_interface);
    
    void set_outgoing_interface(InterfacePtr &outgoing_interface);

    void set_parameter_vector(std::vector<double> parameter_vector) { parameter_vector_ = parameter_vector; }
    std::vector<double> parameter_vector() {return parameter_vector_; }

    void set_nn_path(std::string nn_path) { nn_path_ = nn_path; }
    void set_nn_path2(std::string nn_path) { nn_path2_ = nn_path; }
    
 protected:
    // Key: Entity ID
    // Value: Team ID
    std::vector<double> parameter_vector_;
    std::string nn_path_;
    std::string nn_path2_;

    std::shared_ptr<std::unordered_map<int,int> > team_lookup_;

    InterfacePtr incoming_interface_;
    InterfacePtr outgoing_interface_;
    
    std::thread network_thread_;
    
    MissionParsePtr mp_;

    std::list<EntityPtr> ents_;

    ContactMapPtr contacts_;
    
    std::map<int, std::list<ShapePtr> > shapes_;    
    
    std::map<int, ContactVisualPtr> contact_visuals_;

    std::thread thread_;
    bool display_progress_;

    std::string jsbsim_root_;

    double t0_;
    double tend_;
    double dt_;
    double t_;
    bool paused_;
    bool single_step_;

    bool take_step_;

    Timer timer_;

    bool finished_;
    bool exit_;

    std::mutex finished_mutex_;
    std::mutex contacts_mutex_;
    std::mutex exit_mutex_;
    std::mutex timer_mutex_;
    std::mutex paused_mutex_;
    std::mutex single_step_mutex_;
    std::mutex take_step_mutex_;
    std::mutex time_mutex_;
    std::mutex time_warp_mutex_;
    std::mutex entity_pool_mutex_;

    bool use_entity_threads_;
    int num_entity_threads_;
    bool entity_pool_stop_;
    std::deque<std::shared_ptr<Task>> entity_pool_queue_;
    std::condition_variable_any entity_pool_condition_var_;
    std::vector<std::thread> entity_worker_threads_;
    void worker();
    void run_entities();

    std::shared_ptr<Log> log_;

    //InteractionDetection inter_detect_;

    EndConditionFlags end_conditions_;

    RandomPtr random_;

    PluginManagerPtr plugin_manager_;

    bool collision_exists(Eigen::Vector3d &p);

    std::shared_ptr<GeographicLib::LocalCartesian> proj_;

    std::list<EntityInteractionPtr> ent_inters_;
    std::list<MetricsPtr> metrics_;

    NetworkPtr network_;

    int next_id_;
    FileSearch file_search_;
    RTreePtr rtree_;

    void create_rtree();
    void run_autonomy();
    void set_autonomy_contacts();
    void run_dynamics();
    bool run_interaction_detection();
    void run_logging();
    void run_remove_inactive();
    void run_check_network_msgs();
    void run_send_shapes();
    void run_send_contact_visuals();

    bool send_shutdown_msg_;

    PluginPtr pubsub_;
    PublisherPtr pub_end_time_;
    PublisherPtr pub_ent_gen_;
    PublisherPtr pub_ent_rm_;
    PublisherPtr pub_ent_pres_end_;
    PublisherPtr pub_ent_int_exit_;
    PublisherPtr pub_no_teams_;
    PublisherPtr pub_one_team_;
    
private:
};
}
#endif
