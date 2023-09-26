#pragma once

//A class the helps with resizing and rescaling for an sf::Sprite
class ResizableSprite : public sf::Drawable
{
public:
    //gets the actual reference to the sprite
    sf::Sprite& get();
    //returns the bounds of the sprite
    sf::FloatRect getBounds() const;
    //sets the size of the sprite
    void setSize(sf::Vector2f size);
    //gets the size of the sprite
    sf::Vector2f getSize() const;
    //gets the position of the sprite
    sf::Vector2f getPosition() const;
    //sets the position of the sprite
    void setPosition(sf::Vector2f position);
    //sets the texture. 
    //It also resizes the sprite back from 
    //the texture size to the actual size we have specified
    void setTexture(const sf::Texture& texture, bool resetRect = false);
    //sets the texture rect
    void setTextureRect(const sf::IntRect& rect);
    //returns true if the sprite intersects the given rect false otherwise
    bool intersects(sf::FloatRect rect) const;
    //return true if the sprite intersects the rect of the given sprite false otherwise
    bool intersects(const ResizableSprite& sprite);
    //returns true if the rect of the sprite cotains the given position
    bool contains(sf::Vector2f position);
    //set the origin factor.
    //0.5f, 0.5f would be the same as:
    //setOrigin(0.5f * size.x, 0.5f * size.y)
    void setOriginFactor(sf::Vector2f factor);
    //draws the sprite to the target
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    //the actual sprite
    sf::Sprite m_sprite;
    //stores the size we have provided for resizing it back to this size after setting the texture
    sf::Vector2f m_size;

private:
    //resets the size of the sprite with the 'm_size'
    void resetSize();
};