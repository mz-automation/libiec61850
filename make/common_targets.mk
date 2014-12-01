$(LIB_NAME):
		cd $(LIBIEC_HOME); $(MAKE) -f Makefile

lib:	$(LIB_NAME)
	
libclean:	clean
	cd $(LIBIEC_HOME); $(MAKE) -f Makefile clean
