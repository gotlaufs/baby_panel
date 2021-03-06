EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:hc595_expander-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Conn_01x02_Male J1
U 1 1 5A37FB2A
P 1850 3100
F 0 "J1" H 1850 3200 50  0000 C CNN
F 1 "Conn_01x02_Male" H 1850 2900 50  0000 C CNN
F 2 "baby_panel_footprints:Header_1x02_2.54mm_Oval" H 1850 3100 50  0001 C CNN
F 3 "" H 1850 3100 50  0001 C CNN
	1    1850 3100
	1    0    0    -1  
$EndComp
Text Notes 1750 2900 0    60   ~ 0
Power In\n
$Comp
L VCC #PWR01
U 1 1 5A37FBC2
P 2250 2950
F 0 "#PWR01" H 2250 2800 50  0001 C CNN
F 1 "VCC" H 2250 3100 50  0000 C CNN
F 2 "" H 2250 2950 50  0001 C CNN
F 3 "" H 2250 2950 50  0001 C CNN
	1    2250 2950
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG02
U 1 1 5A37FBD8
P 2550 2950
F 0 "#FLG02" H 2550 3025 50  0001 C CNN
F 1 "PWR_FLAG" H 2550 3100 50  0000 C CNN
F 2 "" H 2550 2950 50  0001 C CNN
F 3 "" H 2550 2950 50  0001 C CNN
	1    2550 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 3100 3000 3100
Wire Wire Line
	2250 3100 2250 2950
Wire Wire Line
	2550 3100 2550 2950
Connection ~ 2250 3100
$Comp
L GND #PWR03
U 1 1 5A37FBF9
P 2250 3450
F 0 "#PWR03" H 2250 3200 50  0001 C CNN
F 1 "GND" H 2250 3300 50  0000 C CNN
F 2 "" H 2250 3450 50  0001 C CNN
F 3 "" H 2250 3450 50  0001 C CNN
	1    2250 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 3450 2250 3200
Wire Wire Line
	2050 3200 2550 3200
$Comp
L PWR_FLAG #FLG04
U 1 1 5A37FC16
P 2550 3450
F 0 "#FLG04" H 2550 3525 50  0001 C CNN
F 1 "PWR_FLAG" H 2550 3600 50  0000 C CNN
F 2 "" H 2550 3450 50  0001 C CNN
F 3 "" H 2550 3450 50  0001 C CNN
	1    2550 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	2550 3200 2550 3450
Connection ~ 2250 3200
$Comp
L Conn_01x04_Male J2
U 1 1 5A37FC8B
P 1850 4300
F 0 "J2" H 1850 4500 50  0000 C CNN
F 1 "Conn_01x04_Male" H 1850 4000 50  0000 C CNN
F 2 "baby_panel_footprints:Header_1x04_2.54mm_Oval" H 1850 4300 50  0001 C CNN
F 3 "" H 1850 4300 50  0001 C CNN
	1    1850 4300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 5A37FCEC
P 2250 4750
F 0 "#PWR05" H 2250 4500 50  0001 C CNN
F 1 "GND" H 2250 4600 50  0000 C CNN
F 2 "" H 2250 4750 50  0001 C CNN
F 3 "" H 2250 4750 50  0001 C CNN
	1    2250 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2250 4750 2250 4500
Wire Wire Line
	2250 4500 2050 4500
$Comp
L 74HC595 U1
U 1 1 5A37FD24
P 5700 3700
F 0 "U1" H 5850 4300 50  0000 C CNN
F 1 "74HC595" H 5700 3100 50  0000 C CNN
F 2 "baby_panel_footprints:SOIC-16W_7.5x10.3mm_Pitch1.27mm_HandSoldering" H 5700 3700 50  0001 C CNN
F 3 "" H 5700 3700 50  0001 C CNN
	1    5700 3700
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5A37FEBA
P 3000 3300
F 0 "C1" H 3025 3400 50  0000 L CNN
F 1 "100n" H 3025 3200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3038 3150 50  0001 C CNN
F 3 "" H 3000 3300 50  0001 C CNN
	1    3000 3300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 5A37FF03
P 3000 3550
F 0 "#PWR06" H 3000 3300 50  0001 C CNN
F 1 "GND" H 3000 3400 50  0000 C CNN
F 2 "" H 3000 3550 50  0001 C CNN
F 3 "" H 3000 3550 50  0001 C CNN
	1    3000 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 3550 3000 3450
Wire Wire Line
	3000 3100 3000 3150
Connection ~ 2550 3100
$Comp
L R R1
U 1 1 5A380082
P 6800 3250
F 0 "R1" V 6750 3400 50  0000 C CNN
F 1 "200" V 6800 3250 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6730 3250 50  0001 C CNN
F 3 "" H 6800 3250 50  0001 C CNN
	1    6800 3250
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 5A38015C
P 6800 3350
F 0 "R2" V 6750 3500 50  0000 C CNN
F 1 "200" V 6800 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6730 3350 50  0001 C CNN
F 3 "" H 6800 3350 50  0001 C CNN
	1    6800 3350
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 5A38017E
P 6800 3450
F 0 "R3" V 6750 3600 50  0000 C CNN
F 1 "200" V 6800 3450 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6730 3450 50  0001 C CNN
F 3 "" H 6800 3450 50  0001 C CNN
	1    6800 3450
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 5A3801A3
P 6800 3550
F 0 "R4" V 6750 3700 50  0000 C CNN
F 1 "200" V 6800 3550 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6730 3550 50  0001 C CNN
F 3 "" H 6800 3550 50  0001 C CNN
	1    6800 3550
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 5A3801CB
P 6800 3650
F 0 "R5" V 6750 3800 50  0000 C CNN
F 1 "200" V 6800 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6730 3650 50  0001 C CNN
F 3 "" H 6800 3650 50  0001 C CNN
	1    6800 3650
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 5A3801F6
P 6800 3750
F 0 "R6" V 6750 3900 50  0000 C CNN
F 1 "200" V 6800 3750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6730 3750 50  0001 C CNN
F 3 "" H 6800 3750 50  0001 C CNN
	1    6800 3750
	0    1    1    0   
$EndComp
$Comp
L R R7
U 1 1 5A380224
P 6800 3850
F 0 "R7" V 6750 4000 50  0000 C CNN
F 1 "200" V 6800 3850 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6730 3850 50  0001 C CNN
F 3 "" H 6800 3850 50  0001 C CNN
	1    6800 3850
	0    1    1    0   
$EndComp
$Comp
L R R8
U 1 1 5A380255
P 6800 3950
F 0 "R8" V 6750 4100 50  0000 C CNN
F 1 "200" V 6800 3950 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6730 3950 50  0001 C CNN
F 3 "" H 6800 3950 50  0001 C CNN
	1    6800 3950
	0    1    1    0   
$EndComp
Wire Wire Line
	6400 3250 6650 3250
Wire Wire Line
	6650 3350 6400 3350
Wire Wire Line
	6400 3450 6650 3450
Wire Wire Line
	6650 3550 6400 3550
Wire Wire Line
	6400 3650 6650 3650
Wire Wire Line
	6650 3750 6400 3750
Wire Wire Line
	6400 3850 6650 3850
Wire Wire Line
	6650 3950 6400 3950
Wire Wire Line
	2050 4200 3850 4200
Wire Wire Line
	3850 4200 3850 3250
Wire Wire Line
	3850 3250 5000 3250
Wire Wire Line
	2050 4300 4750 4300
Wire Wire Line
	3950 3450 5000 3450
Wire Wire Line
	2050 4400 4650 4400
Wire Wire Line
	4050 3750 5000 3750
$Comp
L VCC #PWR07
U 1 1 5A380922
P 4800 3100
F 0 "#PWR07" H 4800 2950 50  0001 C CNN
F 1 "VCC" H 4800 3250 50  0000 C CNN
F 2 "" H 4800 3100 50  0001 C CNN
F 3 "" H 4800 3100 50  0001 C CNN
	1    4800 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3850 4800 3850
Wire Wire Line
	4800 3850 4800 3100
$Comp
L VCC #PWR08
U 1 1 5A3809C2
P 4950 3100
F 0 "#PWR08" H 4950 2950 50  0001 C CNN
F 1 "VCC" H 4950 3250 50  0000 C CNN
F 2 "" H 4950 3100 50  0001 C CNN
F 3 "" H 4950 3100 50  0001 C CNN
	1    4950 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3100 4950 3550
Wire Wire Line
	4950 3550 5000 3550
$Comp
L Conn_01x04_Male J3
U 1 1 5A380C46
P 6900 4800
F 0 "J3" H 6900 5000 50  0000 C CNN
F 1 "Conn_01x04_Male" H 6900 4500 50  0000 C CNN
F 2 "baby_panel_footprints:Header_1x04_2.54mm_Oval" H 6900 4800 50  0001 C CNN
F 3 "" H 6900 4800 50  0001 C CNN
	1    6900 4800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6400 4150 6550 4150
Wire Wire Line
	6550 4150 6550 4700
Wire Wire Line
	6550 4700 6700 4700
Wire Wire Line
	6700 4800 4750 4800
Wire Wire Line
	4750 4800 4750 4300
Wire Wire Line
	6700 4900 4650 4900
Wire Wire Line
	4650 4900 4650 4400
$Comp
L GND #PWR09
U 1 1 5A380EC6
P 6500 5250
F 0 "#PWR09" H 6500 5000 50  0001 C CNN
F 1 "GND" H 6500 5100 50  0000 C CNN
F 2 "" H 6500 5250 50  0001 C CNN
F 3 "" H 6500 5250 50  0001 C CNN
	1    6500 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 5250 6500 5000
Wire Wire Line
	6500 5000 6700 5000
$Comp
L Conn_01x09_Male J4
U 1 1 5A3813F0
P 7550 3650
F 0 "J4" H 7550 4150 50  0000 C CNN
F 1 "Conn_01x09_Male" H 7550 3150 50  0000 C CNN
F 2 "baby_panel_footprints:Header_1x09_2.54mm_Oval" H 7550 3650 50  0001 C CNN
F 3 "" H 7550 3650 50  0001 C CNN
	1    7550 3650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6950 3250 7350 3250
Wire Wire Line
	7350 3350 6950 3350
Wire Wire Line
	6950 3450 7350 3450
Wire Wire Line
	7350 3550 6950 3550
Wire Wire Line
	6950 3650 7350 3650
Wire Wire Line
	7350 3750 6950 3750
Wire Wire Line
	6950 3850 7350 3850
Wire Wire Line
	7350 3950 6950 3950
$Comp
L GND #PWR010
U 1 1 5A381D32
P 7200 4250
F 0 "#PWR010" H 7200 4000 50  0001 C CNN
F 1 "GND" H 7200 4100 50  0000 C CNN
F 2 "" H 7200 4250 50  0001 C CNN
F 3 "" H 7200 4250 50  0001 C CNN
	1    7200 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 4250 7200 4050
Wire Wire Line
	7200 4050 7350 4050
$Comp
L Conn_01x04_Male J5
U 1 1 5A381EDE
P 7600 4900
F 0 "J5" H 7600 5100 50  0000 C CNN
F 1 "Conn_01x04_Male" H 7600 4600 50  0000 C CNN
F 2 "baby_panel_footprints:Header_1x04_2.54mm_Oval" H 7600 4900 50  0001 C CNN
F 3 "" H 7600 4900 50  0001 C CNN
	1    7600 4900
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR011
U 1 1 5A381F99
P 7250 5250
F 0 "#PWR011" H 7250 5000 50  0001 C CNN
F 1 "GND" H 7250 5100 50  0000 C CNN
F 2 "" H 7250 5250 50  0001 C CNN
F 3 "" H 7250 5250 50  0001 C CNN
	1    7250 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 5250 7250 4700
Wire Wire Line
	7250 4700 7400 4700
Wire Wire Line
	7400 4800 7250 4800
Connection ~ 7250 4800
Wire Wire Line
	7400 4900 7250 4900
Connection ~ 7250 4900
Wire Wire Line
	7400 5000 7250 5000
Connection ~ 7250 5000
Text Notes 1800 5100 0    60   ~ 0
Data In
Text Notes 6450 5600 0    60   ~ 0
Data Out
Text Notes 7450 5300 0    60   ~ 0
Additional GND
Text Notes 7750 3550 0    60   ~ 0
LED Out
Wire Wire Line
	4050 3750 4050 4300
Connection ~ 4050 4300
Wire Wire Line
	3950 3450 3950 4400
Connection ~ 3950 4400
$EndSCHEMATC
