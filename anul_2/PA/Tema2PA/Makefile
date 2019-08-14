.PHONY: build clean run
build:
	mkdir classes;
	javac -cp classes src/tema2PA/*.java -d classes;
clean:
	rm -r classes
run-p1:
	java -Xmx512m -cp classes tema2PA.P1
run-p2:
	java -Xmx512m -Xss128m -cp classes tema2PA.P2
