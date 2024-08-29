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

    ; ARROWS
    i:: Send {U+2191} ; ↑
    j:: Send {U+2190} ; ←
    k:: Send {U+2193} ; ↓
    l:: Send {U+2192} ; →
    u:: Send {U+2196} ; ↖
    o:: Send {U+2197} ; ↗
    ,:: Send {U+2198} ; ↘
    m:: Send {U+2199} ; ↙
    h:: Send {U+2194} ; ↔
    `;:: Send {U+2195} ; ↕

#IF