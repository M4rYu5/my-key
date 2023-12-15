ShowToolTip(message, duration){
    ToolTip %message% ; displays message around mouse pointer
    SetTimer RemoveToolTip, %duration%
    return

    RemoveToolTip:
    SetTimer RemoveToolTip, Off
    ToolTip
    return
}
