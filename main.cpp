#include <simplecpp>
#include "constants.h"
#include "shooter.h"
#include "bubble.h"
// #include "constants.h"

// /* Simulation Vars */
// const double STEP_TIME = 0.02;

// /* Game Vars */
// //const int PLAY_Y_HEIGHT = 450;
// const int LEFT_MARGIN = 70;
// //const int TOP_MARGIN = 20;
// const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
        bubbles[i].updateVelocity(STEP_TIME);
    }
}

vector<Bubble> create_bubbles()
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, -BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, BUBBLE_DEFAULT_RADIUS, BUBBLE_DEFAULT_VX, 0, COLOR(255,105,180)));
    return bubbles;
}

// void CollisionWithBullets(vector<Bubble> &bubbles, vector<Bullet> &bullets)
// {
//     for(int i = 0; i < int(bubbles.size()); i++)
//     {
//         for(int j = 0; j < int(bullets.size()); j++)
//         {
//             if((bubbles[i].get_center_x() - bullets[j].get_center_x())*(bubbles[i].get_center_x() - bullets[j].get_center_x()) + 
//             (bubbles[i].get_center_y() - bullets[j].get_center_y())*(bubbles[i].get_center_y() - bullets[j].get_center_y()) < BUBBLE_DEFAULT_RADIUS + 7.05 + 100)
//             {
//                 bubbles.erase(bubbles.begin() + i);
//                 bullets.erase(bullets.begin() + j);
//             }
//         }
//     }
// }

void afterCollision(vector<Bubble> &bubbles, vector<Bullet> &bullets, int i, int j)
{
    if(bubbles[i].get_radius() < 20.0)
    {
        bubbles.erase(bubbles.begin() + i);
        bullets.erase(bullets.begin() + j);
    }
    else
    {
        //bubbles.erase(bubbles.begin() + i);
        //bullets.erase(bullets.begin() + j);
        bubbles.push_back(Bubble(bubbles[i].get_center_x() + bubbles[i].get_radius()/2, bubbles[i].get_center_y(), bubbles[i].get_radius()/2, BUBBLE_DEFAULT_VX, 0, COLOR(bubbles[i].get_radius()*50, 0, 220)));
        bubbles.push_back(Bubble(bubbles[i].get_center_x() - bubbles[i].get_radius()/2, bubbles[i].get_center_y(), bubbles[i].get_radius()/2, -BUBBLE_DEFAULT_VX, 0, COLOR(bubbles[i].get_radius()*50, 0, 220)));
        bubbles.erase(bubbles.begin() + i);
        bullets.erase(bullets.begin() + j);
    }
}

void collisionWithBullet(vector<Bubble> &bubbles, vector<Bullet> &bullets)
{
    //bool isColliding = false;
    for(int i = 0; i< int(bubbles.size()); i++)
    {
        for(int j = 0; j< int(bullets.size()); j++)
        {
            if(abs(bubbles[i].get_center_x() - bullets[j].get_center_x()) < bubbles[i].get_radius() + bullets[j].get_width()/2 
            && abs(bubbles[i].get_center_y() - bullets[j].get_center_y()) < bubbles[i].get_radius() + bullets[j].get_height()/2)
            {
                afterCollision(bubbles, bullets, i, j);
            }
        }
    }
}

bool collisionWithShooter(vector<Bubble> &bubbles, Shooter shooter)
{
    for(int i = 0; i < int(bubbles.size()); i++)
    {
        if((abs(bubbles[i].get_center_x()- shooter.get_body_center_x()) < bubbles[i].get_radius() + shooter.get_body_width()/2) && 
        (abs(bubbles[i].get_center_y() - (shooter.get_body_center_y() + shooter.get_head_center_y())/2) < bubbles[i].get_radius() + (shooter.get_body_height()/2 + 2*shooter.get_head_radius())/2) )
        {
            return false;
        }
        else return true;
    }
}

void updateLife(int &life)
{
    
}

int main()
{
    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));
    
    string msg_cmd("Cmd: _");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles();

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {   
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);
            
            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }
        
        //Check Collision
        collisionWithBullet(bubbles, bullets);

        // Update the bubbles
        move_bubbles(bubbles);

        // Update the bullets
        move_bullets(bullets);

        // collisionWithShooter(bubbles, shooter);
        if(!collisionWithShooter(bubbles, shooter))
        {
            Text result(100, 100, "YOU LOST");
            result.imprint();
            break;
        }

        if(bubbles.size() == 0)
        {
            // Text result(100, 100, "WIN");
            // result.imprint();
            // break;
            int life;
            updateLife(&life);
        }

        wait(STEP_TIME);
    }

    getClick();
}
