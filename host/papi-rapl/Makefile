
#CC = /opt/intel/bin/icc

libraplpower.a: rapl_power.o
	mkdir tmp; \
	cp libpapi.a tmp; \
	cp libtestlib.a tmp; \
	cp rapl_power.o tmp; \
	cd tmp; \
	ar x libpapi.a; \
	ar x libtestlib.a; \
	ar rcs $@ *.o; \
	mv $@ ../; \
	cd ../; \
	rm -r tmp;

rapl_power.o: rapl_power.c
	$(CC) -c $< -o $@

clean:
	rm -rf *.o libraplpower.a
