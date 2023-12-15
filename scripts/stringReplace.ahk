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