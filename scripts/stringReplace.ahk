:*:4date::
FormatTime, CurrentDate,, yyyy-MM-dd
SendInput %CurrentDate% 
return

:*:4time::
FormatTime, CurrentTime,, HH:mm
SendInput %CurrentTime%
return

:*:4dt::
FormatTime, CurrentDateTime,, yyyy-MM-dd HH:mm
SendInput %CurrentDateTime%
return


; ↑ ← ↓ →
:*:;i::{U+2191}
:*:;j::{U+2190}
:*:;k::{U+2193}
:*:;l::{U+2192}