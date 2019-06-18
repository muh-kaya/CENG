#include "p18f8722.inc"
; CONFIG1H
  CONFIG  OSC = HSPLL, FCMEN = OFF, IESO = OFF
; CONFIG2L
  CONFIG  PWRT = OFF, BOREN = OFF, BORV = 3
; CONFIG2H
  CONFIG  WDT = OFF, WDTPS = 32768
; CONFIG3L
  CONFIG  MODE = MC, ADDRBW = ADDR20BIT, DATABW = DATA16BIT, WAIT = OFF
; CONFIG3H
  CONFIG  CCP2MX = PORTC, ECCPMX = PORTE, LPT1OSC = OFF, MCLRE = ON
; CONFIG4L
  CONFIG  STVREN = ON, LVP = OFF, BBSIZ = BB2K, XINST = OFF
; CONFIG5L
  CONFIG  CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF, CP4 = OFF, CP5 = OFF
  CONFIG  CP6 = OFF, CP7 = OFF
; CONFIG5H
  CONFIG  CPB = OFF, CPD = OFF
; CONFIG6L
  CONFIG  WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF, WRT4 = OFF
  CONFIG  WRT5 = OFF, WRT6 = OFF, WRT7 = OFF
; CONFIG6H
  CONFIG  WRTC = OFF, WRTB = OFF, WRTD = OFF
; CONFIG7L
  CONFIG  EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF, EBTR4 = OFF
  CONFIG  EBTR5 = OFF, EBTR6 = OFF, EBTR7 = OFF
; CONFIG7H
  CONFIG  EBTRB = OFF

;*******************************************************************************
; Variables & Constants
;*******************************************************************************
UDATA_ACS
    t_1	    res 1	
    t_2	    res 1	
    t_3	    res 1
	    
    state   res 1	
    state2  res 1
  
    cond    res 1
    check   res 1
   

v1 UDATA 0x20
    cmp

;*******************************************************************************
; Reset Vector
;*******************************************************************************

RES_VECT  CODE    0x0000            
    GOTO    START                   

;*******************************************************************************
; MAIN PROGRAM
;*******************************************************************************

MAIN_PROG CODE


START
    call INIT	
    call DELAY_2sec

    CLRF LATA
    CLRF LATB
    CLRF LATC
    CLRF LATD

    MOVLW 0x00
    MOVWF cmp

    MOVLW 0x01

    MOVWF   LATA
    MOVWF   LATB
    call DELAY_C

MAIN_LOOP

    MOVLW 0x00

    call BUTTON_TASK

    CLRF WREG	
    CPFSEQ state
    GOTO _stateCC

    _stateC:
	
	MOVLW 0x0
	CPFSEQ cmp
	GOTO _cond2
	GOTO _line1

	_cond2:
	    MOVLW 0x1
	    CPFSEQ cmp
	    GOTO _cond3
	    GOTO _line2

	_cond3:
	    call BUTTON_TASK_2
	    CLRF WREG
	    CPFSEQ state2
	    GOTO _line3
	    GOTO MAIN_LOOP


        _line1:
            INCF cmp, 1
            BTFSC PORTA, 0
            GOTO _porta
            BTFSC PORTA, 3
            GOTO _porta3
            BTFSC PORTD, 0
            GOTO _portd
            GOTO _portd3

            _porta:
                CLRF LATA
                CLRF LATB

                MOVLW 0x1
                MOVWF LATB
                MOVWF LATC
                GOTO _state_end
            _porta3:
                CLRF LATA

                MOVLW 0x6
                MOVWF LATA
                GOTO _state_end

            _portd:
                CLRF LATD

                MOVLW 0x6
                MOVWF LATD
                GOTO _state_end

            _portd3:
                CLRF LATC
                CLRF LATD

                MOVLW 0x8
                MOVWF LATB
                MOVWF LATC
                GOTO _state_end


        _line2:
            INCF cmp, 1
            BTFSC PORTB, 0
            GOTO _portb
            BTFSC PORTB, 3
            GOTO _portb3
            BTFSC PORTD, 1
            GOTO _portd1
            GOTO _porta1


            _portb:
                CLRF LATB
                CLRF LATC

                MOVLW 0x1
                MOVWF LATC
                MOVWF LATD
                GOTO _state_end


            _portb3:
                CLRF LATB
                CLRF LATC

                MOVLW 0x8
                MOVWF LATA
                MOVWF LATB
                GOTO _state_end


            _portd1:
                CLRF LATD

                MOVLW 0xC
                MOVWF LATD
                GOTO _state_end

            _porta1:
                CLRF LATA

                MOVLW 0x3
                MOVWF LATA
                GOTO _state_end


       _line3:
            MOVLW 0x0
            MOVWF cmp
            CLRF  state2

            BTFSC PORTC, 0
            GOTO _portc
            BTFSC PORTD, 2
            GOTO _portd2
            BTFSC PORTB, 3
            GOTO _portb32
            GOTO _porta12

            _portc:
                CLRF LATC
                CLRF LATD

                MOVLW 0x3
                MOVWF LATD
                GOTO _state_end


            _portd2:
                CLRF LATD

                MOVLW 0x8
                MOVWF LATC
                MOVWF LATD
                GOTO _state_end


            _portb32:
                CLRF LATA
                CLRF LATB

                MOVLW 0xC
                MOVWF LATA
                GOTO _state_end

            _porta12:
                CLRF LATA

                MOVLW 0x1
                MOVWF LATA
                MOVWF LATB

                GOTO _state_end

        _state_end:

            call DELAY_C

            GOTO MAIN_LOOP



    _stateCC:

	MOVLW 0x2
	CPFSEQ cmp
	GOTO _cond2_2
	GOTO _line3_2

	_cond2_2:
	    MOVLW 0x1
	    CPFSEQ cmp
	    GOTO _cond3_2
	    GOTO _line2_2

	_cond3_2:
        call BUTTON_TASK_2
	    CLRF WREG
	    CPFSEQ state2
	    GOTO _line1_2
	    GOTO MAIN_LOOP


	_line1_2:

            MOVLW 0x2
            MOVWF cmp
            CLRF  state2

            BTFSC PORTA, 0
            GOTO _porta_cc
            BTFSC PORTD, 0
            GOTO _portd_cc
            BTFSC PORTD, 3
            GOTO _portd3_cc
            GOTO _porta3_cc

            _porta_cc:
                CLRF LATA
                CLRF LATB

                MOVLW 0x3
                MOVWF LATA
                GOTO _state_end_2


            _portd_cc:
                CLRF LATD

                MOVLW 0x1
                MOVWF LATC
                MOVWF LATD
                GOTO _state_end_2


            _portd3_cc:
                CLRF LATC
                CLRF LATD

                MOVLW 0xC
                MOVWF LATD
                GOTO _state_end_2

            _porta3_cc:
                CLRF LATA

                MOVLW 0x8
                MOVWF LATA
                MOVWF LATB

                GOTO _state_end_2


	_line2_2:

            DECF cmp, 1
            BTFSC PORTB, 0
            GOTO _portb_2
            BTFSC PORTB, 3
            GOTO _portb3_2
            BTFSC PORTD, 1
            GOTO _portd1_2
            GOTO _porta1_2

            _portb_2:
                CLRF LATB
                CLRF LATC

                MOVLW 0x1
                MOVWF LATA
                MOVWF LATB
                GOTO _state_end_2


            _portb3_2:
                CLRF LATB
                CLRF LATC

                MOVLW 0x8
                MOVWF LATC
                MOVWF LATD
                GOTO _state_end_2


            _portd1_2:
                CLRF LATD

                MOVLW 0x3
                MOVWF LATD
                GOTO _state_end_2

            _porta1_2:
                CLRF LATA

                MOVLW 0xC
                MOVWF LATA
                GOTO _state_end_2

	_line3_2:

            DECF cmp, 1

            BTFSC PORTC, 0
            GOTO _portc_2
            BTFSC PORTD, 2
            GOTO _portd2_2
            BTFSC PORTB, 3
            GOTO _portb32_2
            GOTO _porta12_2

            _portc_2:
                CLRF LATC
                CLRF LATD

                MOVLW 0x1
                MOVWF LATB
                MOVWF LATC
                GOTO _state_end_2


            _portd2_2:
                CLRF LATD

                MOVLW 0x6
                MOVWF LATD
                GOTO _state_end_2


            _portb32_2:
                CLRF LATA
                CLRF LATB

                MOVLW 0x8
                MOVWF LATB
                MOVWF LATC
                GOTO _state_end_2

            _porta12_2:
                CLRF LATA

                MOVLW 0x6
                MOVWF LATA

                GOTO _state_end_2


        _state_end_2:

            call DELAY_CC	

            GOTO MAIN_LOOP  
	    
	    
DELAY_2sec
MOVLW	0x14
MOVWF	t_1
		
_loop_1:
    MOVLW   0xD4
    MOVWF   t_2
    
    _loop_2:
	MOVLW	0xBD
	MOVWF	t_3
	
	_loop_3:
	    call BUTTON_TASK
	    call BUTTON_TASK_2
	    
	    DECFSZ  t_3, F
	    GOTO    _loop_3
	    
	    DECFSZ  t_2, F
	    GOTO    _loop_2
	    
	    DECFSZ  t_1, F
	    GOTO    _loop_1
	    
	    return

	    
DELAY_C
MOVLW	8
MOVWF	t_1
		
_loop_c_1:
    MOVLW   0xBF
    MOVWF   t_2
    
    _loop_c_2:
	MOVLW	0xC4
	MOVWF	t_3
	
	_loop_c_3:
	    call BUTTON_TASK
	    call BUTTON_TASK_2
	    
	    DECFSZ  t_3, F
	    GOTO    _loop_c_3
	    
	    DECFSZ  t_2, F
	    GOTO    _loop_c_2
	    
	    DECFSZ  t_1, F
	    GOTO    _loop_c_1
	    
	    return		


DELAY_CC
MOVLW	4
MOVWF	t_1
		
_loop_cc_1:
    MOVLW   0xBF
    MOVWF   t_2
    
    _loop_cc_2:
	MOVLW	0xD2
	MOVWF	t_3
	
	_loop_cc_3:
    	    call BUTTON_TASK
	    call BUTTON_TASK_2
	    
	    DECFSZ  t_3, F
	    GOTO    _loop_cc_3
	    
	    DECFSZ  t_2, F
	    GOTO    _loop_cc_2
	    
	    DECFSZ  t_1, F
	    GOTO    _loop_cc_1
	    
	    return
	


INIT
    CLRF    state
    CLRF    state2
    CLRF    cond
    CLRF    check

    MOVLW   0Fh
    MOVWF   ADCON1

    MOVLW   0xF
    MOVWF   LATA
    MOVWF   LATB
    MOVWF   LATC
    MOVWF   LATD

    MOVLW   0x10
    MOVWF   TRISA

    MOVLW   0x20
    MOVWF   TRISB

    CLRF    TRISC
    CLRF    TRISD

    return


BUTTON_TASK
    BTFSS PORTA,4
    GOTO _case
    GOTO _case2

        _case:
            CLRF    WREG
            CPFSEQ  cond
            GOTO    _end2
            return

        _case2:
            MOVLW   0x0
            CPFSEQ  cond
            GOTO    _end
            COMF    cond, 1


        _end:
            BTFSC   PORTA, 4
            return
            GOTO    _end2
                _end2:
                    COMF    cond, 1
                    COMF    state, 1
                    return


BUTTON_TASK_2 
    BTFSS PORTB,5
    GOTO _check
    GOTO _check1

        _check:
            CLRF    WREG
            CPFSEQ  check
            GOTO    _end1_2
            return

        _check1:
            MOVLW   0x0
            CPFSEQ  check
            GOTO    _end1
            COMF    check, 1


        _end1:
            BTFSC   PORTB, 5
            return
            GOTO    _end1_2
                _end1_2:
                    COMF    check, 1
		    GOTO    _end1_1
                    return
			_end1_1:
			    MOVLW   0x0
			    CPFSEQ  state
			    GOTO    _end1_cc
			    GOTO    _end1_c
			    
				_end1_cc:
				    MOVLW   0x0
				    CPFSEQ  cmp
				    return
				    COMF    state2, 1
				    return
				
				_end1_c:
				    MOVLW   0x2
				    CPFSEQ  cmp
				    return
				    COMF    state2, 1
				    return
			    		    
END
