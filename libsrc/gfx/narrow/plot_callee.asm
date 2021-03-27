;
;     Z88 Graphics Functions - Small C+ stubs
;
;     Written around the Interlogic Standard Library
;
;     Stubs Written by D Morris - 30/9/98
;
;
;    $Id: plot_callee.asm $
;


IF !__CPU_INTEL__ & !__CPU_GBZ80__
; CALLER LINKAGE FOR FUNCTION POINTERS
; ----- void  plot(int x, int y)


    SECTION code_graphics

    PUBLIC    plot_callee
    PUBLIC    _plot_callee
    PUBLIC    ASMDISP_PLOT_CALLEE

    EXTERN    swapgfxbk
    EXTERN    __graphics_end

    EXTERN    plotpixel
    INCLUDE "graphics/grafix.inc"

.plot_callee
._plot_callee
    pop    af    ; ret addr
    pop hl    ; y
    pop de    ; x
    ld    h,e
    push    af    ; ret addr

.asmentry
    push    ix
IF NEED_swapgfxbk = 1
    call    swapgfxbk
ENDIF
    call    plotpixel
IF NEED_swapgfxbk
    jp      __graphics_end
ELSE
  IF !__CPU_INTEL__ & !__CPU_GBZ80__
    pop     ix
  ENDIF
    ret
ENDIF
DEFC ASMDISP_PLOT_CALLEE = asmentry - plot_callee
ENDIF
