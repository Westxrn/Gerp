CXX = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow -O2
LDFLAGS = -g3

gerp: main.o Gerp.o Index.o DirNode.o FSTree.o Hashmap.o
	${CXX} $(LDFLAGS) -O2 -o $@ $^

main.o: main.cpp Gerp.h
	$(CXX) $(CXXFLAGS) -c $<

Gerp.o: Gerp.cpp Gerp.h DirNode.h FSTree.h Index.h Hashmap.h
	$(CXX) $(CXXFLAGS) -c $<

Index.o: Index.cpp Index.h Hashmap.h 
	$(CXX) $(CXXFLAGS) -c $<

Hashmap.o: Hashmap.cpp Hashmap.h Index.h wordStruct.h
	$(CXX) $(CXXFLAGS) -c $<

unit_test: unit_test_driver.o processing.o FSTree.o DirNode.o Hashmap.h
	${CXX} $(LDFLAGS) $^

clean:
	@find . -type f \( \
		-name '*.o' ! -name 'FSTree.o' ! -name 'DirNode.o' \
		\) -exec rm -f {} \;
	@rm -f *~ a.out

