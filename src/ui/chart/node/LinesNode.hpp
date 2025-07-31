#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class LinesNode : public CCDrawNode {
    protected:
        std::vector<CCPoint> m_targetPoints;
        std::vector<CCPoint> m_startPoints;
        std::vector<CCPoint> m_deltaPoints;
        std::vector<CCPoint> m_currentPoints;

        bool m_isAnimating;
        float m_animationTime;
        float m_animationDuration;

        float m_thickness = .5f;
        ccColor4F m_color;

        bool init(std::vector<CCPoint> points, ccColor4F color);
        void redraw();

        float easeInOut(float t) {
            return t * t * (3.f - 2.f * t);
        }
    public:
        static LinesNode* create(std::vector<CCPoint> points, ccColor4F color);

        void update(float delta) override;
        
        void setThickness(float thickness) {
            m_thickness = thickness;
        }

        void animateTo(std::vector<CCPoint> points, float duration) {
            m_targetPoints = points;
            m_startPoints = m_currentPoints;

            m_deltaPoints.clear();
            for (int i = 0; i < m_currentPoints.size(); i++) {
                m_deltaPoints.push_back(m_targetPoints[i] - m_startPoints[i]);
            }

            m_isAnimating = true;
            m_animationTime = 0.f;
            m_animationDuration = duration;
        }

        void snapTo(std::vector<CCPoint> points) {
            m_currentPoints = points;
            this->redraw();
        }
};