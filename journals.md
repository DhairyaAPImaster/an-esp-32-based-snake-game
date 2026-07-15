# An ESP 32 based snake game. Build Journal

## Jul 14, 2026, 5:38 PM

So well i did a bunch of stuff heres what all i did ---> 

First i made the circuit in the editor so i used 4 buttons which are UP DOWN LEFT and RIGHT which i think is obvious 

Then after finishing up the circuit which was actually the least toughest part i saterted writing the code and well lets say it was mildly easy until it wasn't and then honestly i use a little of Chat GPT's help to see if the code i was writing was correct or not and used it to debug some of the mistakes i had made like the order in which the variables were defined which was one of the main problems along with a few small syntax errors which i fixed pretty easily.

So yeah hopefully i finish the project by tomorrow or day after tomorrow

ps: i had to learn like a bunch of like C++ since well the last time i needed C++ i used my friends help and well i dont really know much C++ so yeah learned a lot mainly the functions.

---

## Jul 14, 2026, 7:05 PM

So well in the past hour after taking some breaks to learn more about the errors i was encountering i actually managed to fix them.

Also i made the display ***Actually*** Display the snake and the food so yeah 

the snake is white and a rectangle while the food is round but still white so yeah.

Also i made the game over screen too so if the game gets over then the screen clears and shows--->

GAME
OVER :(

and then shows the score which is snake length - 5 because the snake spawns with 5 recangles in its body so the actuall score will be snake length - 5

So yeah thats done now and now i just have to ship this project!!!!!


**PS- IT IS 12:34 AM in india right now i need to go to sleep i have an exam tomorrow or wait today**

---

## Jul 15, 2026, 2:06 PM

So basically i am transfering the entire project onto a breadboard since well i was having issues with the ground plane and the ground connections without it so an currently doing that and hopefully this time the buttons work.....


## Also

I came across a bug that was really annoying and was making me unable to actually connect the top pins of the buttons to the gpio pins of the esp32 but after a quick chat in #breadboard it was able to be fixed (Thanks @Tanuki ) 


Anyways so yeah i am currently doing that and hopefully this time the ground connections do not cause any issues and i can proceed smoothly.... (Hopefully)

---

## Jul 15, 2026, 3:20 PM

Well I fixed bugs in the code i got the ground connections working but there was an issue in the code for the buttons so yeah fixed that so now to start the game the user needs to press all the buttons and then the game starts since well the issue was ---> 

**the buttons for some reason remain pressed once i press them so like yeah they arent really coming back up they just stay clicked and then when clicking second thime then they un-click but then go back to being clicked**


The fix ---> 

to start the game press all buttons and then the game starts when the signal is 1111 and then when left=0 then that means left is pressed right=0 then right is pressed the 0's are momentary
and so on.....

---

## Jul 15, 2026, 3:52 PM

***This is a short journal***


# FINALLYYYY!!!!!!


got the buttons to work almost properly just some issues remain working on fixing them rn 



like ill write the full details in the next journal after i finish this whole button bugs fully!!!


***PS- a friend of mine (rahav) helped me figure out what went wrong..... bro without him i dont think i would have been able to solve the button thing so fast.... i have not had my dinner yet cause i was too busy doing ts. i need help lol 😭***

---

## Jul 15, 2026, 4:41 PM

# FINALLY  EVERYTHING IS WORKING!!!!!!


LETS GOOOO 


Ok so i fixed the uttons along with the help of my friend as i mentioned in the previous journal so it turns out that the buttons issue was due to debouncing issue so well that has been fixed from a piece of code written by my friend

const unsigned long DEBOUNCE_DELAY = 30; // ms a reading must stay stable to be trusted

struct Button {
  uint8_t pin;
  bool stableState;      // debounced, trusted state (HIGH = not pressed, since INPUT_PULLUP)
  bool lastRawState;     // last raw reading, used to detect changes
  unsigned long lastChangeTime;
};

Button btnUp    = {BTN_UP,    HIGH, HIGH, 0};
Button btnDown  = {BTN_DOWN,  HIGH, HIGH, 0};
Button btnLeft  = {BTN_LEFT,  HIGH, HIGH, 0};
Button btnRight = {BTN_RIGHT, HIGH, HIGH, 0};

struct Point {
  int x;
  int y;
};

......


Well anyways so now since that has been fixed i ran into another problem which was that the button takes 4-5 clicks to be registered and well that seems to be due to like the simulatr instead of code or wiring issues so well

## THE PROJECT IS FINALLY FINISHED!!!!!