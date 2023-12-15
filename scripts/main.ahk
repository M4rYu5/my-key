; NOTE:
;
; All the ahk included scripts are just pasted into this script
; and the auto-executed section will end at the first Return, Exit or hotkey/hotstring label
; 
; in my particular case the auto-executed sectioin will end before ending the navigationLayer
; and all global variables declared in scripts included after this one will not be initialized
;
; Possible solutions:
; #1 like C++ split scripts in headers and bodyes
; #2 set global variables if not initialized when using them, 
;   maybe making a function to get/set global variables:
;   GetGlobal(globalVar, defaultVal){
;       global globalVar
;       if(defaultLayer == ""){
;           Return defaultVal
;       }
;       Return globalVar
;   }
; #3 make distinct init functions (in each script) to init global variable
;
; however at this time I will declare the navigationLayer.ahk before muteMike.ahk

; 2023-12-14: More than a year ago the navigationLayer.ahk was disabled in favor of smallRemap (it was too slow to switch layers while typing).


; #Include navigationLayer.ahk
#Include smallRemap.ahk
#Include 60pKeyRemap.ahk
#Include stringReplace.ahk

#Include muteMike.ahk