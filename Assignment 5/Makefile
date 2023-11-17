all: concurrent serial

concurrent:
	@rm -f concurrent.out
	@gcc concurrent.c -lpthread -o concurrent.out

serial:
	@rm -f serial.out
	@gcc serial.c -lpthread -o serial.out

clean:
	@rm *.out