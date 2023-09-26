#include "stdafx.h"
#include "ResizableSprite.h"

sf::Sprite& ResizableSprite::get()
{
    return m_sprite;
}

sf::FloatRect ResizableSprite::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

void ResizableSprite::setSize(sf::Vector2f size)
{
    m_size = size;
    m_sprite.setScale({ 1.f, 1.f });
    resetSize();
}

sf::Vector2f ResizableSprite::getSize() const
{
    return m_size;
}

sf::Vector2f ResizableSprite::getPosition() const
{
    return m_sprite.getPosition();
}

void ResizableSprite::setPosition(sf::Vector2f position)
{
    m_sprite.setPosition(position);
}

void ResizableSprite::setTexture(const sf::Texture& texture, bool resetRect)
{
    m_sprite.setTexture(texture, resetRect);

    resetSize();
}

void ResizableSprite::setTextureRect(const sf::IntRect& rect)
{
    m_sprite.setTextureRect(rect);

    resetSize();
}

bool ResizableSprite::intersects(sf::FloatRect rect) const
{
    return getBounds().intersects(rect);
}

bool ResizableSprite::intersects(const ResizableSprite& sprite)
{
    return intersects(sprite.getBounds());
}

bool ResizableSprite::contains(sf::Vector2f position)
{
    return getBounds().contains(position);
}

void ResizableSprite::setOriginFactor(sf::Vector2f factor)
{
    m_sprite.setOrigin(factor.x * m_sprite.getTexture()->getSize().x, factor.y * m_sprite.getTexture()->getSize().y);
}

void ResizableSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}

void ResizableSprite::resetSize()
{
    const sf::Texture* texture = m_sprite.getTexture();

    if (texture == nullptr) return;
    //set the sprite scale to that the size is m_size
    m_sprite.setScale(
        m_size.x / texture->getSize().x / m_sprite.getTextureRect().width * texture->getSize().x,
        m_size.y / texture->getSize().y / m_sprite.getTextureRect().height * texture->getSize().y
    );
}
