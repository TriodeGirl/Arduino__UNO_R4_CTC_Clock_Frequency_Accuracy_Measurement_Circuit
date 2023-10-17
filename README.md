# Arduino__UNO_R4_CTC_Clock_Frequency_Accuracy_Measurement_Circuit
R4 Minima RA4M1 barebones direct-register code to use the CTC module in the RA4M1 processor.

Uses an external 1kHz clock to measure the R4 RA4M1 processor's HOCO clock accuracy.

External clock can be provided with an Arduino Leonardo (code provided to output a c. 1.000kHz clock on pin D6)

The following two images show the relative stability (or not) of a 12.000MHz XTAL (as fitted to the EK-RA4M1 board) to a divided-by-four 48Mhz HOCO clock. The center rising-edge is the twelth clock after the trigger i.e. at 1.00uS

12.000 MHz XTAL

![Susan-RA4M1-XTAL-12MHz-compare-measurment-at-1uS-1](https://github.com/TriodeGirl/Arduino__UNO_R4_CTC_Clock_Frequency_Accuracy_Measurement_Circuit/assets/139503623/224cd251-a109-45bb-952f-db8395704572)


Divided-by-four 48MHz HOCO clock

![Susan-RA4M1-HOCO-12MHz-compare-measurment-at-1uS-1](https://github.com/TriodeGirl/Arduino__UNO_R4_CTC_Clock_Frequency_Accuracy_Measurement_Circuit/assets/139503623/18e7bf0b-53c9-40a1-bb68-47547210e198)
