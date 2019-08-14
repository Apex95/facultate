serial:
	gcc g_serial.c -o g_serial

parallel:
	gcc g_omp.c -fopenmp -o g_omp


rs:
	time ./g_serial in.txt 100 outs.txt

rp:
	time ./g_omp in.txt 100 outp.txt

clean:
	rm g_serial g_omp
