.PHONY: build clean run
build:
	mkdir classes;
	
	javac -cp classes src/test/*.java -d classes;
	javac -cp classes src/trie/*.java -d classes;
clean:
	rm -r classes
run:
	java -Xmx512m -cp classes trie.Main
