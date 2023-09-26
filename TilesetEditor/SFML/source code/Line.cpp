#include "stdafx.h"
#include "Line.h"

Line::Line(sf::Vector2f Point1, sf::Vector2f Point2, float Thickness,sf::Color Color)
{
    sf::Vector2f origin = Point2 - Point1;

    sf::Vector2f normal = origin.toUnit();
    sf::Vector2f n1 = { -normal.y, normal.x };
    sf::Vector2f n2 = { normal.y, -normal.x };
    //storing the all 4 points that form the line
    sf::Vector2f Points[4];
    Points[0] = Point1 + Thickness * n1;
    Points[1] = Point1 + Thickness * n2;
    Points[2] = Points[1] + origin;
    Points[3] = Points[0] + origin;

    m_vert.setPrimitiveType(sf::Quads);
    m_vert.resize(4);

    for (size_t i = 0; i < 4; i++)
    {
        m_vert[i].color = std::move(Color);
        m_vert[i].position = Points[i];
    }
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_vert, states);
}
