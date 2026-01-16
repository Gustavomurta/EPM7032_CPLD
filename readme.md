# EPM7032S Parallel Programmer using Arduino Mega 

In 2024 I built this Parallel Programmer using the Arduino Mega. 
But I had some doubts about how the programming was done.
After confirming that it's necessary to enable parallel programming mode on the EPM7032S 
by applying 12V to pin 44 - EDIT/VPP, everything became clearer to me.

This project is based on discussions from the EEV Blog forum: 

https://www.eevblog.com/forum/fpga/programming-(non-jtag)-max7000-devices/msg6159005/#msg6159005

**EPM7032S - JTAG UNLOCK - first time without clear IDCODE!!**

Using my Parallel Programmer - Arduino Mega, I was able for the first time to erase an EPM7032S (JTAG UNLOCK) without erasing the IDCODE.

Thank you very much Carlos Ukamak and all the colleagues who helped me.
