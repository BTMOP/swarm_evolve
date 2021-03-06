#include <scrimmage/common/Timer.h>
#include <boost/thread.hpp>
#include <ctime>

namespace scrimmage{

void Timer::start_overall_timer()
{
    start_time_ = boost::posix_time::second_clock::local_time();
    actual_time_ = start_time_;
    actual_elapsed_time_ = start_time_ - start_time_; // 0
    sim_time_ = start_time_;
    sim_elapsed_time_ = start_time_ - start_time_; // 0
}

boost::posix_time::time_duration Timer::elapsed_time() {
    return boost::posix_time::second_clock::local_time() - start_time_;
}

void Timer::start_loop_timer()
{
    loop_timer_ = boost::posix_time::second_clock::local_time();

    boost::posix_time::time_duration time_diff = loop_timer_ - actual_time_;

    actual_time_ = loop_timer_;
    actual_elapsed_time_ += time_diff;

    boost::posix_time::time_duration sim_time_diff = time_diff * time_warp_;
    sim_time_ += sim_time_diff;
    sim_elapsed_time_ += sim_time_diff;
}

bool Timer::loop_wait()
{
    bool missed_deadline = true;

    boost::posix_time::ptime time = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration time_diff = time - loop_timer_;

    if (time_diff < iterate_period_) {
        boost::posix_time::time_duration remainder = iterate_period_ - time_diff;
        boost::this_thread::sleep(remainder);
        missed_deadline = false;
    }
    return missed_deadline;
}

void Timer::set_iterate_rate(double iterate_rate)
{
    iterate_rate_ = iterate_rate;
}

void Timer::set_time_warp(double time_warp)
{
    time_warp_ = time_warp;
}

void Timer::update_time_config()
{
    if (iterate_rate_ > 0 && time_warp_ > 0) {
        double milli = (1.0/iterate_rate_ * 1000000.0)/time_warp_;
        iterate_period_ = boost::posix_time::time_duration(0,0,0,(long)milli);
    } else {
        iterate_period_ = boost::posix_time::time_duration(0,0,0,0); // 0
    }
}

unsigned long Timer::getnanotime()
{
    unsigned long nano = 0;
    timespec ts;
    // clock_gettime(CLOCK_MONOTONIC, &ts); // Works on FreeBSD
    clock_gettime(CLOCK_REALTIME, &ts); // Works on Linux

    nano = ts.tv_sec * 1e9 + ts.tv_nsec;

    //cout << "Time: " << nano << endl;
    return nano;
}

void Timer::inc_warp()
{
    if (time_warp_ < 2) {
        time_warp_ *= 2;
    } else {
        time_warp_ += 1.0;
    }
    update_time_config();
}

void Timer::dec_warp()
{
    if (time_warp_ < 2) {
        time_warp_ /= 2;
    } else {
        time_warp_ -= 1.0;
    }
    update_time_config();
}

double Timer::time_warp()
{
    return time_warp_;
}

}
