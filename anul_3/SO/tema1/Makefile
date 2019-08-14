build: main.obj
    lib /def:hash.def /out:hash.lib
	link main.obj hash.lib /out:tema1.exe

main.obj: main.c
	cl /out:main.obj /c main.c
    
    
clean:
    del tema1.exe *.obj *.lib
