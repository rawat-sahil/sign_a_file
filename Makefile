flags=-g -lcrypto
dependecy= helper.h
helperfunction=helper.o
all:fput fget fput_encrypt fget_decrypt fsign fverify

%.o:%.cpp $(depends)
	g++ $^ $(flags) -c -o $@

fput:fput.o $(helperfunction)
	g++ $^ $(flags) -o $@
	sudo chmod u+s $@

fget:$(helperfunction) fget.o
	g++ $^ $(flags) -o $@
	sudo chmod u+s $@


fput_encrypt:$(helperfunction) fput_encrypt.o
	g++ $^ $(flags) -o $@
	sudo chmod u+s $@

fget_decrypt:$(helperfunction) fget_decrypt.o
	g++ $^ $(flags) -o $@
	sudo chmod u+s $@

fsign:$(helperfunction) fsign.o
	g++ $^ $(flags) -o $@
	sudo chmod u+s $@

fverify:$(helperfunction) fverify.o
	g++ $^ $(flags) -o $@
	sudo chmod u+s $@

clean:
	-rm *o fput fget fput_encrypt fget_decrypt fsign fverify testfile something something.sign peda*
