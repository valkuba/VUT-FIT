; Autor reseni: Jmeno Prijmeni login

; Projekt 2 - INP 2022
; Vernamova sifra na architekture MIPS64

; mozem to dat ak ako nejaky key:   .asciiz "ja"


; DATA SEGMENT
                .data
login:          .asciiz "xvales04"  ; sem doplnte vas login
cipher:         .space  17  ; misto pro zapis sifrovaneho loginu
key1:		.asciiz "v"
key2:		.asciiz "a"

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize "funkce" print_string)

; CODE SEGMENT
                .text

                ; ZDE NAHRADTE KOD VASIM RESENIM
main:
		daddi	r27, r27, 0 ; checking
		daddi	r11, r11, 0 ; changing loginos
		daddi	r29, r29, 0 ; counteros
		daddi	r22, r22, 0 ; check z
		daddi	r0, r0, 0
		daddi	r4, r4, 0

		

loop_check:
		lb r11, login(r29)	; nacteni do r11 login na indexu r10
		daddi r11, r11, -97	; je pismeno? a zaroven spravny posun
		bgez r11, even_check
		b exit

even_check:
		andi r27, r29, 0x0001
		bnez r27, odd
		lb r27, key1(r0)
		dadd r11, r11, r27
		daddi r11, r11, 1
		b end_check
odd:
		lb r27, key2(r0)
		daddi r27, r27, -97
		daddi r11, r11, 96
		dsub r11, r11, r27	; sifra


end_check:
		;
		;daddi r11, r11, -96

		;daddi r27, r27, 97

		daddi r22, r11, -123
		bgez r22, bigger_than_z
		b loop_conti
bigger_than_z:
		daddi r11, r11, -26	

loop_conti:
		daddi r22, r11, -97
		bgez r22, loop_end
smaller_than_a:
		daddi r11, r11, 26


loop_end:
		sb r11, cipher(r29)	; ulozeni do sifry
		daddi r29, r29, 1	; iterace
		b loop_check		; jump na navesti loop
		
		
exit:
                daddi   r4, r0, cipher  ;  vozrovy vypis: adresa login: do r4
                jal     print_string    ; vypis pomoci print_string - viz nize


                syscall 0   ; halt

print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
