; NOTE:
;
; All the ahk included scripts are just 'pasted' into this script
; and the auto-executed section will end at the first Return, Exit or hotkey/hotstring label,
; and this could leave global variables uninitialized.
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


#Include toggleNavigationBase.ahk

#Include smallRemap.ahk
#Include 60pKeyRemap.ahk
#Include stringReplace.ahk

#Include muteMike.ahk