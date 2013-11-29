/* di_calls.c
*/

#include <xeroskernel.h>

/*
 *Device Independet call for opening a device. It finds the device structure in the device table, finds an empty fdslot in the pcb to assign it to, and returns that fd.
 * Returns -1 if there is no fd slots empty, -2 if the device number is bad, -3 if the device is already in use.
 */
int  di_open(int dev_no, pcb *p) {
	int fd = 0;
	int echo;
	//check for empty fd slot
	while (p->fdtab[0] != NULL) {
		fd++;
		if (fd == 4) 
			return -1; //Error, no empty fd slot
	}
	//check for bad device number
	if (dev_no > 1 || dev_no < 0) 
		return -2; //ERROR: bad device number. 
	struct devsw *dev  = &devtab[dev_no]; 
	
	//check for device already in use
	if (dev->inuse > 0) return -3; //E: device already in use!
	p->fdtab[fd] = dev;
	
	//figure out the echo part so we multiple functions can use this kernel
	if (dev_no == 1) 
		echo = 1;
	else 
		echo = 0;
	//point to the device_driver code
	(p->fdtab[fd]->dvinit)(echo);
	p->fdtab[fd]->inuse = 1;				
	//now that we know that the keyboard is in use, we have to mark the other version as well
	if (dev_no == 0)
		devtab[1].inuse =1;
	else 
		devtab[0].inuse = 1;	
	return fd;
}
/*
 * Closes the  device and gets frees up the FD for the process. 
 * returns -2 if the fd doesnt exist, or -3 if the device isn't opened.
 * returns 0 on success
 */
int di_close(int fd, pcb *p) {
	//check to see if the fd is allocated
	if ( p->fdtab[fd] == NULL) 
		return -2;
	//check to see if the device is opened
	if(p->fdtab[fd]->inuse == 0) return -3; //E: Device not opened
	//let both echo keyboard and regular keyboard know that the device is not in use
	p->fdtab[fd]->inuse = 0;
	if(p->fdtab[fd]->dvnum == 1)
		devtab[0].inuse = 0;
	else devtab[1].inuse = 1;
	//call the driver specific close and NULL out the fdtab
	(p->fdtab[fd]->dvclose)();
	p->fdtab[fd] = NULL;
	return 0;	

}

/*
 * Writes to a buffer that will be shared by the device.
 * returns -1 if the fd is invalid.
 * calls the device specific write.
 */
int di_write(int fd, void *buff, int buflen, pcb *p) {
	if (p->fdtab[fd] == NULL)
		return -1;
	int return_val = (p->fdtab[fd]->dvwrite)(buff, buflen);
//	p->fdtab[fd]->
	return return_val;	
}
/*
 *Reads from the device buffer by calling the device specific function
 * returns -1 if the fd is invalid.
 */
int di_read(int fd, void *buff, int buflen, pcb *p) {
	if (p->fdtab[fd] == NULL) return -1;
	
	int return_val = (p->fdtab[fd]->dvread)(buff, buflen);
	return return_val;
}
/*
 * passes along a control signal to the device. Calls the device specific control
 *returns -1 if the fd is invalid.
 */
int di_ioctl(int fd, unsigned long command, pcb *p) {
	if(p->fdtab[fd] == NULL) return -1;

	int return_val = (p->fdtab[fd]->dvioctl)(command);
	return return_val;
}
