#include <SFML/Graphics.hpp>
using namespace sf;

float random(){
	int chiappalo=rand()%2+1;
	if(chiappalo==1) return 3.16;
	if(chiappalo==2) return 0;
}

int main(){
	//variabili utili
	int width=800, height=600;
	Vector2f paddleSize(25,100);
	int points1=0, points2=0;
	bool playing=false, moving=false;
	float ballRadius=10.f, ballAngle=random();
	float paddleSpeed=700.f,ballSpeed=700.f;
	float pi = 3.14159f;
	
	//immagine
	Texture t1;
	t1.loadFromFile("Resources/Cattura.PNG");
	Sprite image;
	image.setTexture(t1);
	image.setPosition(85, 150);
	
	//crea paddle sinistro p1
	RectangleShape leftPaddle;
	leftPaddle.setSize(paddleSize);
	leftPaddle.setOrigin(paddleSize/2.f);
	leftPaddle.setFillColor(Color::White);
	leftPaddle.setOutlineThickness(3);
	leftPaddle.setOutlineColor(Color::Red);
	
	//crea paddle destro p2
	RectangleShape rightPaddle;
	rightPaddle.setSize(paddleSize);
	rightPaddle.setOrigin(paddleSize/2.f);
	rightPaddle.setFillColor(Color::White);
	rightPaddle.setOutlineThickness(3);
	rightPaddle.setOutlineColor(Color::Green);
	
	//crea ball
	CircleShape ball;
	ball.setRadius(ballRadius);
	ball.setFillColor(Color::White);
	ball.setOrigin(ballRadius/2, ballRadius/2);
	
	//immagine ico
	auto ico = Image{};
	ico.loadFromFile("Resources/PONG.png");
	
	//make window
	RenderWindow game(VideoMode(width,height), "PONG!");
	game.setIcon(ico.getSize().x, ico.getSize().y, ico.getPixelsPtr());
	Event event;
	
	//crea orologi
	Clock clock1;
	
	//main loop while open
	while(game.isOpen()){
		while(game.pollEvent(event)){
			if(event.type==Event::Closed){
				game.close();
			}
			if(event.type==Event::KeyPressed&&event.key.code==Keyboard::Space){
				if(!playing){
					clock1.restart();
					playing=true;
					moving=true;
					leftPaddle.setPosition(10+paddleSize.x, height/2);
					rightPaddle.setPosition(width-10-paddleSize.x, height/2);
					ball.setPosition(width/2, height/2);
					points1=0;
					points2=0;
					ballSpeed=700.f;
				}
				else{
					if(!moving){
						ballSpeed=700.f;
						ballAngle=random();
						moving=true;
						playing=true;
					}
				}
			}
			if(event.type==Event::KeyPressed&&event.key.code==Keyboard::Escape){
				playing=false;
				moving=false;
			}
		}//fine event
		
		//gestisci gioco
		if(playing){
			//movment paddle
			float timePassed=clock1.restart().asSeconds();
			game.setTitle("Points: "+std::to_string(points1)+" | "+std::to_string(points2));
			if((Keyboard::isKeyPressed(Keyboard::W)) && (leftPaddle.getPosition().y > 50)) leftPaddle.move(0.f, -paddleSpeed*timePassed);
			if((Keyboard::isKeyPressed(Keyboard::S)) && (leftPaddle.getPosition().y < 550)) leftPaddle.move(0.f, paddleSpeed*timePassed);
			if((Keyboard::isKeyPressed(Keyboard::Up)) && (rightPaddle.getPosition().y > 50)) rightPaddle.move(0.f, -paddleSpeed*timePassed);
			if((Keyboard::isKeyPressed(Keyboard::Down)) && (rightPaddle.getPosition().y < 550)) rightPaddle.move(0.f, paddleSpeed*timePassed);
			//ball movement
            float mv = ballSpeed * timePassed;
            ball.move(cos(ballAngle) * mv, sin(ballAngle) * mv);
            //wall collision
            if(ball.getPosition().x-ballRadius<0.f){
            	ball.setPosition(width/2, height/2);
				points2++;
				ballSpeed=0.f;
				moving=false;
			}
            if(ball.getPosition().x+ballRadius>width){
				ball.setPosition(width/2, height/2);
				points1++;
				ballSpeed=0.f;
				moving=false;
			}
			if(ball.getPosition().y-ballRadius<0.f){
				ballAngle=-ballAngle;
				ball.setPosition(ball.getPosition().x, ballRadius+0.1f);
			}
			if(ball.getPosition().y+ballRadius>height){
				ballAngle=-ballAngle;
				ball.setPosition(ball.getPosition().x, height-ballRadius-0.1f);
			}
			//paddle collision
			//left
			if(ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
				{
                	if(ball.getPosition().y>leftPaddle.getPosition().y)
                		ballAngle = pi - ballAngle + (rand()%20) * pi/180;
                	else
                		ballAngle = pi - ballAngle - (rand()%20) * pi/180;
                	ball.setPosition(leftPaddle.getPosition().x+ballRadius+paddleSize.x/2+0.1f, ball.getPosition().y);
				}
			//right
			if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
	            {
	                if (ball.getPosition().y > rightPaddle.getPosition().y)
	                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
	                else
	                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;
	                ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
	            }
		}
		//draw stuff sullo schermo
		game.clear();
		if(!playing){
			game.draw(image);
		}
		else{
			game.draw(rightPaddle);
			game.draw(leftPaddle);
			game.draw(ball);
		}
		
		//display stuff sullo schermo
		game.display();
			
	}//fine while open
}//FINE PROGRAMMA
