#Include showToolTip.ahk

; Mute mike when home button pressed
Browser_Home::MuteMike()

MuteMike(){ 
    ; change your mike volume to an unusual level (ex 77)
    ; then open "Find mike.ahk" script to get the mike id
    mikeId := 9
    
    SoundSet, +1, MASTER, mute, mikeId 
    SoundGet, master_mute, , mute, mikeId
    message = Mute %master_mute%
    ShowToolTip(message, 1000)
}