flags=-g
dependecy= helper.h
helperfunction=helper.o
all:fput fget fput_encrypt fget_decrypt fsign fverify
	touch testfile

%.o:%.cpp $(depends)
	g++ $(flags) -c -o $@ $<

fput:fput.o $(helperfunction)
	g++ $(flags) -o $@ $^
	sudo chmod u+s $@

fget:fget.o $(helperfunction)
	g++ $(flags) -o $@ $^
	sudo chmod u+s $@


fput_encrypt:fput_encrypt.o $(helperfunction)
	g++ $(flags) -o $@ $^
	sudo chmod u+s $@

fget_decrypt:fget_decrypt.o $(helperfunction)
	g++ $(flags) -o $@ $^
	sudo chmod u+s $@

fsign:fsign.o $(helperfunction)
	g++ $(flags) -o $@ $^
	sudo chmod u+s $@

fverify:fverify.o $(helperfunction)
	g++ $(flags) -o $@ $^
	sudo chmod u+s $@

clean:
	-rm *o fput fget fput_encrypt fget_decrypt fsign fverify testfile
