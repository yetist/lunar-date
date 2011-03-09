vapidir = $(datarootdir)/vala/vapi
vapi_DATA = lunar-date-2.0.vapi 

dist_vapi_DATA = lunar-date-2.0.deps

BUILT_SOURCES = $(vapi_DATA)

pkgconfigdir = ${libdir}/pkgconfig

lunar-date-2.0.vapi: $(top_builddir)/lunar-date/LunarDate-2.0.gir
	$(VAPIGEN_V)$(VAPIGEN) \
		--library lunar-date-2.0 \
		--pkg gobject-2.0 \
		--pkg glib-2.0 \
		$< \
		$(NULL)
	@mv $@ lunar-date-2.0.vapi.tmp1
	$(AM_V_GEN)sed -e \
                 's/LunarDate-2.0.h/date.h/' \
                 < lunar-date-2.0.vapi.tmp1 > lunar-date-2.0.vapi.tmp2
	$(AM_V_GEN)sed -e \
		 's/ref string\[\]?/ref unowned string\[\]/' \
                 < lunar-date-2.0.vapi.tmp2 > $@
	@rm lunar-date-2.0.vapi.tmp*

CLEANFILES = \
        $(BUILT_SOURCES) \
        $(NULL)


# set up the verbosity rules to avoid some build noise
VAPIGEN_V = $(VAPIGEN_V_$(V))
VAPIGEN_V_ = $(VAPIGEN_V_$(AM_DEFAULT_VERBOSITY))
VAPIGEN_V_0 = @echo "  VAPIG " $^;
