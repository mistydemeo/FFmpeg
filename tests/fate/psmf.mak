#
# Test detecting ATRAC-3 audio in Sony MPEG files
#

PSMF_PROBE_COMMAND = \
	ffprobe$(PROGSSUF)$(EXESUF) -show_entries stream=codec_name \
	-select_streams a -print_format default=noprint_wrappers=1 -bitexact -v 0

FATE_PSMF_FFPROBE = fate-psmf-probe-00006 \
					fate-psmf-probe-EV01_01_0500D

fate-psmf-probe-00006: SRC = $(TARGET_SAMPLES)/psmf/00006.MPS
fate-psmf-probe-00006: CMD = run $(PSMF_PROBE_COMMAND) -i "$(SRC)"
fate-psmf-probe-00006: CMP = oneline
fate-psmf-probe-00006: REF = codec_name=atrac3p
fate-psmf-probe-EV01_01_0500D: SRC = $(TARGET_SAMPLES)/psmf/EV01_01_0500D.PMF
fate-psmf-probe-EV01_01_0500D: CMD = run $(PSMF_PROBE_COMMAND) -i "$(SRC)"
fate-psmf-probe-EV01_01_0500D: CMP = oneline
fate-psmf-probe-EV01_01_0500D: REF = codec_name=atrac3p

FATE_SAMPLES_FFPROBE += $(FATE_PSMF_FFPROBE)

fate-psmf: $(FATE_PSMF_FFPROBE)
