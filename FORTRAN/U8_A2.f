	PROGRAM U8_A2
	REAL ZAHL, ZAHL2
	ZAHL2 = 2.4
	ZAHL = 0.75E-3
	WRITE(*,*) "Hallo Welt!" !a)
	WRITE(6,'(11A)') "Hallo Welt!"!b)
	PRINT *, 'Gut gemacht' !c)
	WRITE(*,1) 'Hallo Welt!', ZAHL  !e)
	WRITE(*,2) ZAHL2
	STOP
1	FORMAT (A,E10.2E2)
2	FORMAT (F3.1)
	END PROGRAM U8_A2