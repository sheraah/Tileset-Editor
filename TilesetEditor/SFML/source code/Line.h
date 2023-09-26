#pragma once

//a basic line class. You can contruct it right in the draw function parameters.
class Line : public sf::Drawable
{
public:
    //contructor to initialize all properties
    Line(sf::Vector2f Point1, sf::Vector2f Point2, float Thickness, sf::Color Color);
    //drwaing the line to the target
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    //the vertex array for the line
    sf::VertexArray m_vert;
};

