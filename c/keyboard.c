#include <xeroskernel.h>

unsigned int keyboard_buf[4];
short keyboard_it = 0;
unsigned int echo= 0;
char end_of_char = 0x04; //arbitrarily set it as caps lock
/*
 *Keyboard specific device open.
 * take in ifthis keyboard will be echomode o rnot
 * if the other keyboard is not open (echo or non ehco), open the device
 * enable the itnerrupt by using enable_irq
 * return 0 on success
 */
unsigned int keyboard_open(int echo) {
	keyboard_it = 0; 
	enable_irq(1,0);
	//figure out if this is echo or not.if it is, turn off the other one. 
	if (echo == 1) echo = 1;
	else echo = 0;
	return (unsigned int) 0;
}

/*
 *Close the keyboard by turning off the IRQ. 
 &  return 0 on success
 */
unsigned int keyboard_close(void) {
	enable_irq(1,1);
	return 0;
}
/* read a value from the keyboard. 
 * this is done by supplying the process with the next character in the keyboard buffer.
 * if there is nothing in the buffer, return 0.
 * if buffer has items in it, put the next item in the char, and return  1.
 * move everything in the buffer over to maintain order. e
 */
unsigned int keyboard_read(char *buf, int len) {
	int buf_empty = 5; //a number bigger than what we have
	char *ret_val; //this value will be returned. 
//check to see if the buffer is empty
	for(keyboard_it = 0; keyboard_it < 4; keyboard_it++) {
		if (keyboard_buf[keyboard_it] != 0) {
			buf_empty = keyboard_it;
			break;
		}
	}
	if (buf_empty == 5) return 0; //return 0 if the buffer is in fact empty

	//Case if there is something in the buffer: return value gets the value 	
	*ret_val =  keyboard_buf[keyboard_it]; //put it in the program buffer

	//move everything in the bufferover by one. 
	keyboard_buf[0] = keyboard_buf[1];
	keyboard_buf[1] = keyboard_buf[2];
	keyboard_buf[2] = keyboard_buf[3];
	keyboard_buf[3] = 0;

	//if echo is one, echo.
	if (echo == 1) {
		sysputs(ret_val);
	}
	//point to the value
	buf = ret_val;
	if (*ret_val == end_of_char) return 2; //special return case if user is entering the end of file buttton. 
	return 1;
}

/*
 *you can't write to keyboards. return -1.
 */
unsigned int keyboard_write(void *buf, int len) {
	return -1; //error can't return anything
}
/*
 * keyboard IOCTL sets the new end of char.
 * that new value is passed through command.
 * thus we cast it and grab the new value!
 * set up the new end of char.
 * return 0 if successful, -1 if not. 
 */
unsigned int keyboard_ioctl(unsigned long command) {
	end_of_char = (unsigned char) command;
	return -0;
}
/*
 * This Interrupt service routine is called by the dispatcher whenever a keyboard interrupt occurs.
 * We first check to see if there is indeed data in the device Register.
 * then we get the value and convert it to ASCII char. 
 * we find the next empty spot in buffer and place the char there. 
 * we return 0 if theere is no space in teh buffer. We return -1 if something went wrong, and we did something we weren't supposed to do. 
 */
extern int keyboard_isr() {
	int i = 5; //value bigger than the buffer
	short value_written = 0;
	//check to see if keyboard is ready with data
	if( (inb(0x64) & 0x01) == 1) {
		unsigned char char_reg = inb(0x60); //get val
		unsigned int ascii = kbtoa(char_reg); //convert to ascii
		
		//find empty spot in character buffer
		for (i = 0; i < 4; i++) {
			if(keyboard_buf[i] == 0){ 
				//if spot found, write to buffer
				keyboard_buf[i] = ascii;
				value_written = 1;
			}
		}
		//return 1 on success, 0 if not/
		if(value_written = 1) return 1;
		else return 0;	
		
	}
	else {
	//bad: we shouldn't reach ehre
		return -1;
	}
	return -1; //Just to get rid of pesky warning!
}	
/*
 *This inits the keyboard devices. 
 * we set the dv num and inuse bits to 0.
 * we point our keyboard specific functions to the device independent ones using function pointers.
 * we set up the device numbers as well as the inuse bits.  
 */
int keyboard_init() {

	struct devsw* keyboard = &devtab[KEYBOARD]; 
	keyboard->dvinit = (funcptr)(keyboard_open);
	keyboard->dvwrite = (funcptr)(keyboard_write);
	keyboard->dvread = (funcptr) (keyboard_read);
	keyboard->dvclose = (funcptr) (keyboard_close);
	keyboard->dvioctl = (funcptr) (keyboard_ioctl);
	keyboard->dvnum = 0;
	keyboard->inuse = 0;

	struct devsw* keyboardecho = &devtab[KEYBOARDECHO];
	keyboardecho->dvinit = (funcptr) (keyboard_open);
	keyboardecho->dvwrite = (funcptr)(keyboard_write);
	keyboardecho->dvread = (funcptr) (keyboard_read);
	keyboardecho->dvclose = (funcptr) (keyboard_close);
	keyboardecho->dvioctl = (funcptr) (keyboard_ioctl);
	keyboardecho->dvnum = 1;
	keyboardecho->inuse = 0;
	return 0;
}
