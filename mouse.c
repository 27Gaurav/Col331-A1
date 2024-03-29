#include "types.h"
#include "defs.h"
#include "x86.h"
#include "mouse.h"
#include "traps.h" // Include necessary headers

// Wait until the mouse controller is ready for us to send a packet
void mousewait_send(void) {
    while (inb(0x64) & 0x02)
        ; // Wait until bit 1 is clear
    

}

// Wait until the mouse controller has data for us to receive
void mousewait_recv(void) {
    while (!(inb(0x64) & 0x01))
        ; 
    
    // Wait until bit 0 is set
}

// Send a one-byte command to the mouse controller, and wait for it to be properly acknowledged
void mousecmd(unsigned char cmd) {
    mousewait_send();
    outb(0x64, 0xD4); // Tell the controller we're sending a command to the mouse
    mousewait_send();
    outb(0x60, cmd); // Send the command byte
    mousewait_recv();
    uchar ack  = inb(0x60); // Read the acknowledgement
    if (ack != 0xFA)
        panic("Mouse command failed");
  

}

void mouseinit(void) {
    mousewait_send();
    outb(0x64, 0xA8); // Enable the mouse
    mousewait_send();
    outb(0x64,0x20); 
    mousewait_recv();
    unsigned char status_byte = inb(0x60); // Read the status byte
    status_byte |= 0x02; // Set the 2nd bit to enable interrupts
    mousewait_send();
    outb(0x64, 0x60); 
    mousewait_send();
    outb(0x60, status_byte); 

    mousecmd(0xF6); 
    mousecmd(0xF4); // Activate and start sending interrupts

    ioapicenable(IRQ_MOUSE, 0);
    cprintf("Mouse has been initialized\n");  
    
}

void mouseintr(void) {
    while(inb(0x64)){

    unsigned char data;
    
    // Read the data from the data port (0x60) of the mouse controller
    mousewait_recv();
    data = inb(0x60);
    
    // Print messages based on mouse clicks
    if (data & 0x01)
        cprintf("LEFT\n");
    if (data & 0x02)
        cprintf("RIGHT\n");
    if (data & 0x04)
        cprintf("MID\n");
    for (int i =0 ; i<2 ; i++){
        mousewait_recv();
        data =inb(0x60);
    }}
    
    
    
}
