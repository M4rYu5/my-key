


autoClick := true



RAlt & z::Click100()
RAlt & x::Click1000()
RAlt & v::AutoClickStop()
RAlt & c::AutoClickStart()



Click100(){
	Loop, 100
	{
		Click
	}
}

Click1000(){
	Loop, 1000
	{
		Click
	}
}

AutoClickStop(){
	global autoClick := false
}

AutoClickStart(){
	global autoClick := true
	Loop
	{
		if(autoClick){
			Loop, 10{
				Click
			}
		}
		else{
			break
		}
	}

}