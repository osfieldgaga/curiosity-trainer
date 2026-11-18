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

; -------------------------
; Send '0' bit (T0H = 24 cycles)
; -------------------------
bit0_high:
    ; HIGH for 24 cycles total
    repeat  #20               ; (20 + 4 cycles overhead = 24)
    nop

    bclr    LATB, #9          ; LOW

    ; LOW for 72 cycles
    repeat  #68               ; (68 + 4 = 72)
    nop

    bra     bit_done

; -------------------------
; Send '1' bit (T1H = 48 cycles)
; -------------------------
bit1_high:
    ; HIGH for 48 cycles
    repeat  #44               ; (44 + 4 = 48)
    nop

    bclr    LATB, #9          ; LOW

    ; LOW for 48 cycles
    repeat  #44               ; (44 + 4 = 48)
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


