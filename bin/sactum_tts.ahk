
^l:: 
	stop:=0
	num := 0
	Loop, parse, %clipboard%, `n, `r
	{
		if(stop = 1){
			return
		}
		Line := StrSplit(A_LoopField, "|", "`r")
		Send t
		Sleep, 2
		Send %Line[1]%{Enter}
		Sleep, %Line[2]%
	}
return

^k::
stop:=1
return
