Setup:
Revise the turn function in BBAR lib to 
  
  void BBCar::turn( double speed, double factor ){
    
    servo0.set_factor(1);
    
    servo1.set_factor(1);
    
    servo0.set_speed(speed);
    
    servo1.set_speed(speed);
  } 
  
  
Part1: 	
In this part, I first measure the car’s speed and the time we need to go to the parking lot. And, I use the RPC function (/RP/run d1 d2 P )to determine the distance between the BBCAR and the parking lot.
 ![image](https://user-images.githubusercontent.com/79572143/122173397-4448f800-ceb4-11eb-8768-154eb47392f3.png)

 
Part2:
I use the line regression function in main.py in openMV　to detect the black on the floor. And, put the car on the floor and the car will follow the line.


Part3:
		I let the car’s openMV face the apriltag by some degree and some distance (the distance should not be too close, or the openMV may not read the apriltag), then the car will follow the apriltag

