# TreasureHuntGame

IOT Device game 
Uses GPS, ESP32, tft lcd screen 1.8", buttons

Uses Python server-side code with sqlite3 to create server database to hold past accomplished objective locations.
Uses GPS and C++ for user interface and display



You start with a pre-made list of locations you need to visit on campus living on the server. 
One of these locations is displayed on the labkit's LCD. You and your device, must move to that region, 
and then when it/you get there, the screen will update and tell you the next one to go to, and so on.

My locations ist only has 5 objectives currently, but the code is designed so that more could easily be added.

I utilized a database that stores just the current index of the objective location the player 
is on and the time so that when playing you can look up the most recent objective location 
and see what you are currently supposed to head to, even if the arduino is reset in the middle 
of a game. Furthermore, the player can restart the game at any point by just pressing button 2. 
The list of the 5 objective locations is just in the python file making it easy to simply just 
index the location list to see what objective location. I made a nice little display on the 
arduino side of things to show the user some basic starter instructions for how to use the 
buttons to play the game based on what they want to do when playing (which is very simple as 
there are only two buttons and two possible things the user can do, play the game or restart the game!).

https://www.youtube.com/watch?v=quVItTsUDjk to see it in action!
