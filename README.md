# Crypto-Price-Tracker-Similation

using a logically simulated cryptomarket of 4 cryptocoins to display the current currency price of Bitcoin
and Ethereum on LCD Screens that will have an alarming LEDS and a buzzer when it hits a “
low “ price. We will have Arduino #1 that will control the coinmarket API and serve as the
master arduino with an led lit(up to 4) for # of tickers being tracked. Arduino #2 will display
the OLED display which will show the current crypto tickers being tracked. Arduino #3 will
have a buzzer that will alarm when one of the tickers has increased in price by >=2%, this
will also cause the LED in Arduino #1 to blink for that crypto. And Arduino #4 which will
have 3 buttons that will allow us to get detailed info on crypto(1 scrolls up, 1 scrolls down,
other works like “enter”). The 3 non-master arduinos will be powered by batteries and will
work through connectivity to the master arduino(Arduino #1). It also does the same for the other 2 cryptocoins
