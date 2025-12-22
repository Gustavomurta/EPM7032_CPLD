
```

After running the first test of Phase 1, I obtained a response from the SDOUTA and SDOUTB pins. Success!
In phase 1, I only activated NTPW and then immediately activated 12V on EDIT/VPP. Notice that the SDOUTA and SDOUTB pins were low (0) before this activation.
After activation, the pins were high (1). This proves that the CPLD is responding!

EPM7032S - Phase 1
Port PB PH00: 10000000  // SCOB(PB3) / SDOUTB(PB2) / SCOA(PB1) / SDOUTA(PB0)
Port PB PH01: 10000101  // SCOB(PB3) / SDOUTB(PB2) / SCOA(PB1) / SDOUTA(PB0)
In the programmer's project, I will have to develop several phases. Some of them more complex. Stay tuned for the development of the other phases.



EPM7032S Parallel Programmer - ERASE V6
EPM7032S - Phase 1
Port PB PH00: 10000000
Port PB PH01: 10000101
Test count: 1
End

EPM7032S - Phase 1
Port PB PH00: 10000000  // SCOB(PB3) / SDOUTB(PB2) / SCOA(PB1) / SDOUTA(PB0)
Port PB PH01: 10000101  // SCOB(PB3) / SDOUTB(PB2) / SCOA(PB1) / SDOUTA(PB0)
Test count: 2
End

EPM7032S - Phase 1
Port PB PH00: 10000000
Port PB PH01: 10000101
Test count: 3
End

EPM7032S - Phase 1
Port PB PH00: 10000000
Port PB PH01: 10000101
Test count: 4
End
```
