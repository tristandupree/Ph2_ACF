all: HWDescription HWInterface src

HWDescription::
	$(MAKE) -C $@
HWInterface::
	$(MAKE) -C $@
src::
	$(MAKE) -C $@


clean:
	(cd HWInterface; make clean)
	(cd HWDescription; make clean)
	(rm -f lib/* bin/*)

