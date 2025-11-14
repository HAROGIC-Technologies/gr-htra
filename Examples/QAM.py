#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: QAM Demod
# GNU Radio version: 3.10.7.0

from packaging.version import Version as StrictVersion
from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio import analog
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
import QAM_epy_block_0 as epy_block_0  # embedded python block
import htra_source
import sip



class QAM(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "QAM Demod", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("QAM Demod")
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

        self.settings = Qt.QSettings("GNU Radio", "QAM")

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
        self.variable_constellation_64qam = variable_constellation_64qam = digital.constellation_calcdist([complex(i, q) for q in (-7,-5,-3,-1,1,3,5,7) for i in (-7,-5,-3,-1,1,3,5,7)], [(((s & 7) ^ ((s & 7)>>1) ^ ((s & 7)>>2)) * 8) |(((s>>3) & 7) ^ (((s>>3) & 7)>>1) ^ (((s>>3) & 7)>>2))for s in range(64)],
        4, 1, digital.constellation.POWER_NORMALIZATION).base()
        self.variable_constellation_16qam = variable_constellation_16qam = digital.constellation_16qam().base()
        self.symbol_rate = symbol_rate = 250000
        self.sps = sps = 4
        self.samp_rate = samp_rate = 125000000
        self.reference_level = reference_level = 0
        self.excess_bw = excess_bw = 0.35
        self.decimate_factor = decimate_factor = 64
        self.center_freq = center_freq = 1000000000

        ##################################################
        # Blocks
        ##################################################

        self._samp_rate_range = Range(110000000, 130000000, 1, 125000000, 200)
        self._samp_rate_win = RangeWidget(self._samp_rate_range, self.set_samp_rate, "Samp_rate", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._samp_rate_win)
        self._reference_level_range = Range(-50, 23, 1, 0, 200)
        self._reference_level_win = RangeWidget(self._reference_level_range, self.set_reference_level, "Reference_Level", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._reference_level_win)
        self._center_freq_range = Range(9000, 6300000000, 100, 1000000000, 200)
        self._center_freq_win = RangeWidget(self._center_freq_range, self.set_center_freq, "Center_Frequency", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._center_freq_win)
        self.root_raised_cosine_filter_0_0_1 = filter.fir_filter_ccf(
            1,
            firdes.root_raised_cosine(
                1,
                (symbol_rate*4),
                symbol_rate,
                excess_bw,
                (11*sps*3)))
        self.rational_resampler_xxx_0_0_1 = filter.rational_resampler_ccc(
                interpolation=(4*symbol_rate*decimate_factor),
                decimation=samp_rate,
                taps=[],
                fractional_bw=0.4)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
            1024, #size
            samp_rate, #samp_rate
            'IQvT', #name
            2, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude(V)', "")

        self.qtgui_time_sink_x_0.enable_tags(True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)


        labels = ['Ch-I', 'Ch-Q', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_win, 0, 0, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
            512, #size
            window.WIN_BLACKMAN_hARRIS, #wintype
            center_freq, #fc
            (samp_rate/decimate_factor), #bw
            'Spectrum', #name
            1,
            None # parent
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis((-140), 10)
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
        self.top_grid_layout.addWidget(self._qtgui_freq_sink_x_0_win, 1, 0, 1, 2)
        for r in range(1, 2):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.qtgui_const_sink_x_0_1_0_0_0 = qtgui.const_sink_c(
            1024, #size
            'Constellation', #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_const_sink_x_0_1_0_0_0.set_update_time(0.10)
        self.qtgui_const_sink_x_0_1_0_0_0.set_y_axis((-2), 2)
        self.qtgui_const_sink_x_0_1_0_0_0.set_x_axis((-2), 2)
        self.qtgui_const_sink_x_0_1_0_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0_1_0_0_0.enable_autoscale(False)
        self.qtgui_const_sink_x_0_1_0_0_0.enable_grid(True)
        self.qtgui_const_sink_x_0_1_0_0_0.enable_axis_labels(True)

        self.qtgui_const_sink_x_0_1_0_0_0.disable_legend()

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
                self.qtgui_const_sink_x_0_1_0_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0_1_0_0_0.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0_1_0_0_0.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0_1_0_0_0.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0_1_0_0_0.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0_1_0_0_0.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0_1_0_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_const_sink_x_0_1_0_0_0_win = sip.wrapinstance(self.qtgui_const_sink_x_0_1_0_0_0.qwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_const_sink_x_0_1_0_0_0_win, 0, 1, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(1, 2):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.htra_source_0_0_1 = htra_source.htra_source("USB",0,"'192.168.1.100'",center_freq, samp_rate, htra_source.DecimationFactor.DECIM_64, reference_level)
        self.epy_block_0 = epy_block_0.OptimizedDDPLL(Loop_BandWidth=0.0628*0.5, Damping_Factor=1, Constellation_Object=variable_constellation_16qam)
        self.digital_symbol_sync_xx_0_1_0 = digital.symbol_sync_cc(
            digital.TED_GARDNER,
            sps,
            (0.0628*0.1),
            1,
            1.0,
            1.5,
            1,
            variable_constellation_16qam,
            digital.IR_PFB_NO_MF,
            128,
            [])
        self.blocks_skiphead_0 = blocks.skiphead(gr.sizeof_gr_complex*1, 100000)
        self.blocks_complex_to_float_0 = blocks.complex_to_float(1)
        self.analog_agc2_xx_0_0_1 = analog.agc2_cc((1e-2), (1e-3), 1.0, 1.0, 65536)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_agc2_xx_0_0_1, 0), (self.digital_symbol_sync_xx_0_1_0, 0))
        self.connect((self.blocks_complex_to_float_0, 1), (self.qtgui_time_sink_x_0, 1))
        self.connect((self.blocks_complex_to_float_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.blocks_skiphead_0, 0), (self.epy_block_0, 0))
        self.connect((self.digital_symbol_sync_xx_0_1_0, 0), (self.blocks_skiphead_0, 0))
        self.connect((self.epy_block_0, 0), (self.qtgui_const_sink_x_0_1_0_0_0, 0))
        self.connect((self.htra_source_0_0_1, 0), (self.blocks_complex_to_float_0, 0))
        self.connect((self.htra_source_0_0_1, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.htra_source_0_0_1, 0), (self.rational_resampler_xxx_0_0_1, 0))
        self.connect((self.rational_resampler_xxx_0_0_1, 0), (self.root_raised_cosine_filter_0_0_1, 0))
        self.connect((self.root_raised_cosine_filter_0_0_1, 0), (self.analog_agc2_xx_0_0_1, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "QAM")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_variable_constellation_64qam(self):
        return self.variable_constellation_64qam

    def set_variable_constellation_64qam(self, variable_constellation_64qam):
        self.variable_constellation_64qam = variable_constellation_64qam

    def get_variable_constellation_16qam(self):
        return self.variable_constellation_16qam

    def set_variable_constellation_16qam(self, variable_constellation_16qam):
        self.variable_constellation_16qam = variable_constellation_16qam
        self.epy_block_0.Constellation_Object = self.variable_constellation_16qam

    def get_symbol_rate(self):
        return self.symbol_rate

    def set_symbol_rate(self, symbol_rate):
        self.symbol_rate = symbol_rate
        self.root_raised_cosine_filter_0_0_1.set_taps(firdes.root_raised_cosine(1, (self.symbol_rate*4), self.symbol_rate, self.excess_bw, (11*self.sps*3)))

    def get_sps(self):
        return self.sps

    def set_sps(self, sps):
        self.sps = sps
        self.root_raised_cosine_filter_0_0_1.set_taps(firdes.root_raised_cosine(1, (self.symbol_rate*4), self.symbol_rate, self.excess_bw, (11*self.sps*3)))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.htra_source_0_0_1.set_sample_rate(self.samp_rate)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.center_freq, (self.samp_rate/self.decimate_factor))
        self.qtgui_time_sink_x_0.set_samp_rate(self.samp_rate)

    def get_reference_level(self):
        return self.reference_level

    def set_reference_level(self, reference_level):
        self.reference_level = reference_level
        self.htra_source_0_0_1.set_ref_level(self.reference_level)

    def get_excess_bw(self):
        return self.excess_bw

    def set_excess_bw(self, excess_bw):
        self.excess_bw = excess_bw
        self.root_raised_cosine_filter_0_0_1.set_taps(firdes.root_raised_cosine(1, (self.symbol_rate*4), self.symbol_rate, self.excess_bw, (11*self.sps*3)))

    def get_decimate_factor(self):
        return self.decimate_factor

    def set_decimate_factor(self, decimate_factor):
        self.decimate_factor = decimate_factor
        self.qtgui_freq_sink_x_0.set_frequency_range(self.center_freq, (self.samp_rate/self.decimate_factor))

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.htra_source_0_0_1.set_center_freq(self.center_freq)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.center_freq, (self.samp_rate/self.decimate_factor))




def main(top_block_cls=QAM, options=None):

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
