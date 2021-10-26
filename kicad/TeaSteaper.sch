EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A 11000 8500
encoding utf-8
Sheet 1 1
Title "TeaSteaper"
Date "2021-10-21"
Rev "1.0"
Comp "Christopher B. Liebman"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TeaSteaper:ESP32-WROOM-32D U1
U 1 1 6171FC3D
P 3850 2700
F 0 "U1" H 3450 4100 50  0000 C CNN
F 1 "ESP32-WROOM-32D" H 4300 4100 50  0000 C CNN
F 2 "TeaMaker:ESP32-WROOM-32" H 3850 1200 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 3550 2750 50  0001 C CNN
	1    3850 2700
	1    0    0    -1  
$EndComp
$Comp
L TeaSteaper:ULN2803A U2
U 1 1 617333C5
P 6800 5200
F 0 "U2" H 6800 5767 50  0000 C CNN
F 1 "ULN2803A" H 6800 5676 50  0000 C CNN
F 2 "TeaMaker:SOIC-18W_7.5x11.6mm_P1.27mm" H 6850 4550 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/uln2803a.pdf" H 6900 5000 50  0001 C CNN
	1    6800 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 5000 6400 5000
Wire Wire Line
	6000 5100 6400 5100
Wire Wire Line
	6000 5200 6400 5200
Wire Wire Line
	6000 5300 6400 5300
$Comp
L power:GND #PWR02
U 1 1 6174E4EC
P 3850 4200
F 0 "#PWR02" H 3850 3950 50  0001 C CNN
F 1 "GND" H 3855 4027 50  0000 C CNN
F 2 "" H 3850 4200 50  0001 C CNN
F 3 "" H 3850 4200 50  0001 C CNN
	1    3850 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 6174F1CB
P 6800 6000
F 0 "#PWR05" H 6800 5750 50  0001 C CNN
F 1 "GND" H 6805 5827 50  0000 C CNN
F 2 "" H 6800 6000 50  0001 C CNN
F 3 "" H 6800 6000 50  0001 C CNN
	1    6800 6000
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR01
U 1 1 61751536
P 3850 1200
F 0 "#PWR01" H 3850 1050 50  0001 C CNN
F 1 "+3V3" H 3865 1373 50  0000 C CNN
F 2 "" H 3850 1200 50  0001 C CNN
F 3 "" H 3850 1200 50  0001 C CNN
	1    3850 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 617541B9
P 7250 4750
F 0 "#PWR06" H 7250 4600 50  0001 C CNN
F 1 "+5V" H 7265 4923 50  0000 C CNN
F 2 "" H 7250 4750 50  0001 C CNN
F 3 "" H 7250 4750 50  0001 C CNN
	1    7250 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 4900 7250 4900
Wire Wire Line
	7250 4900 7250 4750
Wire Wire Line
	6800 5900 6800 6000
Wire Wire Line
	3850 4100 3850 4200
Wire Wire Line
	3850 1200 3850 1300
NoConn ~ 3250 1700
NoConn ~ 3250 1800
NoConn ~ 3250 2700
NoConn ~ 3250 2800
NoConn ~ 3250 2900
NoConn ~ 3250 3000
NoConn ~ 3250 3100
NoConn ~ 3250 3200
Wire Wire Line
	4450 1500 4700 1500
Text Label 4700 1500 2    50   ~ 0
IO0
Wire Wire Line
	4450 1600 4700 1600
Wire Wire Line
	4450 1800 4700 1800
Text Label 4700 1600 2    50   ~ 0
TXD
Text Label 4700 1800 2    50   ~ 0
RXD
Wire Wire Line
	7200 5000 7550 5000
Wire Wire Line
	7200 5100 7550 5100
Wire Wire Line
	7200 5200 7550 5200
Wire Wire Line
	7200 5300 7550 5300
Wire Wire Line
	4450 2200 4700 2200
Wire Wire Line
	4450 2300 4700 2300
Wire Wire Line
	4450 2400 4700 2400
Text Label 4700 2200 2    50   ~ 0
TCK
Text Label 4700 2300 2    50   ~ 0
TMS
Text Label 4700 2400 2    50   ~ 0
TDO
Wire Wire Line
	4450 2100 4700 2100
Text Label 4700 2100 2    50   ~ 0
TDI
Wire Wire Line
	4450 2700 4700 2700
Text Label 4700 2700 2    50   ~ 0
SCLK
Wire Wire Line
	4450 2900 4700 2900
Text Label 4700 2900 2    50   ~ 0
DC
Wire Wire Line
	4450 3000 4700 3000
Text Label 4700 3000 2    50   ~ 0
RST
Wire Wire Line
	4450 3100 4700 3100
Text Label 4700 3100 2    50   ~ 0
MOSI
Wire Wire Line
	4450 2000 4700 2000
Text Label 4700 2000 2    50   ~ 0
CS
Wire Wire Line
	4450 3300 4700 3300
Wire Wire Line
	4450 3200 4700 3200
Wire Wire Line
	4450 3400 4700 3400
Wire Wire Line
	4450 3500 4700 3500
Text Label 4700 3200 2    50   ~ 0
M1A
Text Label 4700 3300 2    50   ~ 0
M1B
Text Label 4700 3400 2    50   ~ 0
M1C
Text Label 4700 3500 2    50   ~ 0
M1D
Text Label 6000 5000 0    50   ~ 0
M1A
Text Label 6000 5300 0    50   ~ 0
M1B
Text Label 6000 5500 0    50   ~ 0
M1C
Text Label 6000 5700 0    50   ~ 0
M1D
Text Label 7550 5000 2    50   ~ 0
P_M1A
Text Label 7550 5300 2    50   ~ 0
P_M1B
Text Label 7550 5500 2    50   ~ 0
P_M1C
Wire Wire Line
	4450 2600 4700 2600
Text Label 4700 2600 2    50   ~ 0
BL
$Comp
L TeaSteaper:AH3572-SA-7 U3
U 1 1 617301C6
P 8050 2200
F 0 "U3" H 8900 2465 50  0000 C CNN
F 1 "AH3572-SA-7" H 8900 2374 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8050 2600 50  0001 L CNN
F 3 "https://www.diodes.com/assets/Datasheets/AH3572.pdf" H 8050 2700 50  0001 L CNN
F 4 "+125°C" H 8050 2800 50  0001 L CNN "ambient temperature range high"
F 5 "-40°C" H 8050 2900 50  0001 L CNN "ambient temperature range low"
F 6 "No" H 8050 3000 50  0001 L CNN "automotive"
F 7 "IC" H 8050 3100 50  0001 L CNN "category"
F 8 "Sensors" H 8050 3200 50  0001 L CNN "device class L1"
F 9 "Magnetic Sensors" H 8050 3300 50  0001 L CNN "device class L2"
F 10 "unset" H 8050 3400 50  0001 L CNN "device class L3"
F 11 "MAG SWITCH OMNIPOLAR SOT23 T&R" H 8050 3500 50  0001 L CNN "digikey description"
F 12 "AH3572-SA-7DICT-ND" H 8050 3600 50  0001 L CNN "digikey part number"
F 13 "https://www.diodes.com/assets/Package-Files/SOT23.pdf" H 8050 3700 50  0001 L CNN "footprint url"
F 14 "1.1mm" H 8050 3800 50  0001 L CNN "height"
F 15 "SOT92P240X102-3" H 8050 3900 50  0001 L CNN "ipc land pattern name"
F 16 "Yes" H 8050 4000 50  0001 L CNN "lead free"
F 17 "b5108e0e74253d65" H 8050 4100 50  0001 L CNN "library id"
F 18 "Diodes Inc." H 8050 4200 50  0001 L CNN "manufacturer"
F 19 "+150°C" H 8050 4300 50  0001 L CNN "max junction temp"
F 20 "28V" H 8050 4400 50  0001 L CNN "max supply voltage"
F 21 "3V" H 8050 4500 50  0001 L CNN "min supply voltage"
F 22 "621-AH3572-SA-7" H 8050 4600 50  0001 L CNN "mouser part number"
F 23 "3mA" H 8050 4700 50  0001 L CNN "operating supply current"
F 24 "60mA" H 8050 4800 50  0001 L CNN "output current"
F 25 "SOT-23-3" H 8050 4900 50  0001 L CNN "package"
F 26 "Yes" H 8050 5000 50  0001 L CNN "rohs"
F 27 "Hall Effect" H 8050 5100 50  0001 L CNN "sensing method"
F 28 "Other" H 8050 5200 50  0001 L CNN "sensor output"
F 29 "0.013mm" H 8050 5300 50  0001 L CNN "standoff height"
F 30 "+125°C" H 8050 5400 50  0001 L CNN "temperature range high"
F 31 "-40°C" H 8050 5500 50  0001 L CNN "temperature range low"
	1    8050 2200
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 6175C3C2
P 6350 2500
F 0 "#PWR03" H 6350 2250 50  0001 C CNN
F 1 "GND" H 6355 2327 50  0000 C CNN
F 2 "" H 6350 2500 50  0001 C CNN
F 3 "" H 6350 2500 50  0001 C CNN
	1    6350 2500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR07
U 1 1 6175D4BB
P 8000 2100
F 0 "#PWR07" H 8000 1950 50  0001 C CNN
F 1 "+3V3" H 8015 2273 50  0000 C CNN
F 2 "" H 8000 2100 50  0001 C CNN
F 3 "" H 8000 2100 50  0001 C CNN
	1    8000 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 2200 8000 2200
Wire Wire Line
	8000 2200 8000 2100
Wire Wire Line
	6450 2400 6350 2400
Wire Wire Line
	6350 2400 6350 2500
Wire Wire Line
	6450 2200 6050 2200
Text Label 6050 2200 0    50   ~ 0
ULIM
Wire Wire Line
	4450 1900 4700 1900
Wire Wire Line
	4450 2500 4700 2500
Text Label 4700 1900 2    50   ~ 0
ULIM
$Comp
L TeaSteaper:AZ1117-3.3 U5
U 1 1 6176FDD7
P 2800 6000
F 0 "U5" H 2800 6242 50  0000 C CNN
F 1 "AZ1117-3.3" H 2800 6151 50  0000 C CNN
F 2 "TeaMaker:SOT-223-3_TabPin2" H 2800 6250 50  0001 C CIN
F 3 "https://www.diodes.com/assets/Datasheets/AZ1117.pdf" H 2800 6000 50  0001 C CNN
	1    2800 6000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 6177480C
P 3250 6250
F 0 "C3" H 3365 6296 50  0000 L CNN
F 1 "10uf" H 3365 6205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 3288 6100 50  0001 C CNN
F 3 "~" H 3250 6250 50  0001 C CNN
	1    3250 6250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 61777290
P 2350 6250
F 0 "C2" H 2465 6296 50  0000 L CNN
F 1 "10uf" H 2465 6205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 2388 6100 50  0001 C CNN
F 3 "~" H 2350 6250 50  0001 C CNN
	1    2350 6250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 6400 2350 6550
Wire Wire Line
	2350 6550 2800 6550
Wire Wire Line
	3250 6550 3250 6400
Wire Wire Line
	2800 6300 2800 6550
Connection ~ 2800 6550
Wire Wire Line
	2800 6550 3250 6550
$Comp
L power:GND #PWR016
U 1 1 6177BA9F
P 2800 6650
F 0 "#PWR016" H 2800 6400 50  0001 C CNN
F 1 "GND" H 2805 6477 50  0000 C CNN
F 2 "" H 2800 6650 50  0001 C CNN
F 3 "" H 2800 6650 50  0001 C CNN
	1    2800 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 6550 2800 6650
Wire Wire Line
	2500 6000 2350 6000
Wire Wire Line
	2350 6000 2350 6100
Wire Wire Line
	3100 6000 3250 6000
Wire Wire Line
	3250 6000 3250 6100
$Comp
L power:+5V #PWR015
U 1 1 6177F537
P 2350 5850
F 0 "#PWR015" H 2350 5700 50  0001 C CNN
F 1 "+5V" H 2365 6023 50  0000 C CNN
F 2 "" H 2350 5850 50  0001 C CNN
F 3 "" H 2350 5850 50  0001 C CNN
	1    2350 5850
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR017
U 1 1 61780800
P 3250 5850
F 0 "#PWR017" H 3250 5700 50  0001 C CNN
F 1 "+3V3" H 3265 6023 50  0000 C CNN
F 2 "" H 3250 5850 50  0001 C CNN
F 3 "" H 3250 5850 50  0001 C CNN
	1    3250 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 5850 2350 6000
Connection ~ 2350 6000
Wire Wire Line
	3250 5850 3250 6000
Connection ~ 3250 6000
NoConn ~ 4450 3600
NoConn ~ 4450 3700
NoConn ~ 4450 3800
$Comp
L Device:C C1
U 1 1 61789360
P 1800 1850
F 0 "C1" H 1915 1896 50  0000 L CNN
F 1 "10uf" H 1915 1805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 1838 1700 50  0001 C CNN
F 3 "~" H 1800 1850 50  0001 C CNN
	1    1800 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 6178D8BC
P 1800 1450
F 0 "R1" H 1870 1496 50  0000 L CNN
F 1 "10k" H 1870 1405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1730 1450 50  0001 C CNN
F 3 "~" H 1800 1450 50  0001 C CNN
	1    1800 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 1500 3050 1500
Text Label 3050 1500 0    50   ~ 0
EN
$Comp
L power:+3V3 #PWR011
U 1 1 617908F0
P 1800 1200
F 0 "#PWR011" H 1800 1050 50  0001 C CNN
F 1 "+3V3" H 1815 1373 50  0000 C CNN
F 2 "" H 1800 1200 50  0001 C CNN
F 3 "" H 1800 1200 50  0001 C CNN
	1    1800 1200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 61791884
P 1800 2100
F 0 "#PWR012" H 1800 1850 50  0001 C CNN
F 1 "GND" H 1805 1927 50  0000 C CNN
F 2 "" H 1800 2100 50  0001 C CNN
F 3 "" H 1800 2100 50  0001 C CNN
	1    1800 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 1200 1800 1300
Wire Wire Line
	1800 1600 1800 1650
Wire Wire Line
	1800 1650 2050 1650
Connection ~ 1800 1650
Wire Wire Line
	1800 1650 1800 1700
Text Label 2050 1650 2    50   ~ 0
EN
$Comp
L Device:C C4
U 1 1 6179F00E
P 5050 1100
F 0 "C4" H 5165 1146 50  0000 L CNN
F 1 "0.1uf" H 5165 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 5088 950 50  0001 C CNN
F 3 "~" H 5050 1100 50  0001 C CNN
	1    5050 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 6179F977
P 5450 1100
F 0 "C5" H 5565 1146 50  0000 L CNN
F 1 "10uf" H 5565 1055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 5488 950 50  0001 C CNN
F 3 "~" H 5450 1100 50  0001 C CNN
	1    5450 1100
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR018
U 1 1 617A0BCD
P 5250 900
F 0 "#PWR018" H 5250 750 50  0001 C CNN
F 1 "+3V3" H 5265 1073 50  0000 C CNN
F 2 "" H 5250 900 50  0001 C CNN
F 3 "" H 5250 900 50  0001 C CNN
	1    5250 900 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 617A1B15
P 5250 1300
F 0 "#PWR019" H 5250 1050 50  0001 C CNN
F 1 "GND" H 5255 1127 50  0000 C CNN
F 2 "" H 5250 1300 50  0001 C CNN
F 3 "" H 5250 1300 50  0001 C CNN
	1    5250 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 1250 5250 1250
Wire Wire Line
	5250 1250 5250 1300
Wire Wire Line
	5250 1250 5450 1250
Connection ~ 5250 1250
Wire Wire Line
	5250 900  5250 950 
Wire Wire Line
	5250 950  5450 950 
Wire Wire Line
	5250 950  5050 950 
Connection ~ 5250 950 
$Comp
L TeaSteaper:Barrel_Jack_Switch J3
U 1 1 617B3C93
P 1700 6100
F 0 "J3" H 1757 6417 50  0000 C CNN
F 1 "Barrel_Jack_Switch" H 1757 6326 50  0000 C CNN
F 2 "TeaMaker:BarrelJack_CUI_PJ-202A_Horizontal" H 1750 6060 50  0001 C CNN
F 3 "~" H 1750 6060 50  0001 C CNN
	1    1700 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 6200 2000 6550
Wire Wire Line
	2000 6550 2350 6550
Connection ~ 2350 6550
Wire Wire Line
	2000 6000 2350 6000
NoConn ~ 2000 6100
$Comp
L TeaSteaper:Conn_02x03_Odd_Even J1
U 1 1 617D1409
P 1550 3200
F 0 "J1" H 1600 3517 50  0000 C CNN
F 1 "Serial" H 1600 3426 50  0000 C CNN
F 2 "TeaMaker:IDC-Header_2x03_P2.54mm_Vertical" H 1550 3200 50  0001 C CNN
F 3 "~" H 1550 3200 50  0001 C CNN
	1    1550 3200
	1    0    0    -1  
$EndComp
$Comp
L TeaSteaper:Conn_02x05_Odd_Even J2
U 1 1 617D3646
P 1550 3950
F 0 "J2" H 1600 4367 50  0000 C CNN
F 1 "JTAG" H 1600 4276 50  0000 C CNN
F 2 "TeaMaker:IDC-Header_2x05_P2.54mm_Vertical" H 1550 3950 50  0001 C CNN
F 3 "~" H 1550 3950 50  0001 C CNN
	1    1550 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 3100 1100 3100
Wire Wire Line
	1350 3200 1100 3200
Wire Wire Line
	1350 3300 1100 3300
Wire Wire Line
	1850 3300 2100 3300
Wire Wire Line
	1850 3200 2350 3200
Wire Wire Line
	2350 3200 2350 3400
$Comp
L power:GND #PWR014
U 1 1 617DF70A
P 2350 3400
F 0 "#PWR014" H 2350 3150 50  0001 C CNN
F 1 "GND" H 2355 3227 50  0000 C CNN
F 2 "" H 2350 3400 50  0001 C CNN
F 3 "" H 2350 3400 50  0001 C CNN
	1    2350 3400
	1    0    0    -1  
$EndComp
Text Label 2100 3300 2    50   ~ 0
IO0
Text Label 1100 3100 0    50   ~ 0
EN
Text Label 1100 3200 0    50   ~ 0
TXD
Text Label 1100 3300 0    50   ~ 0
RXD
NoConn ~ 1850 3100
NoConn ~ 1350 3750
$Comp
L power:GND #PWR09
U 1 1 617E5A93
P 1250 4250
F 0 "#PWR09" H 1250 4000 50  0001 C CNN
F 1 "GND" H 1255 4077 50  0000 C CNN
F 2 "" H 1250 4250 50  0001 C CNN
F 3 "" H 1250 4250 50  0001 C CNN
	1    1250 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 3850 1250 3850
Wire Wire Line
	1250 3850 1250 3950
Wire Wire Line
	1350 3950 1250 3950
Connection ~ 1250 3950
Wire Wire Line
	1250 3950 1250 4050
Wire Wire Line
	1350 4050 1250 4050
Connection ~ 1250 4050
Wire Wire Line
	1250 4050 1250 4150
Wire Wire Line
	1350 4150 1250 4150
Connection ~ 1250 4150
Wire Wire Line
	1250 4150 1250 4250
Wire Wire Line
	1850 3750 2100 3750
Wire Wire Line
	1850 3850 2100 3850
Wire Wire Line
	1850 3950 2100 3950
Wire Wire Line
	1850 4050 2100 4050
NoConn ~ 1850 4150
Text Label 2100 3750 2    50   ~ 0
TMS
Text Label 2100 3850 2    50   ~ 0
TCK
Text Label 2100 3950 2    50   ~ 0
TDO
Text Label 2100 4050 2    50   ~ 0
TDI
$Comp
L TeaSteaper:Conn_01x08 J4
U 1 1 6181980D
P 9450 1300
F 0 "J4" H 9530 1292 50  0000 L CNN
F 1 "Display" H 9530 1201 50  0000 L CNN
F 2 "TeaMaker:JST_PH_S8B-PH-K_1x08_P2.00mm_Horizontal" H 9450 1300 50  0001 C CNN
F 3 "~" H 9450 1300 50  0001 C CNN
	1    9450 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1200 9000 1200
Wire Wire Line
	9250 1300 9000 1300
Wire Wire Line
	9250 1400 9000 1400
Wire Wire Line
	9250 1500 9000 1500
Wire Wire Line
	9250 1600 9000 1600
Wire Wire Line
	9250 1700 9000 1700
$Comp
L power:PWR_FLAG #FLG02
U 1 1 617452EE
P 1900 7150
F 0 "#FLG02" H 1900 7225 50  0001 C CNN
F 1 "PWR_FLAG" H 1900 7323 50  0000 C CNN
F 2 "" H 1900 7150 50  0001 C CNN
F 3 "~" H 1900 7150 50  0001 C CNN
	1    1900 7150
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG01
U 1 1 61747174
P 1550 7150
F 0 "#FLG01" H 1550 7225 50  0001 C CNN
F 1 "PWR_FLAG" H 1550 7323 50  0000 C CNN
F 2 "" H 1550 7150 50  0001 C CNN
F 3 "~" H 1550 7150 50  0001 C CNN
	1    1550 7150
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR010
U 1 1 61747C05
P 1550 7150
F 0 "#PWR010" H 1550 7000 50  0001 C CNN
F 1 "+5V" H 1565 7323 50  0000 C CNN
F 2 "" H 1550 7150 50  0001 C CNN
F 3 "" H 1550 7150 50  0001 C CNN
	1    1550 7150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 61748871
P 1900 7150
F 0 "#PWR013" H 1900 6900 50  0001 C CNN
F 1 "GND" H 1905 6977 50  0000 C CNN
F 2 "" H 1900 7150 50  0001 C CNN
F 3 "" H 1900 7150 50  0001 C CNN
	1    1900 7150
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0101
U 1 1 6174D62F
P 8850 900
F 0 "#PWR0101" H 8850 750 50  0001 C CNN
F 1 "+3V3" H 8865 1073 50  0000 C CNN
F 2 "" H 8850 900 50  0001 C CNN
F 3 "" H 8850 900 50  0001 C CNN
	1    8850 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1000 8850 1000
Wire Wire Line
	8850 1000 8850 900 
$Comp
L power:GND #PWR0102
U 1 1 6176BA5D
P 8850 1150
F 0 "#PWR0102" H 8850 900 50  0001 C CNN
F 1 "GND" H 8855 977 50  0000 C CNN
F 2 "" H 8850 1150 50  0001 C CNN
F 3 "" H 8850 1150 50  0001 C CNN
	1    8850 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1100 8850 1100
Wire Wire Line
	8850 1100 8850 1150
Text Label 9000 1200 0    50   ~ 0
MOSI
Text Label 9000 1300 0    50   ~ 0
SCLK
Text Label 9000 1400 0    50   ~ 0
CS
Text Label 9000 1500 0    50   ~ 0
DC
Text Label 9000 1600 0    50   ~ 0
RST
Text Label 9000 1700 0    50   ~ 0
BL
Wire Wire Line
	7200 5400 7550 5400
Wire Wire Line
	7200 5500 7550 5500
Wire Wire Line
	7200 5600 7550 5600
Wire Wire Line
	7200 5700 7550 5700
Text Label 7550 5700 2    50   ~ 0
P_M1D
Text Label 7550 5600 2    50   ~ 0
P_M1D
Text Label 7550 5400 2    50   ~ 0
P_M1C
Text Label 7550 5200 2    50   ~ 0
P_M1B
Text Label 7550 5100 2    50   ~ 0
P_M1A
Wire Wire Line
	6400 5400 6000 5400
Wire Wire Line
	6400 5500 6000 5500
Wire Wire Line
	6400 5600 6000 5600
Wire Wire Line
	6400 5700 6000 5700
Text Label 6000 5600 0    50   ~ 0
M1D
Text Label 6000 5400 0    50   ~ 0
M1C
Text Label 6000 5200 0    50   ~ 0
M1B
Text Label 6000 5100 0    50   ~ 0
M1A
$Comp
L TeaSteaper:Rotary_Encoder_Switch SW2
U 1 1 617DE08A
P 7250 3450
F 0 "SW2" H 7250 3817 50  0000 C CNN
F 1 "Rotary_Encoder_Switch" H 7250 3726 50  0000 C CNN
F 2 "TeaMaker:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm" H 7100 3610 50  0001 C CNN
F 3 "~" H 7250 3710 50  0001 C CNN
	1    7250 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 1700 4700 1700
Wire Wire Line
	4450 2800 4700 2800
$Comp
L power:GND #PWR08
U 1 1 61834C5F
P 7650 3650
F 0 "#PWR08" H 7650 3400 50  0001 C CNN
F 1 "GND" H 7655 3477 50  0000 C CNN
F 2 "" H 7650 3650 50  0001 C CNN
F 3 "" H 7650 3650 50  0001 C CNN
	1    7650 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 61835457
P 6550 3650
F 0 "#PWR04" H 6550 3400 50  0001 C CNN
F 1 "GND" H 6555 3477 50  0000 C CNN
F 2 "" H 6550 3650 50  0001 C CNN
F 3 "" H 6550 3650 50  0001 C CNN
	1    6550 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 3450 6550 3450
Wire Wire Line
	6550 3450 6550 3650
Wire Wire Line
	7550 3550 7650 3550
Wire Wire Line
	7650 3550 7650 3650
Wire Wire Line
	7550 3350 7850 3350
Wire Wire Line
	6950 3550 6650 3550
Wire Wire Line
	6950 3350 6650 3350
Text Label 6650 3350 0    50   ~ 0
ROTA
Text Label 6650 3550 0    50   ~ 0
ROTB
Text Label 7850 3350 2    50   ~ 0
ROTSW
Text Label 4700 1700 2    50   ~ 0
ROTSW
Text Label 4700 2500 2    50   ~ 0
ROTB
Text Label 4700 2800 2    50   ~ 0
ROTA
$Comp
L TeaSteaper:Conn_01x05 J5
U 1 1 61869D41
P 9250 5250
F 0 "J5" H 9330 5292 50  0000 L CNN
F 1 "Conn_01x05" H 9330 5201 50  0000 L CNN
F 2 "TeaMaker:JST_XH_B5B-XH-A_1x05_P2.50mm_Vertical" H 9250 5250 50  0001 C CNN
F 3 "~" H 9250 5250 50  0001 C CNN
	1    9250 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 5150 8650 5150
Wire Wire Line
	9050 5250 8650 5250
Wire Wire Line
	9050 5350 8650 5350
Wire Wire Line
	9050 5450 8650 5450
$Comp
L power:+5V #PWR020
U 1 1 6188A8AA
P 8650 4950
F 0 "#PWR020" H 8650 4800 50  0001 C CNN
F 1 "+5V" H 8665 5123 50  0000 C CNN
F 2 "" H 8650 4950 50  0001 C CNN
F 3 "" H 8650 4950 50  0001 C CNN
	1    8650 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9050 5050 8650 5050
Wire Wire Line
	8650 5050 8650 4950
Text Label 8650 5450 0    50   ~ 0
P_M1A
Text Label 8650 5350 0    50   ~ 0
P_M1B
Text Label 8650 5250 0    50   ~ 0
P_M1C
Text Label 8650 5150 0    50   ~ 0
P_M1D
Wire Wire Line
	1800 2000 1800 2100
$EndSCHEMATC
