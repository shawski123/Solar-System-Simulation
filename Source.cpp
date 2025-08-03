#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <string>

const float DEFAULT_SPEED = 0.0025f;

class Planet {
private:
    sf::Sprite& sprite;
    sf::Vector2f offset;
    sf::Vector2f position;
    sf::Vector2f scale;
    float orbitSpeed;
    float orbitRadius;
public:
    Planet(sf::Sprite& newSprite, sf::Vector2f newOffset, sf::Vector2f newPosition, float newOrbitSpeed, float newOrbitRadius, sf::Vector2f newScale) : sprite(newSprite), offset(newOffset),
        position(newPosition), orbitSpeed(newOrbitSpeed), orbitRadius(newOrbitRadius), scale(newScale) {
        sprite.setScale(scale);
        sprite.setPosition(position);
        sprite.setOrigin({ 32.f / 2, 32.f / 2 });
    }

    void setOrigin(sf::Vector2f origin) {
        sprite.setOrigin(origin);
    }

    sf::Vector2f getOffset() const {
        return offset;
    }

    sf::Vector2f& getScale() {
        return scale;
    }

    void setScale(sf::Vector2f scaleSet) {
        scale = scaleSet;
        sprite.setScale(scale);
    }

    void setOffset(sf::Vector2f newOffset) {
        offset = newOffset;
    }

    float& getSpeed() {
        return orbitSpeed;
    }

    void setSpeed(float speed) {
        orbitSpeed = speed;
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    void setPosition(sf::Vector2f newPos) {
        position = newPos;
        sprite.setPosition(position);
    }

    float getRadius() {
        return orbitRadius;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};

void orbitSun(const Planet& sun, Planet& planet);

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "Our Solar System");
    window.setFramerateLimit(144);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    std::vector<std::unique_ptr<sf::Music>> soundtrack;

    //Background
    sf::Texture backText;
    backText.loadFromFile("resources/spacesky1.png");
    sf::Sprite backGround(backText);
    backGround.setOrigin({ 960, 540 });
    backGround.setPosition({ 960,540 });
    backGround.setScale({ 1,1 });

    //Audio
    auto music = std::make_unique<sf::Music>();
    if (!music->openFromFile("resources/spacemusic.mp3")) return 1;
    music->setLooping(true);
    music->setPlayingOffset(sf::seconds(1));
    music->play();
    soundtrack.push_back(std::move(music));

    auto music2 = std::make_unique<sf::Music>();
    if (!music2->openFromFile("resources/spacemusic2.mp3")) return 1;
    music2->setLooping(true);
    soundtrack.push_back(std::move(music2));

    //Sun
    sf::Texture sunText;
    sunText.loadFromFile("resources/soleil.png");
    sf::Sprite spriteSun(sunText);
    Planet sun(spriteSun, {}, { 960, 540 }, 0, 0, { 1,1 });

    //Mercury
    sf::Texture mercuryText("resources/mercure.png");
    sf::Sprite spriteMercury(mercuryText);
    Planet mercury(spriteMercury, { 75,0 }, { }, DEFAULT_SPEED, 75, { 0.5,0.5 });

    //Venus
    sf::Texture venusText("resources/venus.png");
    sf::Sprite spriteVenus(venusText);
    Planet venus(spriteVenus, { 150,0 }, { }, DEFAULT_SPEED, 150, { 1,1 });

    //Earth
    sf::Texture earthText("resources/terre.png");
    sf::Sprite spriteEarth(earthText);
    Planet earth(spriteEarth, { 225,0 }, {}, DEFAULT_SPEED, 225, { 1,1 });

    //Mars
    sf::Texture marsText("resources/mars.png");
    sf::Sprite spriteMars(marsText);
    Planet mars(spriteMars, { 300,0 }, {}, DEFAULT_SPEED, 300, { 0.75,0.75 });

    //Jupiter
    sf::Texture jupiterText("resources/jupiter.png");
    sf::Sprite spriteJupiter(jupiterText);
    Planet jupiter(spriteJupiter, { 375,0 }, {}, DEFAULT_SPEED, 375, { 1.5,1.5 });

    //Saturn
    sf::Texture saturnText("resources/saturne.png");
    sf::Sprite spriteSaturn(saturnText);
    Planet saturn(spriteSaturn, { 450,0 }, {}, DEFAULT_SPEED, 450, { 1.5,1.5 });
    saturn.setOrigin({ 83.f / 2, 41.f / 2 });

    //Uranus
    sf::Texture uranusText("resources/uranus.png");
    sf::Sprite spriteUranus(uranusText);
    Planet uranus(spriteUranus, { 525,0 }, {}, DEFAULT_SPEED, 525, { 1,1 });

    //Neptune
    sf::Texture neptuneText("resources/neptune.png");
    sf::Sprite spriteNeptune(neptuneText);
    Planet neptune(spriteNeptune, { 600,0 }, {}, DEFAULT_SPEED, 600, { 1,1 });

    float zoom = 1.0f;

    int musicIndex = 0;
    const int songAmount = 2;

    sf::Vector2f parallaxOffset({ 0,0 });

    float planetSpeed = DEFAULT_SPEED;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        std::vector<Planet*> planets = { &sun,&mercury, &venus, &earth, &mars, &jupiter, &saturn, &uranus, &neptune };

        ImGui::Begin("Configuration");
        ImGui::SeparatorText("Change speed");
        if (ImGui::CollapsingHeader("Change individual speed")) {
            if (ImGui::CollapsingHeader("Mercury")) {
                ImGui::SliderFloat("Set speed##Mercury", &mercury.getSpeed(), 0, 1.f);
            }

            if (ImGui::CollapsingHeader("Venus")) {
                ImGui::SliderFloat("Set speed##Venus", &venus.getSpeed(), 0, 1.f);
            }

            if (ImGui::CollapsingHeader("Earth")) {
                ImGui::SliderFloat("Set speed##Earth", &earth.getSpeed(), 0, 1.f);
            }

            if (ImGui::CollapsingHeader("Mars")) {
                ImGui::SliderFloat("Set speed##Mars", &mars.getSpeed(), 0, 1.f);
            }

            if (ImGui::CollapsingHeader("Jupiter")) {
                ImGui::SliderFloat("Set speed##Jupiter", &jupiter.getSpeed(), 0, 1.f);
            }

            if (ImGui::CollapsingHeader("Saturn")) {
                ImGui::SliderFloat("Set speed##Saturn", &saturn.getSpeed(), 0, 1.f);
            }

            if (ImGui::CollapsingHeader("Uranus")) {
                ImGui::SliderFloat("Set speed##Uranus", &uranus.getSpeed(), 0, 1.f);
            }

            if (ImGui::CollapsingHeader("Neptune")) {
                ImGui::SliderFloat("Set speed##Neptune", &neptune.getSpeed(), 0, 1.f);
            }

        }

        ImGui::Separator();

        ImGui::Text("Set speed for all planets");
        if (ImGui::SliderFloat("", &planetSpeed, 0, 1.f)) {
            for (Planet* planet : planets) {
                planet->setSpeed(planetSpeed);
            }
        }

        if (ImGui::Button("Normalize speeds")) {
            planetSpeed = DEFAULT_SPEED;
            for (Planet* planet : planets) {
                planet->setSpeed(DEFAULT_SPEED);
            }
        }

        ImGui::SeparatorText("Zoom");

        ImGui::SliderFloat("Zoom in/out", &zoom, 0, 4.f);

        float rawOffset = std::clamp((1.f - zoom) * 100.f, -150.f, 150.f);
        parallaxOffset = { rawOffset, rawOffset };

        backGround.setPosition({960 + parallaxOffset.x, 540 + parallaxOffset.y });

        for (Planet* planet : planets) {
            planet->setScale({ zoom,zoom });
        }

        if (ImGui::Button("Reset zoom")) {
            for (Planet* planet : planets) {
                planet->setScale({ 1.f,1.f });
                zoom = 1.f;
            }

            backGround.setScale({ 1.f,1.f });
        }

        ImGui::SeparatorText("Other settings");

        if (ImGui::Button("Pause")) {
            for (Planet* planet : planets) {
                planet->setSpeed(0);
            }
        }


        if (ImGui::Button("Change ambience")) {
            musicIndex++;

            if (musicIndex > songAmount - 1) {
                musicIndex = 0;
            }

            std::cout << musicIndex << std::endl;

            switch (musicIndex) {
            case 0:
                soundtrack[0]->play();
                soundtrack[1]->stop();
                soundtrack[0]->setPlayingOffset(sf::seconds(1));
                break;
            case 1:
                soundtrack[1]->play();
                soundtrack[0]->stop();
                break;
            }
        }

        ImGui::End();

        orbitSun(sun, mercury);
        orbitSun(sun, venus);
        orbitSun(sun, earth);
        orbitSun(sun, mars);
        orbitSun(sun, jupiter);
        orbitSun(sun, saturn);
        orbitSun(sun, uranus);
        orbitSun(sun, neptune);

        window.clear();
        window.draw(backGround);
        sun.draw(window);
        mercury.draw(window);
        venus.draw(window);
        earth.draw(window);
        mars.draw(window);
        jupiter.draw(window);
        saturn.draw(window);
        uranus.draw(window);
        neptune.draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }
}


void orbitSun(const Planet& sun, Planet& planet) {

    sf::Vector2f offset = planet.getOffset();
    float speed = planet.getSpeed();
    float fixedRad = planet.getRadius();

    float newX = offset.x - speed * offset.y;
    float newY = offset.y + speed * offset.x;

    offset = { newX, newY };

    float length = std::sqrt(newX * newX + newY * newY);
    if (length != 0) {
        offset /= length;
        offset *= fixedRad;
    }

    planet.setOffset(offset);
    planet.setPosition(sun.getPosition() + offset);
}



