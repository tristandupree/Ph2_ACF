all: Interface src

Interface::
	$(MAKE) -C $@
src::
	$(MAKE) -C $@


clean:
	(cd Interface; make clean)
	(rm -f lib/* bin/*)
