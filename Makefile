all: HWDescription HWInterface System tools src

HWDescription::
	$(MAKE) -C $@
HWInterface::
	$(MAKE) -C $@
System::
	$(MAKE) -C $@
tools::
	$(MAKE) -C $@
src::
	$(MAKE) -C $@


clean:
	(cd System; make clean)
	(cd HWInterface; make clean)
	(cd HWDescription; make clean)
	(rm -f lib/* bin/*)

