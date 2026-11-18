    .global _sk6812_sendBuffer

_sk6812_sendBuffer:
    ; W0 = buf pointer
    ; W1 = length (bytes)

    ; Ensure RB9 is output
    bclr    TRISB, #9

next_byte:
    cp0     W1
    bra     Z, done
    dec     W1, W1

    mov.b   [W0++], W2        ; Load next byte into W2
    mov     #8, W3            ; Bit counter = 8

next_bit:
    sl      W2, W2            ; Shift MSB into carry

    bset    LATB, #9          ; Start HIGH pulse

    bra     C, bit1_high      ; If carry=1 ? send '1'
;    rlc W2, W2

; -------------------------
; Send '0' bit (T0H = 28 cycles)
; -------------------------
bit0_high:
    ; HIGH for 28 cycles total
    repeat  #30               ; (24 + 4 cycles overhead = 28)
    nop

    bclr    LATB, #9          ; LOW

    ; LOW for 64 cycles
    repeat  #64               ; (60 + 4 = 64)
    nop

    bra     bit_done

; -------------------------
; Send '1' bit (T1H = 56 cycles)
; -------------------------
bit1_high:
    ; HIGH for 56 cycles
    repeat  #60               ; (52 + 4 = 56)
    nop

    bclr    LATB, #9          ; LOW

    ; LOW for 48 cycles
    repeat  #50               ; (44 + 4 = 48)
    nop

bit_done:
    dec     W3, W3
    bra     NZ, next_bit

    bra     next_byte

done:
    ; Reset pulse: >80 µs
    ; 80 µs × 80 MHz = 6400 cycles
    repeat  #6396
    nop

    return


