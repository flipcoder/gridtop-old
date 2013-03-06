#ifndef _FREQ_H
#define _FREQ_H

#include <iostream>
#include <chrono>
#include "Util.h"

class Freq
{
public:

    class Time
    {
        private:
            unsigned m_Value;

        public:
            static Time never() {
                return Time(std::numeric_limits<unsigned>::max());
            }
            Time():
                m_Value(0) {}
            explicit Time(unsigned ms) {
                m_Value = ms;
            }
            Time(const Time& t) {
                m_Value = t.internal();
            }
            unsigned internal() const { return m_Value; }
            //static Time seconds(unsigned s) { return Time(s * 1000);}
            static Time seconds(float s) { return Time((unsigned)(s * 1000.0f)); }
            //static Time minutes(unsigned m) { return Time(m * 60000);}
            static Time minutes(float m) { return Time((unsigned)(m * 60000.0f));}
            static Time ms(unsigned ms) { return Time(ms); }

            operator bool() const { return m_Value; }

            Time& operator+=(Time t) {
                m_Value += t.internal();
                return *this;
            }

            float s() const { return m_Value / 1000.0f; }
            float seconds() const { return m_Value / 1000.0f; }
            unsigned ms() const { return m_Value; }
    };

    class Timeline {
        private:
            //unsigned m_PassedTime;
            unsigned m_PassedTime;
            //unsigned m_uiLastAdvance;
            float m_fSpeed;
        public:
            Timeline():
                m_PassedTime(0L),
                m_fSpeed(1.0f)
            {}
            virtual ~Timeline() {}
            virtual unsigned ms() const {
                return m_PassedTime;
            }
            virtual float seconds() const {
                return m_PassedTime / 1000.0f;
            }
            
            /*virtual */Freq::Time logic(Freq::Time t) { // ms
                float advance = t.ms() * m_fSpeed;
                //m_uiLastAdvance = round_int(advance);
                m_PassedTime += std::rint(advance);
                return Freq::Time::ms(std::rint(advance));
            }
            //float logic(float a) { // seconds
            //    float advance = a * m_fSpeed;
            //    //m_uiLastAdvance = round_int(advance);
            //    m_PassedTime += std::rint(advance * 1000.0f);
            //    return advance;
            //}
            //unsigned advance() const { return m_uiLastAdvance; }

            void speed(float s) {
                m_fSpeed = s;
            }
            float speed() const {
                return m_fSpeed;
            }
            void pause() {
                m_fSpeed = 0.0f;
            }
            void resume(float speed = 1.0f) {
                m_fSpeed = speed;
            }
            void reset() {
                m_PassedTime = 0L;
                m_fSpeed = 1.0f;
            }
    };

    // eventually migrate to chrono
    class Alarm
    {
    protected:
    
        //Freq* m_pTimer;
        Timeline* m_pTimer;
        unsigned m_AlarmTime;
        unsigned m_StartTime;

        //todo:add alarm pausing?
    
    public:
    
        //Alarm():
        //    m_AlarmTime(0L),
        //    m_StartTime(0L),
        //    m_pTimer(Freq::get().accumulator())
        //{
        //    assert(m_pTimer);
        //}

        //static Freq::Alarm never(Timeline* timer) { 
        //    return Freq::Alarm(
        //        Freq::Time::never(),
        //        timer
        //    );
        //}
        explicit Alarm(Timeline* timer):
            m_AlarmTime(0L),
            m_StartTime(0L),
            m_pTimer(timer)
        {
            assert(m_pTimer);
        }

        explicit Alarm(Time t, Timeline* timer):
            m_pTimer(timer)
            //m_pTimer(timer ? timer : Freq::get().accumulator())
        {
            assert(m_pTimer);
            set(t);
        }

        virtual ~Alarm() {}

        void stop() {
            m_AlarmTime = m_StartTime;
        }

        Alarm& operator+=(Freq::Time t) {
            m_AlarmTime += t.ms();
            return *this;
        }

        bool hasTimer() const { return (m_pTimer!=NULL); }
        
        void timer(Timeline* timerRef)
        {
            assert(timerRef);
            m_pTimer = timerRef;
        }
        const Timeline* timer() const { return m_pTimer; }
        Timeline* timer() { return m_pTimer; }
        
        void set(Time t)
        {
            assert(m_pTimer);
            m_StartTime = m_pTimer->ms();
            m_AlarmTime = m_StartTime + t.ms();
        }

        /*
         * Sets an alarm time minimum/floor
         *
         * Returns whether or not alarm time was changed (bool)
         */
        bool floor(Time t)
        {
            assert(m_pTimer);
            Time r = remaining();
            if(t > r) {
                set(t);
                return true;
            }
            return false;
        }
        /*
         * Sets an alarm time maximum/ceiling
         *
         * Returns whether or not alarm time was changed (bool)
         */
        bool ceiling(Time t)
        {
            assert(m_pTimer);
            Time r = remaining();
            if(t < r) {
                set(t);
                return true;
            }
            return false;
        }

        void delay(Time t) {
            assert(m_pTimer);
            m_AlarmTime += t.ms();
        }

        Freq::Time pause() {
            return Freq::Time(m_AlarmTime - m_StartTime);
        }

        void minutes(unsigned m)
        {
           set(Time::minutes(m));
        }
        
        void seconds(unsigned s)
        {
           set(Time::seconds(s));
        }

        void ms(unsigned ms)
        {
           set(Time(ms));
        }
        
        Freq::Time time() const {
        }

        unsigned ms() const
        {
            assert(m_pTimer);
            if(!elapsed())
            {
                unsigned t = (m_AlarmTime - m_pTimer->ms());
                return t;
            }
            return 0L;
        }
        Freq::Time remaining() const {
            return Freq::Time(ms());
        }
        
        float seconds() const
        {
            assert(m_pTimer);
            float t = (m_AlarmTime - m_pTimer->ms()) / 1000.0f;
            return t;
        }
        
        bool elapsed() const
        {
            assert(m_pTimer);
            return (m_pTimer->ms() >= m_AlarmTime);
        }
        
        float fraction() const
        {
            return 1.0f - fraction_left();
        }

        Freq::Time excess() const {
            if(!elapsed())
                return Freq::Time(0);
            return Freq::Time(m_pTimer->ms() - m_AlarmTime);
        }

        float fraction_left() const
        {
            if(elapsed())
                return 0.0f;

            unsigned remaining = ms();
            unsigned range = m_AlarmTime - m_StartTime;
            if(range == 0UL)
                return 0.0f;
            return (float)remaining / (float)range;
        }

        //unsigned endTickTime() { return m_AlarmTime; }
    };

private:

    Timeline m_globalTimeline;
    //unsigned m_uiLastMark;
    unsigned m_StartTime;
    unsigned m_uiMinTick;
    
public:

    Freq():
        m_StartTime(get_ticks()),
        m_uiMinTick(0)
    {}
    
    unsigned get_ticks() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        ).count();
        //return SDL_GetTicks();
    }
    
    // TODO: not yet implemented
    //void pause();
    //void unpause();
    
    //bool tick();

    // accumulates time passed, until enough has passed to advance forward
    // may advance more than 1 frame's worth of time
    // returns # of ms to advance
    Freq::Time tick() {
        unsigned ticks = get_ticks() - m_StartTime;
        unsigned advance = (unsigned)(ticks - m_globalTimeline.ms());
        if(advance >= m_uiMinTick) {
            m_globalTimeline.logic(Freq::Time::ms(advance));
            return Freq::Time::ms(advance);
        }
        return Freq::Time::ms(0);
    }

    Timeline* timeline() { return &m_globalTimeline; }
};

#endif

