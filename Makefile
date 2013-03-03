TARGET=build/Release/leap.node

all: $(TARGET) bundle.js

$(TARGET): leap.cc
	CXXFLAGS=-std=c++0x node-gyp -v configure build

bundle.js: client.js
	browserify client.js -o bundle.js

clean:
	node-gyp clean

run: $(TARGET)
	DYLD_LIBRARY_PATH=sdk/lib nodemon -w server.js -w build/ server.js

.PHONY: clean run
