.PHONY: build clean

build: src src/input.sh src/run.c src/mytime.h src/daemon.h src/config.h
	@mkdir bin
	@gcc -std=gnu90 -o bin/crun src/run.c
	@cp /src/input.sh /bin/run
	@chmod +x /bin/run
	@echo "Build completed"
clean: bin
	@rm -rf bin
	@rm /tmp/daemon_pid.txt
	@rm /tmp/inforun.txt
	@rm /tmp/run2daemon.txt
	@echo "Clean completed"


