#include "LinesNode.hpp"

bool LinesNode::init(std::vector<CCPoint> points, ccColor4F color) {
    if (!CCDrawNode::init()) return false;

    m_currentPoints = points;
    m_color = color;

    this->redraw();
    this->scheduleUpdate();
    
    return true;
}

void LinesNode::update(float delta) {
    if (!m_isAnimating) return;

    m_animationTime += delta;
    float progress = m_animationTime / m_animationDuration;

    if (progress >= 1.f) {
        progress = 1.f;
        m_isAnimating = false;
    }

    float ease = easeInOut(progress);

    for (int i = 0; i < m_currentPoints.size(); i++) {
        m_currentPoints[i] = m_startPoints[i] + m_deltaPoints[i] * ease;
    }

    this->redraw();
}

void LinesNode::redraw() {
    this->clear();

    for (int i = 0; i < m_currentPoints.size() - 1; i++) {
        auto p1 = m_currentPoints[i];
        auto p2 = m_currentPoints[i + 1];

        this->drawSegment(p1, p2, m_thickness, m_color);
    }
}

LinesNode* LinesNode::create(std::vector<CCPoint> points, ccColor4F color) {
    auto ret = new LinesNode();
    if (ret->init(points, color)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}