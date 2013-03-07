#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <memory>
#include <functional>
#include <vector>
#include <cmath>
#include "Freq.h"
#include "IRealtime.h"

/*!
 * \brief   Animation and Waypoints
 * \details Does timed waypoint interpolation/easing and paths
 *
 * \author  Grady O'Connell <flipcoder@gmail.com>
 */

template<class T> class Animation;

/*
 * Template class should be a floating point value, a (math) vector,
 * matrix, or quaternion.  But it can also be a color, or anything
 * interpolatable.
 *
 * You define the easing function in the form of:
 *     T func(const T&, const T&, float t)
 * Where t is a value that ranges from 0 to 1.
 *
 * Time is in seconds, but exists on a "timeline", so the actual real time
 * value can be whatever you want, depending on how fast you advance the
 * timeline.
 *
 * Negative time values are not yet supported.
 * I don't have derivative approximation yet, but I think it could be useful
 * for nonlinear easing functions when you need to also know the speed.
 */
template<class T>
class Waypoint:
    public IRealtime
{
    private:
        
        // TODO: execution callback?
        // TODO: expiry callback?
        // TODO: interpolative callback?
        T m_Value;

        Freq::Time m_Time; // length of time
        Freq::Alarm m_Alarm;

        std::function<T (const T&, const T&, float)> m_Easing;

        Animation<T>* m_pAnimation;
        Freq::Timeline* m_pTimeline;

    public:
        
        Waypoint(
            T value,
            Freq::Time time,
            std::function<T (const T&, const T&, float)> easing,
            Animation<T>* nav = nullptr
        ):
            m_Value(value),
            m_Time(time),
            m_Easing(easing),
            m_pTimeline(nav->timeline()),
            m_Alarm(nav->timeline()),
            m_pAnimation(nav)
        {
            assert(m_pTimeline);
        }
        void nav(Animation<T>* nav) {
            assert(nav);
            m_pAnimation = nav;
        }

        virtual ~Waypoint() {}
        void go() {
            m_Alarm.set(m_Time);
        }

        //virtual void logic(Freq::Time t) {
        //    //m_pTimeline->logic(t);
        //    // TODO: trigger callbacks here?
        //}
        
        virtual bool elapsed() {
            return m_Alarm.elapsed();
        }

        unsigned long remaining() {
            return m_Alarm.ms();
        }

        Freq::Alarm& alarm() { return m_Alarm; }

        T& value() {
            return m_Value;
        }

        void easing(std::function<T (const T&, const T&, float)> func) {
            m_Easing = func;
        }

        const std::function<T (const T&, const T&, float)>& easing() const {
            return m_Easing;
        }
};

template<class T>
class Animation:
    public IRealtime
{
    private:

        mutable std::list<Waypoint<T>> m_Waypoints;
        mutable T m_Current = T();

        //float m_fSpeed = 1.0f;
        Freq::Timeline m_Timeline;

        void reset(Freq::Time excess = Freq::Time(0)) const {
            if(m_Waypoints.empty())
                return;

            m_Waypoints.front().go();
            m_Waypoints.front().alarm() += excess;
        }

        void process() const {
            while(!m_Waypoints.empty() && m_Waypoints.front().elapsed())
            {
                auto& w = m_Waypoints.front();
                Freq::Time excess = w.alarm().excess();
                m_Current = m_Waypoints.front().value();
                m_Waypoints.pop_front();
                reset(excess);
            }
        }
        
    public:
    
        Animation() = default;
        Animation(T current):
            m_Current(current)
        {}
        Animation& operator=(T a){
            m_Current = a;
            return *this;
        }

        Animation(
            Waypoint<T> initial,
            T current=T()
        ):
            m_Current(current)
            //m_fSpeed(1.0f)
        {
            initial.nav(this);
            m_Waypoints.push_back(initial);
            reset();
        }

        virtual ~Animation() {}

        operator T() const {
            return get();
        }

        virtual void logic(Freq::Time t) override {
            m_Timeline.logic(t);
        }

        void pause() {
            m_Timeline.pause();
        }
        void resume() {
            m_Timeline.resume();
        }

        /*
         * Append a waypoint to the end of the cycle
         */
        void waypoint(Waypoint<T> waypoint) {
            waypoint.nav(this);
            m_Waypoints.push_back(waypoint);
            reset();
        }

        bool elapsed() const {
            process();
            return m_Waypoints.empty();
        }

        void abort(){
            m_Current = get();
            m_Waypoints.clear();
        }
        void stop(){
            process();
            if(!m_Waypoints.empty()) {
                m_Current = m_Waypoints.back().value();
                m_Waypoints.clear();
            }
        }
        void stop(const T& position) {
            m_Current = position;
            m_Waypoints.clear();
        }
        void stop(
            T position,
            Freq::Time t,
            std::function<T (const T&, const T&, float)> easing
        ){
            m_Current = t ? get() : position;
            m_Waypoints.clear();

            if(t)
            {
                m_Waypoints.push_back(Waypoint<T>(position, t, easing, this));
                reset();
            }
        }

        void ensure(
            T position,
            Freq::Time t,
            std::function<T (const T&, const T&, float)> easing,
            std::function<bool (const T&, const T&)> equality_cmp
        ){
            if(equality_cmp(m_Current, position))
            {
                // TODO: if t < current waypoint time left
                //       use t instead
                return;
            }

            process();
            if(m_Waypoints.empty()) {
                stop(position,t,easing);
                return;
            }
        }

        T get() const {
            process();
            if(m_Waypoints.empty())
                return m_Current;

            return ease(
                m_Waypoints.front().easing(),
                m_Current,
                m_Waypoints.front().value(),
                m_Waypoints.front().alarm().fraction()
            );
        }

        T ease(
            const std::function<T (const T&, const T&, float)>& easing,
            const T& a,
            const T& b,
            float t
        ) const {
            return easing(a,b,t);
        }

        bool empty() const {
            return m_Waypoints.empty();
        }

        //void speed(float s) {
        //    m_fSpeed = s;
        //}
        //float speed() const {
        //    return m_fSpeed;
        //}

        Freq::Timeline* timeline() { return &m_Timeline; }
};

#endif

