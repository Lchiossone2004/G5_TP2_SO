#include <sound.h>
#include <stdint.h>
#include <time.h>
#include <lib.h>
 //Play sound using built-in speaker
 static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }
 
 
 static void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
     
 	outb(0x61, tmp);
 }

 void timer_wait(int wait) {
    int ticks = ticks_elapsed();
    while(ticks_elapsed() - ticks < wait) {

    }
 }

 void beep(int flag) { //si el flag esta en 1 es q perdio
    if(flag) {
    play_sound(700);
    timer_wait(3);
    play_sound(300);
    timer_wait(3);
    play_sound(100);
    timer_wait(5);
    }
    else {
    play_sound(500);
    timer_wait(2);
    play_sound(1000);
    timer_wait(2);
    }
 	nosound();
        
 }
 