#include <SFML/Graphics.hpp> 
#include <stdlib.h> 
#include <time.h> 

using namespace sf;

const int W = 296;
const int H = 167;

int ground = 600;
bool go = true;
int prop = 0;

class Fruits
{
public:
    Sprite sprite, res;
    int nom, t, fly;
    FloatRect rect;
    float dy, rotate;
    bool onGround, cel;

    void set(Texture& image, int x, int y)
    {
        sprite.setTexture(image);
        res.setTexture(image);

        rect.left = x + 50;
        rect.top = y;

        nom = rand() % 7;
        t = 0;
        dy = -0.7;
    }

    void update()
    {
        if (fly == 1) {
            if (!onGround)
            {
                dy = dy + 0.0005;
            }
        }
        rect.top += dy;

        onGround = false;
        if (rect.top > ground)
        {
            rect.top = ground;
            dy = 0;

            onGround = true;

            if (t == 0)
            {
                t = 1000;
            }
        }

        if (t > 0)
        {
            t--;

            if (t == 0)
            {
                dy = -0.7;
                nom = rand() % 7;
                cel = true;

                go = true;
            }
        }

        if (cel && nom < 6 && rect.top > ground - 1 && dy > 0)
        {
            prop++;
        }
        sprite.setOrigin(50, 50);
        sprite.setRotation(rotate);
        res.setOrigin(50, 50);
        res.setRotation(rotate);
        rotate += 0.25;

        if (cel)
        {
            sprite.setTextureRect(IntRect(100 * nom, 0, 100, 100));
        }
        else
        {
            sprite.setTextureRect(IntRect(100 * nom, 100, 100, 60));
        }

        sprite.setTextureRect(IntRect(100 * nom, 0, 100, 100));
        sprite.setPosition(rect.left, rect.top);

        res.setTextureRect(IntRect(100 * nom, 100, 100, 50));
        res.setPosition(rect.left + 50, rect.top);
    }

};

class Score
{
public:
    Sprite sec[3];
    int s[3], c;
    bool vid[3];

    Score(Texture& image)
    {
        for (int i = 0; i < 3; i++)
        {
            sec[i].setTexture(image);

            sec[i].setPosition(100 + 22 * i, 25);

            sec[i].setTextureRect(IntRect(0, 0, 22, 45));

            s[i];
            c = 0;
        }
    }


    void update()
    {
        if (c < 10)
        {
            vid[0] = true;
            vid[1] = false;
            vid[2] = false;
        }
        else if (c >= 10 && c < 100)
        {
            s[0] = c / 10;
            s[1] = c % 10;

            vid[0] = true;
            vid[1] = true;
            vid[2] = false;
        }
        else if (c >= 100 && c < 1000)
        {
            s[0] = c / 100;
            s[1] = (c / 10) % 10;
            s[2] = (c % 100) % 10;

            vid[0] = true;
            vid[1] = true;
            vid[2] = true;
        }

        for (int i = 0; i < 3; i++)
        {
            sec[i].setTextureRect(IntRect(22 * s[i], 0, 22, 45));
        }
    }

};

void display()
{
    srand(time(0));
    RenderWindow window(VideoMode(888, 501), "Fruit Ninja");

    Texture f;
    f.loadFromFile("fon.png");
    Sprite fon(f);

    CircleShape strel;
    strel.setRadius(3.f);
    strel.setFillColor(Color::Green);
    bool vid[W][H];
    int time[W][H];
    for (int i = 0; i < W; i++)
        for (int j = 0; j < H; j++) {
            vid[i][j] = false;
            time[i][j] = 0;
        }

    Texture f2;
    f2.loadFromFile("fruits.png");
    Fruits fruit[8];
    for (int i = 0; i < 8; i++)
    {
        fruit[i].set(f2, 110 * i, ground);
    }
    int pusk = 100;

    Sprite ArbScore(f2);
    ArbScore.setTextureRect(IntRect(200, 100, 100, 60));
    ArbScore.setScale(0.8, 0.8);
    ArbScore.setPosition(0, 60);
    ArbScore.setRotation(315);

    Texture sc;
    sc.loadFromFile("watermelon.png");
    Score score(sc);

    Texture cr;
    cr.loadFromFile("Red - Cross - Mark - Download - PNG.png");
    Sprite cross[3];
    for (size_t i = 0; i < 3; i++)
    {
        cross[i].setTexture(cr);
        cross[i].setTextureRect(IntRect(0, 0, 50, 50));
        cross[i].setPosition(675 + 75 * i, 25);
    }

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);

        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        for (int i = 0; i < W; i++)
            for (int j = 0; j < H; j++)
            {
                if (pos.x >= i * 6 && pos.x <= (i + 1) * 6 &&
                    pos.y >= j * 6 && pos.y <= (j + 1) * 6)
                {
                    vid[i][j] = true;
                    time[i][j] = 100;
                }

                if (time[i][j] > 0)
                {
                    time[i][j]--;

                    if (time[i][j] == 0)
                        vid[i][j] = false;
                }

            }

        if (go)
        {
            for (int i = 0; i < 8; i++)
            {
                fruit[i].fly = rand() % 2 + 1;

                fruit[i].rect.left = 5 + 110 * i + 50;
            }

            go = false;
        }

        if (pusk > 0)
        {
            pusk--;
        }

        for (int i = 0; i < 8; i++)
        {
            if (fruit[i].sprite.getGlobalBounds().contains(pos.x, pos.y) &&
                fruit[i].cel && pusk == 0)
            {
                if (fruit[i].nom < 6)
                {
                    fruit[i].cel = false;

                    score.c++;

                    fruit[i].rect.left -= 50;
                }
            }
        }

        if (prop > 3)
        {
            prop = 3;
        }

        for (int i = 0; i < prop; i++)
        {
            cross[i].setTextureRect(IntRect(50, 0, 50, 50));
        }
        for (int i = 0; i < 8; i++)
        {
            fruit[i].update();
        }
        score.update();
        window.clear(Color::White);
        window.draw(fon);
        for (int i = 0; i < W; i++)
            for (int j = 0; j < H; j++)
            {
                strel.setPosition(i * 6, j * 6);

                if (vid[i][j])
                    window.draw(strel);
            }
        for (int i = 0; i < 8; i++)
        {
            window.draw(fruit[i].sprite);

            if (!fruit[i].cel)
            {
                window.draw(fruit[i].res);
            }
        }
        window.draw(ArbScore);
        for (int i = 0; i < 3; i++)
        {
            if (score.vid[i])
            {
                window.draw(score.sec[i]);
            }
        }

        for (int i = 0; i < 3; i++)
        {
            window.draw(cross[i]);
        }

        window.display();
    }
}