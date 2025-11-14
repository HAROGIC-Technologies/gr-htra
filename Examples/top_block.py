#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: DAB Receiver
# GNU Radio version: 3.10.7.0

from packaging.version import Version as StrictVersion
from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio import audio
from gnuradio import blocks
from gnuradio import digital
from gnuradio import filter
from gnuradio.filter import firdes
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from PyQt5 import Qt
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio.qtgui import Range, RangeWidget
from PyQt5 import QtCore
import gnuradio.dab as dab
import htra_source
import sip



class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "DAB Receiver", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("DAB Receiver")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except BaseException as exc:
            print(f"Qt GUI: Could not set Icon: {str(exc)}", file=sys.stderr)
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except BaseException as exc:
            print(f"Qt GUI: Could not restore geometry: {str(exc)}", file=sys.stderr)

        ##################################################
        # Variables
        ##################################################
        self.sample_rate = sample_rate = 125e6
        self.reference_level = reference_level = -30
        self.decimate_factor = decimate_factor = 32
        self.center_freq = center_freq = 202.928e6

        ##################################################
        # Blocks
        ##################################################

        self._sample_rate_range = Range(110000000, 130000000, 1, 125e6, 200)
        self._sample_rate_win = RangeWidget(self._sample_rate_range, self.set_sample_rate, "Sample_rate", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._sample_rate_win)
        self._reference_level_range = Range(-50, 23, 1, -30, 200)
        self._reference_level_win = RangeWidget(self._reference_level_range, self.set_reference_level, "Reference_Level", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._reference_level_win)
        self._center_freq_range = Range(9000, 6300000000, 100, 202.928e6, 200)
        self._center_freq_win = RangeWidget(self._center_freq_range, self.set_center_freq, "Center_Frequency", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._center_freq_win)
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=8192,
                decimation=15625,
                taps=[],
                fractional_bw=0)
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
            1024, #size
            window.WIN_BLACKMAN_hARRIS, #wintype
            0, #fc
            2048000, #bw
            'Spectrum', #name
            1,
            None # parent
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis((-160), 10)
        self.qtgui_freq_sink_x_0.set_y_label('Power', 'dBm')
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.enable_grid(False)
        self.qtgui_freq_sink_x_0.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0.enable_axis_labels(True)
        self.qtgui_freq_sink_x_0.enable_control_panel(False)
        self.qtgui_freq_sink_x_0.set_fft_window_normalized(False)

        self.qtgui_freq_sink_x_0.disable_legend()


        labels = ['', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
            "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_freq_sink_x_0_win, 0, 0, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.qtgui_const_sink_x_1 = qtgui.const_sink_c(
            4096, #size
            'Constellation', #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_const_sink_x_1.set_update_time(0.10)
        self.qtgui_const_sink_x_1.set_y_axis((-2), 2)
        self.qtgui_const_sink_x_1.set_x_axis((-2), 2)
        self.qtgui_const_sink_x_1.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_1.enable_autoscale(False)
        self.qtgui_const_sink_x_1.enable_grid(False)
        self.qtgui_const_sink_x_1.enable_axis_labels(True)

        self.qtgui_const_sink_x_1.disable_legend()

        labels = ['', '', '', '', '',
            '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ["blue", "red", "red", "red", "red",
            "red", "red", "red", "red", "red"]
        styles = [0, 0, 0, 0, 0,
            0, 0, 0, 0, 0]
        markers = [0, 0, 0, 0, 0,
            0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]

        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_1.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_1.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_1.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_1.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_1.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_1.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_1.set_line_alpha(i, alphas[i])

        self._qtgui_const_sink_x_1_win = sip.wrapinstance(self.qtgui_const_sink_x_1.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_const_sink_x_1_win, 0, 1, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(1, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.htra_source_0 = htra_source.htra_source("USB",0,"'192.168.1.100'",center_freq, sample_rate, htra_source.DecimationFactor.DECIM_32, reference_level)
        self.digital_mpsk_snr_est_cc_0 = digital.mpsk_snr_est_cc(2, 10000, 0.001)
        self.dab_ofdm_demod_0 = dab.ofdm_demod(
                  dab.parameters.dab_parameters(
                    mode=1,
                    sample_rate=2048000,
                    verbose=False
                  ),
                  dab.parameters.receiver_parameters(
                    mode=1,
                    softbits=True,
                    input_fft_filter=True,
                    autocorrect_sample_rate=True,
                    sample_rate_correction_factor=1+float(0)*1e-6,
                    always_include_resample=True,
                    verbose=False,
                    correct_ffe=True,
                    equalize_magnitude=True
                  )
                )

        self.dab_fic_decode_0 = dab.fic_decode(
                  dab.parameters.dab_parameters(
                    mode=1,
                    sample_rate=2048000,
                    verbose=False
                  )
                )
        self.dab_fic_decode_0.set_print_channel_info(False)
        self.dab_dabplus_audio_decoder_ff_0 = dab.dabplus_audio_decoder_ff(dab.parameters.dab_parameters(mode=1, sample_rate=2048000, verbose=False), 88, 462, 66, 2, True)
        self.blocks_vector_to_stream_0_1 = blocks.vector_to_stream(gr.sizeof_char*1, 32)
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 1536)
        self.blocks_null_sink_0_1 = blocks.null_sink(gr.sizeof_char*1)
        self.audio_sink_0 = audio.sink(24000, '', True)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_vector_to_stream_0, 0), (self.digital_mpsk_snr_est_cc_0, 0))
        self.connect((self.blocks_vector_to_stream_0_1, 0), (self.blocks_null_sink_0_1, 0))
        self.connect((self.dab_dabplus_audio_decoder_ff_0, 0), (self.audio_sink_0, 0))
        self.connect((self.dab_dabplus_audio_decoder_ff_0, 1), (self.audio_sink_0, 1))
        self.connect((self.dab_fic_decode_0, 0), (self.blocks_vector_to_stream_0_1, 0))
        self.connect((self.dab_ofdm_demod_0, 0), (self.blocks_vector_to_stream_0, 0))
        self.connect((self.dab_ofdm_demod_0, 0), (self.dab_dabplus_audio_decoder_ff_0, 0))
        self.connect((self.dab_ofdm_demod_0, 0), (self.dab_fic_decode_0, 0))
        self.connect((self.digital_mpsk_snr_est_cc_0, 0), (self.qtgui_const_sink_x_1, 0))
        self.connect((self.htra_source_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.dab_ofdm_demod_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.qtgui_freq_sink_x_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_sample_rate(self):
        return self.sample_rate

    def set_sample_rate(self, sample_rate):
        self.sample_rate = sample_rate
        self.htra_source_0.set_sample_rate(self.sample_rate)

    def get_reference_level(self):
        return self.reference_level

    def set_reference_level(self, reference_level):
        self.reference_level = reference_level
        self.htra_source_0.set_ref_level(self.reference_level)

    def get_decimate_factor(self):
        return self.decimate_factor

    def set_decimate_factor(self, decimate_factor):
        self.decimate_factor = decimate_factor

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.htra_source_0.set_center_freq(self.center_freq)




def main(top_block_cls=top_block, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
