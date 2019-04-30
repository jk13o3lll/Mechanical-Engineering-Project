Since the L293D shield we used would occupy some PWM pins even it doesn't need PWM,
we changed some pin to digital pin on Arduino mega.
We solder some new circuit for connecting pins and change the default pin number in L293D library (definition in header file).

Original pin for DC:
Pins always used:
    12(motor latch)
    4(motor clock)
    7(motor enable)
    8(motor data)

Used for each: 11(M1), 3(M2), 6(M3), 5(M4)
    pin 11,3,5,6 �O�Ψӽճt�� pwm

Which pin still available?
0:RX    	X
1:TX    	X
2:      	O
3:PWM   	X
4:clk   	V
5:PWM   	X
6:PWM   	X
7:ena   	V
8:data  	V
9:servo 	-
10:servo	-
11:PWM		X
12:latch	V
13:		O

analog_in is all available

Avaliable pins:
2:      	O
4:clk   	V
7:ena   	V
8:data  	V
12:latch	V
13:		O


we choose 2,4,7,13
2(null)->
4(clk)->mega 22
7(ena)->mega 23
13(null)->

�n�k��PIN(L293D):2,4,7,13,VIN, 5Vout(���Ū�
�n�s����PIN(L293D->MEGA):4->22, 7->23
L298N�����qMEGA�O����:2,4,7,13,VIN
�]����L298N���q��MEGA (5V->VIN)�AL293D�����u�n�ޱ��A����p�q�����q��MEGA