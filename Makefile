PLATFORM=Linux

lib:
	$(MAKE) -fplatform/$(PLATFORM) -j$(shell nproc)

game: lib
	$(MAKE) -fplatform/$(PLATFORM) game -j$(shell nproc)

cubes: lib
	$(MAKE) -fplatform/$(PLATFORM) cubes -j$(shell nproc)

debug: lib
	$(MAKE) -fplatform/$(PLATFORM) debug -j$(shell nproc)

memory: lib
	$(MAKE) -fplatform/$(PLATFORM) memory -j$(shell nproc)

clean:
	rm -rf obj