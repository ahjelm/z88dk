; Memory map and section setup
;
; Contains the generic variables + features

;
; crt_model = 0		; everything in RAM
; crt_model = 1		; ROM model, data section copied
; crt_model = 2		; ROM model, data section compressed

		SECTION CODE
		SECTION code_crt_init
crt0_init_bss:
	; TODO: Clear bss area
        xor     a               ;Reset atexit() count
        ld      (exitcount),a
IF !DEFINED_nostreams
	; Setup std* streams
        ld      hl,__sgoioblk
        ld      de,__sgoioblk+1
        ld      bc,39
        ld      (hl),0
        ldir
        ld      hl,__sgoioblk+2
        ld      (hl),19 ;stdin
        ld      hl,__sgoioblk+6
        ld      (hl),21 ;stdout
        ld      hl,__sgoioblk+10
        ld      (hl),21 ;stderr
ENDIF
IF DEFINED_USING_amalloc
        EXTERN ASMTAIL
	ld	hl,ASMTAIL
	ld	(_heap),hl
ENDIF
IF ( __crt_model & 1 )
	; Just copy the DATA section
	EXTERN	ASMTAIL_ROMABLE_END
	EXTERN	ASMHEAD_DATA
	EXTERN	ASMHEAD_DATA_END
	ld	hl,ASMTAIL_ROMABLE_END
	ld	de,ASMHEAD_DATA
	ld	bc,ASMHEAD_DATA_END - ASMHEAD_DATA
	ldir
ENDIF
IF ( __crt_model & 2 )
	; Decompress the DATA section
	EXTERN	ASMTAIL_ROMABLE_END
	EXTERN	ASMHEAD_DATA
	EXTERN	asm_dzx7_standard
	ld	hl,ASMTAIL_ROMABLE_END
	ld	de,ASMHEAD_DATA
	call	asm_dzx7_standard
ENDIF
	
	; SDCC initialiation code gets placed here
		SECTION code_crt_exit

	ret
		SECTION code_compiler
		SECTION code_clib
		SECTION code_crt0_sccz80
		SECTION code_l_sdcc
		SECTION code_compress_zx7
		SECTION code_fp
		SECTION code_math
		SECTION code_error
		SECTION code_user
		SECTION rodata_fp
		SECTION rodata_compiler
		SECTION rodata_clib
		SECTION rodata_user
		SECTION smc_clib
		SECTION ROMABLE_END
IF !__crt_model
                SECTION DATA
		SECTION data_crt
		SECTION data_compiler
		SECTION data_user
		SECTION DATA_END
ENDIF

		SECTION BSS
IF CRT_ORG_BSS
		org	CRT_ORG_BSS
ENDIF
		SECTION bss_crt
IF !DEFINED_nostreams
		PUBLIC	__sgoioblk
__sgoioblk:      defs    40      ;stdio control block
ENDIF
		PUBLIC	base_graphics
		PUBLIC	exitsp
		PUBLIC	exitcount
IF !DEFINED_basegraphics
base_graphics:   defw    0       ;Address of graphics map
ENDIF
exitsp:          defw    0       ;atexit() stack
exitcount:       defb    0       ;Number of atexit() routines
IF DEFINED_USING_amalloc
		PUBLIC _heap
; The heap pointer will be wiped at startup,
; but first its value (based on ASMTAIL)
; will be kept for sbrk() to setup the malloc area
_heap:
                defw 0          ; Initialised by code_crt_init - location of the last program byte
                defw 0
ENDIF
		SECTION bss_fp
		SECTION bss_error
		SECTION bss_fardata
IF bss_fardata_start
		org	bss_fardata_start
ENDIF
		SECTION bss_compiler
IF bss_compiler_start
		org	bss_compiler_start
ENDIF
		SECTION bss_clib
		SECTION bss_user
IF __crt_model > 0
                SECTION DATA
		org	-1
		defb	0		; we want this written out
                SECTION data_crt
                SECTION data_compiler
                SECTION data_user
                SECTION DATA_END
ENDIF
		SECTION BSS_END
