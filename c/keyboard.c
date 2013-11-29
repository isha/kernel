#include <xeroskernel.h>

unsigned int keyboard_buf[4];
short keyboard_it = 0;
unsigned int echo= 0;

unsigned int keyboard_open(int echo) {
	keyboard_it = 0;
	enable_irq(1,0);
	if (echo == 1) echo = 1;
	else echo = 0;
	return 0;
}

unsigned int keyboard_close(void) {
	enable_irq(1,1);
	return 0;
}

unsigned int keyboard_read(void *buf, int len) {
	int buf_empty = 5; //a number bigger than what we have
	unsigned int *ret_val;
//check to see if the buffer is empty
	for(keyboard_it = 0; keyboard_it < 4; keyboard_it++) {
		if (keyboard_buf[keyboard_it] != 0) {
			buf_empty = keyboard_it;
			break;
		}
	}
	if (buf_empty == 5) return 0; //return 0 if the buffer is in fact empty

	//Case if there is something in the buffer: 	
	ret_val =  keyboard_buf[keyboard_it]; //put it in the buffer

	//move everything over by one. 
	keyboard_buf[0] = keyboard_buf[1];
	keyboard_buf[1] = keyboard_buf[2];
	keyboard_buf[2] = keyboard_buf[3];
	keyboard_buf[3] = 0;
	
	buf = ret_val;
	return 1;
}

unsigned int keyboard_write(void *buf, int len) {
	return -1; //error can't return anything
}

unsigned int keyboard_ioctl(unsigned long command) {
	return -1;
}

extern int keyboard_isr() {
	int i = 5;
	short value_written = 0;
	//check to see if keyboard is ready
	if( (inb(0x64) & 0x01) == 1) {
		unsigned char char_reg = inb(0x60); //get val and convert to ascii
		unsigned int ascii = kbtoa(char_reg);
		
		for (i = 0; i < 4; i++) {
			if(keyboard_buf[i] == 0){ 
				keyboard_buf[i] = ascii;
				value_written = 1;
			}
		}
		if(value_written = 1) return 0;
		else return 1;	
		
	}
	else {
		return -1;
	}
	return -1; //Just to get rid of pesky warning!
}	

int keyboard_init() {
	struct devsw* keyboard = &devtab[KEYBOARD]; 
	keyboard->dvinit = (funcptr)(keyboard_open);
	keyboard->dvwrite = (funcptr)(keyboard_write);
	keyboard->dvread = (funcptr) (keyboard_read);
	keyboard->dvclose = (funcptr) (keyboard_close);
	keyboard->dvioctl = (funcptr) (keyboard_ioctl);

	struct devsw* keyboardecho = &devtab[KEYBOARDECHO];
	keyboard->dvinit = (funcptr) (keyboard_open);
	keyboard->dvwrite = (funcptr)(keyboard_write);
	keyboard->dvread = (funcptr) (keyboard_read);
	keyboard->dvclose = (funcptr) (keyboard_close);
	keyboard->dvioctl = (funcptr) (keyboard_ioctl);
	return 0;
}
