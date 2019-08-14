.PHONY: build clean run
build:
	mkdir classes;
	javac -cp classes src/temapa/*.java -d classes;
clean:
	rm -r classes
run-p1:
	java -Xmx512m -cp classes temapa.Main p1
