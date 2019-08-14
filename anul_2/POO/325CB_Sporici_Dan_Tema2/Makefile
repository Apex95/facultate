.PHONY: build clean run
build:
	mkdir classes;
	
	javac -cp classes src/Main/*.java -d classes;
clean:
	rm -r classes
run:
	java -Xmx512m -cp classes Main.Main $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
