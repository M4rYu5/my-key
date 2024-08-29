#Include showToolTip.ahk


; the default layer is 0
global layer := 0


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


; LAYER DEFINITION
#Include toggleNavigationLayer1.ahk