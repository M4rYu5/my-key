#Include showToolTip.ahk


; layer code

defaultLayer := 0
; layer 0: no behaviour changes 
; layer 1: will be a navigation layer
layer := defaultLayer

; the time between same consecutive keys that will trigger a layer switch
deltaBehaviorChanger := 200     ; in ms



; --------------------------------------------------------------------------------------
; --------------------------------------------------------------------------------------
; --------------------------------------------------------------------------------------
; Layer 1; triggered by Caps Lock

capsLayerNumber := 1
layer1KeyPressTime := 0
CapsLock::CapsLayoutSwitch()

CapsLayoutSwitch(){
    global layer1KeyPressTime
    global deltaBehaviorChanger
    currentTime := A_TickCount

    ToggleCapsLayer()
    if (layer1KeyPressTime + deltaBehaviorChanger > currentTime){
        SetCapsLockState, % GetKeyState("CapsLock","T") ? "Off" : "On"
        return
    }
    layer1KeyPressTime := currentTime
}

ToggleCapsLayer(){
    global capsLayerNumber
    global defaultLayer
    global layer
    If (layer == capsLayerNumber)
        ActivateLayer(defaultLayer)
    Else
        ActivateLayer(capsLayerNumber)
}





; --------------------------------------------------------------------------------------
; --------------------------------------------------------------------------------------
; --------------------------------------------------------------------------------------



#IF layer == 1

    a::Tab
    s::LShift
    d::LControl
    f::LAlt

    j::Left
    l::Right
    k::Down
    i::Up
    `;::End
    h::Home

    o::Delete

#IF



; --------------------------------------------------------------------------------------
; --------------------------------------------------------------------------------------
; --------------------------------------------------------------------------------------


ActivateLayer(layerNumber){
    global layer
    layer := layerNumber
    ShowToolTip(layerNumber, 500)
}