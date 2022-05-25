# Timer
Simple arduino bare-metal project

# Overwiew

Simple project osing Arduino UNO with I2C module. The program is written using olny AVR libraries. It works as follows: \n
After turning on the device, the user will see an LCD that displays “00:00:00”
(hours:minutes:seconds). On the last digit, there will be a blinking cursor.
The user has five buttons to control the device. First two move the cursor left and right
(always on the second digit from the pair of two). If the cursor will be moved right from the
max right position, it will jump to the max left position and opposite. The next two decrease
and increase the value marked by the cursor. If the value is equal to 0 and the user tries to
decrease it, the value will change to max (59 in case of minutes and seconds, 23 in case of
hours). If there is a maximum value and the user tries to increase it, the value will change to
0 instead.
After setting the time, the user can start counting by pressing a “start/stop” button (it is not
possible to start counting when the time is set to 0). The cursor will disappear and the values
on the screen will start to decrease with each second. During the countdown, the user can
stop the process by pressing the “start/stop” button. The cursor will reappear and it will be
possible to change the time remaining. To start counting again, the user has to press the
“start/stop” button.
When the timer reaches the value “00:00:00”, the LED will start to blink until the “start/stop”
button is pressed. Then, the timer returns to its initial state, the cursor appears and the new
time can be set.

# Hardware used

● Arduino Uno
● LCD HD44780
● I2C converter module
● breadboard x2
● NO button x5
● jumper cables
● resistors
● LED


# Screenshots

![obraz](https://user-images.githubusercontent.com/106124763/170385480-d648011c-b45b-4731-8142-bfa0bdf16f71.png)

![obraz](https://user-images.githubusercontent.com/106124763/170385503-da5f7faf-c588-4ef8-b034-9de4e57db60b.png)

