vapidir = $(datarootdir)/vala/vapi
vapi_DATA = lunar-calendar-2.0.vapi 

dist_vapi_DATA = lunar-calendar-2.0.deps

BUILT_SOURCES = $(vapi_DATA)

pkgconfigdir = ${libdir}/pkgconfig

lunar-calendar-2.0.vapi: $(top_builddir)/lunar-calendar/LunarCalendar-2.0.gir
	$(VAPIGEN_V)$(VAPIGEN) \
		--library lunar-calendar-2.0 \
		--pkg gtk+-2.0 \
		$< \
		$(NULL)
	@mv $@ lunar-calendar-2.0.vapi.tmp1
	$(AM_V_GEN)sed -e \
                 's#LunarCalendar-2.0.h#lunar-calendar/lunar-calendar.h#' \
                 < lunar-calendar-2.0.vapi.tmp1 > $@
	@rm lunar-calendar-2.0.vapi.tmp*

CLEANFILES = \
        $(BUILT_SOURCES) \
        $(NULL)


# set up the verbosity rules to avoid some build noise
VAPIGEN_V = $(VAPIGEN_V_$(V))
VAPIGEN_V_ = $(VAPIGEN_V_$(AM_DEFAULT_VERBOSITY))
VAPIGEN_V_0 = @echo "  VAPIG " $^;
