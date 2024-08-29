CAPSLOCK & s::LShift
CAPSLOCK & d::LControl
CAPSLOCK & f::LAlt

CAPSLOCK & j::Left
CAPSLOCK & k::Down
CAPSLOCK & l::Right
CAPSLOCK & i::Up
CAPSLOCK & `;::End
CAPSLOCK & h::Home

CAPSLOCK & o::Delete

; often I press CapsLock and Space, after End hotkey (caps + ;), 
; which will keep the CapsLock ON after relese, 
; this will prevent this behaviour
CAPSLOCK & SPACE::SPACE 



; Up and Down doesn't work in OneNote
#IF WinActive("ahk_exe" "ONENOTE.EXE")
    CAPSLOCK & i::ControlSend, OneNote::DocumentCanvas1, {Up}, ahk_exe ONENOTE.EXE
    CAPSLOCK & k::ControlSend, OneNote::DocumentCanvas1, {Down}, ahk_exe ONENOTE.EXE
#IF