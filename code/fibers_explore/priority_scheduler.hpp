#pragma once

// Implementation is basing on Nat Goodspeed implementation of scheduler with
// priorities

#include <boost/fiber/all.hpp>

class priority_props : public boost::fibers::fiber_properties {
   public:
    priority_props(boost::fibers::context* ctx)
        : fiber_properties(ctx), priority_(0) {}

    int get_priority() const { return priority_; }

    void set_priority(int p) {
        if (p != priority_) {
            priority_ = p;
            notify();
        }
    }

    std::string name;

   private:
    int priority_;
};

class priority_scheduler
    : public boost::fibers::algo::algorithm_with_properties<priority_props> {
   private:
    using rqueue_t = boost::fibers::scheduler::ready_queue_type;

    rqueue_t rqueue_;
    std::mutex mtx_{};
    std::condition_variable cnd_{};
    bool flag_{false};

   public:
    priority_scheduler() : rqueue_() {}

    virtual void awakened(boost::fibers::context* ctx,
                          priority_props& props) noexcept {
        int ctx_priority = props.get_priority();

        rqueue_t::iterator i(std::find_if(
            rqueue_.begin(), rqueue_.end(),
            [ctx_priority, this](boost::fibers::context& c) {
                return properties(&c).get_priority() < ctx_priority;
            }));

        rqueue_.insert(i, *ctx);

        std::cout << "awakened(" << props.name << "): ";
        describe_ready_queue();
    }

    virtual boost::fibers::context* pick_next() noexcept {
        if (rqueue_.empty()) {
            return nullptr;
        }
        boost::fibers::context* ctx(&rqueue_.front());
        rqueue_.pop_front();
        std::cout << "pick_next() resuming " << properties(ctx).name << ": ";
        describe_ready_queue();
        return ctx;
    }

    virtual bool has_ready_fibers() const noexcept { return !rqueue_.empty(); }

    virtual void property_change(boost::fibers::context* ctx,
                                 priority_props& props) noexcept {
        std::cout << "property_change(" << props.name << '('
                  << props.get_priority() << ")): ";

        if (!ctx->ready_is_linked()) {
            describe_ready_queue();
            return;
        }

        ctx->ready_unlink();
        awakened(ctx, props);
    }

    void describe_ready_queue() {
        if (rqueue_.empty()) {
            std::cout << "[empty]";
        } else {
            const char* delim = "";
            for (boost::fibers::context& ctx : rqueue_) {
                priority_props& props(properties(&ctx));
                std::cout << delim << props.name << '(' << props.get_priority()
                          << ')';
                delim = ", ";
            }
        }
        std::cout << std::endl;
    }

    void suspend_until(
        std::chrono::steady_clock::time_point const& time_point) noexcept {
        if ((std::chrono::steady_clock::time_point::max)() == time_point) {
            std::unique_lock<std::mutex> lk(mtx_);
            cnd_.wait(lk, [this]() { return flag_; });
            flag_ = false;
        } else {
            std::unique_lock<std::mutex> lk(mtx_);
            cnd_.wait_until(lk, time_point, [this]() { return flag_; });
            flag_ = false;
        }
    }

    void notify() noexcept {
        std::unique_lock<std::mutex> lk(mtx_);
        flag_ = true;
        lk.unlock();
        cnd_.notify_all();
    }
};
