#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <stdlib.h> 
#include <time.h>
#include <vector>


using namespace sf;

const float Gravitation_const = 0;
const int window_size = 1000;
const int objects_quantity = 4000;
const int splitting = 10;
const float chance = 0.5;
const float max_speed = 0.5;

int Box(Vector2f vector){
     
    return int(vector.x / (window_size / splitting)) + splitting * int(vector.y / (window_size / splitting));
}


class corpuscle{
private:
    Vector2f position; 

public:
    float size; 
    float mass;
    float charge;
    int box = 0;
    
    Vector2f speed = Vector2f(0, 0);
    CircleShape shape;

    void init(float Size, float Mass, float Charge, Vector2f Position, Vector2f Speed, Color color){
        size = Size;
        mass = Mass;
        charge = Charge;

        position = Position;
        speed = Speed;

        shape = CircleShape(size);
        shape.setFillColor(color);
        shape.setPosition(Position - Vector2f(size/sqrt(2), size/sqrt(2)));
    }
    void setPosition(Vector2f Position){
        position = Position;
        shape.setPosition(Position - Vector2f(size/sqrt(2), size/sqrt(2)));
        box = Box(Position);
    }
    
    Vector2f getPosition(){
        return position;
    }

};


float RandomFloat(float min, float max){
    int precision = 4;
    
    float value;

    value = rand() % (int)pow(10, precision);
    value = min + (value / pow(10, precision)) * (max - min);

    return value;
}




Vector2f operator* (float scalar, Vector2f vector){
    return Vector2f(vector.x * scalar, vector.y * scalar);
}
Vector2f operator* (Vector2f vector, float scalar){
    return Vector2f(vector.x * scalar, vector.y * scalar);
}



int main()
{
    srand(time(NULL));
    

    RenderWindow model_window(VideoMode(window_size, window_size), "Simple model");
    
    corpuscle objects[objects_quantity];

    RectangleShape vertical[splitting];
    for(int i=0; i<splitting; i++){
        RectangleShape line(Vector2f(5, window_size));
        line.rotate(-90);
        line.setPosition(Vector2f(0, window_size / splitting * (i+1)));
        vertical[i] = line;
    }

    RectangleShape horisontal[splitting];
    for(int i=0; i<splitting; i++){
        RectangleShape line(Vector2f(5, window_size));
        line.setPosition(Vector2f(window_size / splitting * (i + 1), 0));
        horisontal[i] = line;
    }
    
    bool percolation[splitting*splitting][splitting*splitting];

    for(int i = 0; i<splitting*splitting; i++){
        for(int j=0; j < splitting*splitting; j++){
            
            percolation[i][j] = true;
            percolation[j][i] = true;

            if (i == j){
                percolation[i][j] = false;
                percolation[j][i] = false;  
            }
            if((i - j == 1 or i - j == -1 or i - j == 10 or i - j == -10) and (RandomFloat(0, 1) <= chance)){
                percolation[i][j] = false;    
                percolation[j][i] = false;  
            }
            
            
        }
    }

    std::cout << percolation[0][10] << percolation[10][10] << '\n';


    for(int i=0; i<objects_quantity; i++){
        
        corpuscle e; 
        e.init(5, 5, -1, Vector2f(RandomFloat(20, 80), RandomFloat(520, 580)), Vector2f(RandomFloat(-max_speed, max_speed), RandomFloat(-max_speed, max_speed)), Color::Blue);
        objects[i] = e;     
    } 

    while (model_window.isOpen())
    {
        Event event;
        while (model_window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                model_window.close();
        }

        model_window.clear();

        

        
        for(int i=0; i<objects_quantity; i++){
            

            Vector2f old_Position = objects[i].getPosition();

            Vector2f new_Position = old_Position + objects[i].speed;

            int new_box = Box(new_Position);
            //std::cout << new_box;
            
            if(percolation[objects[i].box][new_box]){
        
                new_Position = old_Position;    

                // if (objects[i].box - new_box == 1 or objects[i].box - new_box == -1 or objects[i].box - new_box == 10 or objects[i].box - new_box == -10 or objects[i].box - new_box == 12 or objects[i].box - new_box == -12){
                //     objects[i].speed.x = objects[i].speed.x * -1;
                // } 
                // if (objects[i].box - new_box == 11 or objects[i].box - new_box == -11 or objects[i].box - new_box == 10 or objects[i].box - new_box == -10 or objects[i].box - new_box == 12 or objects[i].box - new_box == -12){
                //     objects[i].speed.y = objects[i].speed.y * -1;
                // }

                if (new_box - objects[i].box == -9 or new_box - objects[i].box == 1 or new_box - objects[i].box == 11){
                    objects[i].speed.x = objects[i].speed.x * -1;
                    //new_Position.x = (new_box % splitting) * (window_size / splitting);
                } 
                if (new_box - objects[i].box == -11 or new_box - objects[i].box == -1 or new_box - objects[i].box == 9){
                    objects[i].speed.x = objects[i].speed.x * -1;
                    //new_Position.x = (objects[i].box % splitting) * (window_size / splitting);
                } 
                if (new_box - objects[i].box == -11 or new_box - objects[i].box == -10 or new_box - objects[i].box == -9){
                    objects[i].speed.y = objects[i].speed.y * -1;
                    //new_Position.y = (objects[i].box / splitting) * (window_size / splitting);
                } 
                if (new_box - objects[i].box == 9 or new_box - objects[i].box == 10 or new_box - objects[i].box == 11){
                    objects[i].speed.y = objects[i].speed.y * -1;
                    //new_Position.y = (new_box / splitting) * (window_size / splitting);
                } 

            }        
            
            if (new_Position.x >= window_size)
            {
                new_Position.x = window_size;
                objects[i].speed.x = objects[i].speed.x * -1;
            }
            if (new_Position.x <= 0)
            {
                new_Position.x = 0;
                objects[i].speed.x = objects[i].speed.x * -1;
            }
            if (new_Position.y >= window_size)
            {
                new_Position.y = window_size;
                objects[i].speed.y = objects[i].speed.y * -1;
            }
            if (new_Position.y <= 0)
            {
                new_Position.y = 0;
                objects[i].speed.y = objects[i].speed.y * -1;
            }

            objects[i].setPosition(new_Position); 

            model_window.draw(objects[i].shape);
        }

        for(int i = 0; i<splitting; i++){
            model_window.draw(vertical[i]);
            model_window.draw(horisontal[i]);
        }
        
        model_window.display();

        

        RectangleShape horisontal(Vector2f(2, window_size - 100));
        horisontal.setPosition(Vector2f(50, 50));
        
    }

    return 0;
}


