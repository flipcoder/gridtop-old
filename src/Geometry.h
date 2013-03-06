#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <glm/glm.hpp>

struct Rectangle
{
    public:

        Rectangle():
            m_x(0),
            m_y(0),
            m_w(1),
            m_h(1)
        {}

        Rectangle(
            int x,
            int y,
            int w,
            int h
        ):
            m_x(x),
            m_y(y),
            m_w(w),
            m_h(h)
        {}

        Rectangle(
            float x,
            float y,
            float w,
            float h
        ):
            m_x(std::rint(x)),
            m_y(std::rint(y)),
            m_w(std::rint(w)),
            m_h(std::rint(h))
        {}

        template<class T>
        static Rectangle xywh(
            T x,
            T y,
            T w,
            T h
        ){
            return Rectangle(x,y,w,h);
        }

        template<class T>
        static Rectangle ltrb(
            T left,
            T top,
            T right,
            T bottom
        ){
            return Rectangle(left, top, right-left, bottom-top);
        }

        bool contains(const glm::ivec2& p) const {
            return p.x >= m_x && p.x < m_x+m_w &&
                   p.y >= m_y && p.y < m_y+m_h;
        }

        bool contains(const glm::vec2& p) const {
            return contains(glm::vec2(std::rint(p.x), std::rint(p.y)));
        }

        bool overlaps(const Rectangle& b) const {
            if((m_x > b.x() + b.w() - 1)  ||
               (m_y > b.y() + b.h() - 1)  ||
               (b.x() > m_x + m_w - 1)||
               (b.y() > m_y + m_h - 1)
            ){
                return false;
            }
            return true;
        }

        GETSET(int, x, m_x);
        GETSET(int, y, m_y);
        GETSET(int, w, m_w);
        GETSET(int, h, m_h);

    private:
        int m_x, m_y, m_w, m_h;
};

#endif

