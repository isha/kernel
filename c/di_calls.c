/* di_calls.c
*/

#include <xeroskernel.h>

int  di_open(int dev_no, pcb *p) {
	int fd = 0;
	int echo;

	while (p->fdtab[0] != NULL) {
		fd++;
		if (fd == 4) 
			return -1; //Error, no empty fd slot
	}
	if (dev_no > 1 || dev_no < 0) 
		return -2; //ERROR: bad device number. 
	struct devsw *dev  = &devtab[dev_no]; 
	
	if (dev->inuse > 0) return -3; //E: device already in use!
	p->fdtab[fd] = dev;

	if (dev_no == 1) 
		echo = 1;
	else 
		echo = 0;

	(p->fdtab[fd]->dvinit)(echo);
	p->fdtab[fd]->inuse = 1;				
	
	return fd;
}

int di_close(int fd, pcb *p) {
	if ( p->fdtab[fd] == NULL) 
		return -2;

	if(p->fdtab[fd]->inuse == 0) return -3; //E: Device not opened
	p->fdtab[fd]->inuse = 0;
	(p->fdtab[fd]->dvclose)();
	p->fdtab[fd] = NULL;
	return 0;	

}
int di_write(int fd, void *buff, int buflen, pcb *p) {
	if (p->fdtab[fd] == NULL)
		return -1;
	int return_val = (p->fdtab[fd]->dvwrite)(buff, buflen);
//	p->fdtab[fd]->
	return return_val;	
}

int di_read(int fd, void *buff, int buflen, pcb *p) {
	if (p->fdtab[fd] == NULL) return -1;
	
	int return_val = (p->fdtab[fd]->dvread)(buff, buflen);
	return return_val;
}

int di_ioctl(int fd, unsigned long command, pcb *p) {
	if(p->fdtab[fd] == NULL) return -1;

	int return_val = (p->fdtab[fd]->dvioctl)(command);
	return return_val;
}
