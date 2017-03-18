#DDRuino
Minimal Dance Dance Revolution like game.  
inspired by <https://www.youtube.com/watch?v=Mf6buDkbyAQ>  

<a href="http://www.youtube.com/watch?feature=player_embedded&v=yLH-KAiVZtQ
" target="_blank"><img src="http://img.youtube.com/vi/yLH-KAiVZtQ/0.jpg " 
alt="movie on youtube" width=50% border="10" /></a>  
[movie on youtube](https://www.youtube.com/watch?v=yLH-KAiVZtQ)  

<a href="http://www.youtube.com/watch?feature=player_embedded&v=w3RBKo6HO3w
" target="_blank"><img src="http://img.youtube.com/vi/w3RBKo6HO3w/0.jpg " 
alt="movie on youtube" width=50% border="10" /></a>  
[movie on youtube](https://www.youtube.com/watch?v=w3RBKo6HO3w)  

##What you need?

* Arduino UNO
* LED Matrix - 1588BS
* Joystick
* (Optional) Arduino Fio v3
* (Optional) XBee
* (Optional) Accelerometer  

##LED Matrix  

<a><img src="https://github.com/matzTada/DDRuino/blob/master/LEDMatrixDirection.png" 
alt="LEDMatrixDirection" width=50%></a>  

references

* <http://playground.arduino.cc/Main/DirectDriveLEDMatrix>

##(Optional) Accelerometer x Wireless control
XBees are set as AT mode.  

###Sender

* Arduino Fio v3
* Accelerometer - ADXL345
* Lithium Polymer battery


references 

* <http://www.geocities.jp/bokunimowakaru/diy/xbee/xbee-arduino_3.html>
* <https://learn.sparkfun.com/tutorials/adxl345-hookup-guide?_ga=1.257198283.1186200183.1484629644>
* <https://learn.adafruit.com/adxl345-digital-accelerometer>



###Pin assign

<a><img src="https://github.com/matzTada/DDRuino/blob/master/DDR_breadboard.png" 
alt="DDR_breadboard" width=50%></a>

Arduino|Other  
--- | ---   
0 (RX) |   
1 (TX) |   
2 | LED Matrix 5 (via Resistor)   
3 | LED Matrix 6   
4 | LED Matrix 7 (via Resistor)    
5 | LED Matrix 8 (via Resistor)    
6 | LED Matrix 9 (via Resistor)    
7 | LED Matrix 10    
8 | LED Matrix 11    
9 | LED Matrix 12 (via Resistor)    
10 | LED Matrix 13   
11 | LED Matrix 14 (via Resistor)    
12 | LED Matrix 15   
13 | LED Matrix 16   
A0 (14) | LED Matrix 4  
A1 (15) | LED Matrix 3  
A2 (16) | LED Matrix 2 (via Resistor)  
A3 (17) | LED Matrix 1 (via Resistor)  
A4 (18) | Joystick VRx (Horizontal)    
A5 (19) | Joystick VRy (vertical)  