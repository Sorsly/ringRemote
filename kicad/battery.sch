EESchema Schematic File Version 4
LIBS:ringRemote-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5850 4250 5850 4350
Wire Wire Line
	5850 3750 5850 3850
$Comp
L ringRemote-eagle-import:ML621S U$?
U 1 0 5C491E03
P 5850 4050
AR Path="/5C491E03" Ref="U$?"  Part="1" 
AR Path="/5C488A08/5C491E03" Ref="U$5"  Part="1" 
F 0 "U$5" H 5850 4220 70  0000 C BNN
F 1 "ML621S" H 5850 3880 70  0000 C TNN
F 2 "ringRemote:ML621S" H 5850 4050 50  0001 C CNN
F 3 "" H 5850 4050 50  0001 C CNN
	1    5850 4050
	0    1    1    0   
$EndComp
Text HLabel 5850 3750 1    50   BiDi ~ 0
VBAT
Text HLabel 5850 4350 3    50   BiDi ~ 0
GND
$EndSCHEMATC
