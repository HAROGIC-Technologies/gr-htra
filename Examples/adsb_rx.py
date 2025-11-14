#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: ADSB Demod
# Generated: Fri Nov 14 01:09:28 2025
##################################################

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt5 import Qt
from PyQt5 import Qt, QtCore
from PyQt5.QtCore import QObject, pyqtSlot
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio import zeromq
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import adsb
import htra
import sip
import sys
from gnuradio import qtgui


class adsb_rx(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "ADSB Demod")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("ADSB Demod")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
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

        self.settings = Qt.QSettings("GNU Radio", "adsb_rx")

        if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
            self.restoreGeometry(self.settings.value("geometry").toByteArray())
        else:
            self.restoreGeometry(self.settings.value("geometry", type=QtCore.QByteArray))

        ##################################################
        # Variables
        ##################################################
        self.threshold = threshold = 1e-9
        self.samp_rate = samp_rate = 125000000
        self.reference_level = reference_level = -80
        self.fs = fs = 2e6
        self.decimate_factor = decimate_factor = 32
        self.center_freq = center_freq = 1090000000

        ##################################################
        # Blocks
        ##################################################
        self._threshold_tool_bar = Qt.QToolBar(self)
        self._threshold_tool_bar.addWidget(Qt.QLabel('Detection Threshold'+": "))
        self._threshold_line_edit = Qt.QLineEdit(str(self.threshold))
        self._threshold_tool_bar.addWidget(self._threshold_line_edit)
        self._threshold_line_edit.returnPressed.connect(
        	lambda: self.set_threshold(eng_notation.str_to_num(str(self._threshold_line_edit.text().toAscii()))))
        self.top_grid_layout.addWidget(self._threshold_tool_bar, 0, 1, 1, 1)
        [self.top_grid_layout.setRowStretch(r,1) for r in range(0,1)]
        [self.top_grid_layout.setColumnStretch(c,1) for c in range(1,2)]
        self._samp_rate_range = Range(110000000, 130000000, 1, 125000000, 200)
        self._samp_rate_win = RangeWidget(self._samp_rate_range, self.set_samp_rate, 'Sample_rate', "counter_slider", float)
        self.top_layout.addWidget(self._samp_rate_win)
        self._reference_level_range = Range(-100, 23, 1, -80, 200)
        self._reference_level_win = RangeWidget(self._reference_level_range, self.set_reference_level, 'Reference_Level', "counter_slider", float)
        self.top_layout.addWidget(self._reference_level_win)
        self._center_freq_range = Range(9000, 40000000000, 100, 1090000000, 200)
        self._center_freq_win = RangeWidget(self._center_freq_range, self.set_center_freq, 'Center_Frequency', "counter_slider", float)
        self.top_layout.addWidget(self._center_freq_win)
        self.zeromq_pub_msg_sink_0 = zeromq.pub_msg_sink('tcp://127.0.0.1:5001', 10)
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=64,
                decimation=125,
                taps=None,
                fractional_bw=0.4,
        )
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
        	int(fs*150e-6), #size
        	int(fs), #samp_rate
        	"", #name
        	2 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(1.0/100.0)
        self.qtgui_time_sink_x_0.set_y_axis(0, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_TAG, qtgui.TRIG_SLOPE_POS, 0, 1.25e-6, 0, "burst")
        self.qtgui_time_sink_x_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0.enable_grid(True)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)

        if not False:
          self.qtgui_time_sink_x_0.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [0, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]

        for i in xrange(2):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_time_sink_x_0_win, 1, 0, 1, 2)
        [self.top_grid_layout.setRowStretch(r,1) for r in range(1,2)]
        [self.top_grid_layout.setColumnStretch(c,1) for c in range(0,2)]
        self.htra_htra_source_0 = htra.htra_source("USB",0,"'192.168.1.100'",center_freq, samp_rate, 32, reference_level,"Complex16bit")
        self._decimate_factor_options = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096]
        self._decimate_factor_labels = ["1", "2", "4", "8", "16", "32", "64", "128", "256", "512", "1024", "2048", "4096"]
        self._decimate_factor_tool_bar = Qt.QToolBar(self)
        self._decimate_factor_tool_bar.addWidget(Qt.QLabel('Decimate_Factor'+": "))
        self._decimate_factor_combo_box = Qt.QComboBox()
        self._decimate_factor_tool_bar.addWidget(self._decimate_factor_combo_box)
        for label in self._decimate_factor_labels: self._decimate_factor_combo_box.addItem(label)
        self._decimate_factor_callback = lambda i: Qt.QMetaObject.invokeMethod(self._decimate_factor_combo_box, "setCurrentIndex", Qt.Q_ARG("int", self._decimate_factor_options.index(i)))
        self._decimate_factor_callback(self.decimate_factor)
        self._decimate_factor_combo_box.currentIndexChanged.connect(
        	lambda i: self.set_decimate_factor(self._decimate_factor_options[i]))
        self.top_layout.addWidget(self._decimate_factor_tool_bar)
        self.blocks_complex_to_mag_squared_0 = blocks.complex_to_mag_squared(1)
        self.analog_const_source_x_0 = analog.sig_source_f(0, analog.GR_CONST_WAVE, 0, 0, threshold)
        self.adsb_framer_1 = adsb.framer(fs, threshold)
        self.adsb_demod_0 = adsb.demod(fs)
        self.adsb_decoder_0 = adsb.decoder("Extended Squitter Only", "None", "Brief")

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.adsb_decoder_0, 'decoded'), (self.zeromq_pub_msg_sink_0, 'in'))
        self.msg_connect((self.adsb_demod_0, 'demodulated'), (self.adsb_decoder_0, 'demodulated'))
        self.connect((self.adsb_demod_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.adsb_framer_1, 0), (self.adsb_demod_0, 0))
        self.connect((self.analog_const_source_x_0, 0), (self.qtgui_time_sink_x_0, 1))
        self.connect((self.blocks_complex_to_mag_squared_0, 0), (self.adsb_framer_1, 0))
        self.connect((self.htra_htra_source_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_complex_to_mag_squared_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "adsb_rx")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_threshold(self):
        return self.threshold

    def set_threshold(self, threshold):
        self.threshold = threshold
        Qt.QMetaObject.invokeMethod(self._threshold_line_edit, "setText", Qt.Q_ARG("QString", eng_notation.num_to_str(self.threshold)))
        self.analog_const_source_x_0.set_offset(self.threshold)
        self.adsb_framer_1.set_threshold(self.threshold)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.htra_htra_source_0.set_sample_rate(self.samp_rate)

    def get_reference_level(self):
        return self.reference_level

    def set_reference_level(self, reference_level):
        self.reference_level = reference_level
        self.htra_htra_source_0.set_ref_level(self.reference_level)

    def get_fs(self):
        return self.fs

    def set_fs(self, fs):
        self.fs = fs
        self.qtgui_time_sink_x_0.set_samp_rate(int(self.fs))

    def get_decimate_factor(self):
        return self.decimate_factor

    def set_decimate_factor(self, decimate_factor):
        self.decimate_factor = decimate_factor
        self._decimate_factor_callback(self.decimate_factor)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.htra_htra_source_0.set_center_freq(self.center_freq)


def main(top_block_cls=adsb_rx, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
