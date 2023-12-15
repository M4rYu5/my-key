; for 60% keyboard


; NICE TO BE PRESSED LEFT HAND ONLY
; Delete / Shift + Delete
CapsLock & g::Delete

; stop build (VS) Ctrl + Break
CapsLock & q::^CtrlBreak


; most programs use F2 for renaming
CapsLock & 2::F2
; most programs use F3 to step over find results
CapsLock & 3::F3
; F7 and shift F7 is used to switch 
; between front-end and back-end files
CapsLock & 7::F7
; Alt + F4 => by pressing CapsLock
CapsLock & 4::F4

; debugging F5, F9, F10, F11, F12
CapsLock & 5::F5
CapsLock & w::F9
CapsLock & e::F10
CapsLock & r::F11
CapsLock & t::F12

; no reason for not having F1 and F6-F12 on CapsLock
CapsLock & 1::F1
CapsLock & 6::F6
CapsLock & 8::F8
CapsLock & 9::F9
CapsLock & 0::F10
CapsLock & -::F11
CapsLock & =::F12


; arrows for right hand, using right shift + pl;'
>+p::Send {Up down}
>+p Up::Send {Up up}
>+l::Send {Left down}
>+l Up::Send {Left up}
>+`;::Send {Down down}
>+`; Up::Send {Down up}
>+'::Send {Right down}
>+' Up::Send {Right up}


; arrows for left hand, using a + esdf
; a isn't working anymore
; also blender uses shift + s shortcut 
; so I can't use left shift + wasd 
; a & e::Send {Up down}
; a & e Up::Send {Up up}
; a & s::Send {Left down}
; a & s Up::Send {Left up}
; a & d::Send {Down down}
; a & d Up::Send {Down up}
; a & f::Send {Right down}
; a & f Up::Send {Right up}


; sound control, using right shift + ,./
>+,::Send {Volume_Down down}
>+, Up::Send {Volume_Down up}
>+.::Send {Volume_Up down}
>+. Up::Send {Volume_Up up}
>+/::Send {Volume_Mute down}
>+/ Up::Send {Volume_Mute up}



; I just have to learn to press Fn + Esc, which shouldn't be that hard
; Esc::`
; `::Esc
; CapsLock & BackSpace::Esc





; -----------------------------------------------------------------------
; -----------------------------------------------------------------------
; ----------------      NAVIGATION LAYER     ----------------------------


#Include showToolTip.ahk


; the default layer is 0
global layer := 0


>^\::
    ToggleLayer(1)
    return


ToggleLayer(layerNumber){
    global layer
    If (layer == layerNumber)
        ActivateLayer(0)
    Else
        ActivateLayer(layerNumber)
}

ActivateLayer(layerNumber){
    global layer
    layer := layerNumber
    txt := "Layer " . layer
    ShowToolTip(txt, 500)
}

#IF layer == 1

    ; It's nice for some games, and software,
    ; where shift has a different function.
    a::Left
    d::Right
    s::Down
    w::Up

#IF
