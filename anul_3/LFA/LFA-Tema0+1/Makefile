.PHONY: build clean run
build:
	mkdir classes;
	javac -cp classes src/lfatema1/*.java -d classes;
clean:
	rm -r classes
run:
	java -Xmx512m -cp classes lfatema1.Main ${arg}
