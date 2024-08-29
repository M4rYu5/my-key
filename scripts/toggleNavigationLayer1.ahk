; -----------------------------------------------------------------
; -------------------------    LAYER 1    -------------------------

; TOGGLE COMMAND
>^\::
    ToggleLayer(1)
    return


; LAYER CONFIG
#IF layer == 1

    ; It's nice for some games, and software,
    ; where shift has a different function.
    a::Left
    d::Right
    s::Down
    w::Up

#IF