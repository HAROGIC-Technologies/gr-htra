#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Not titled yet
# GNU Radio version: 3.10.1.1

from packaging.version import Version as StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from PyQt5.QtCore import QObject, pyqtSlot
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import blocks
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio.qtgui import Range, RangeWidget
from PyQt5 import QtCore
import htra_source



from gnuradio import qtgui

class IQ_streaming(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Not titled yet", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Not titled yet")
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

        self.settings = Qt.QSettings("GNU Radio", "IQ_streaming")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.sample_rate = sample_rate = 122880000
        self.reference_level = reference_level = 0
        self.decimate_factor = decimate_factor = 16
        self.center_freq = center_freq = 1000000000

        ##################################################
        # Blocks
        ##################################################
        self._sample_rate_range = Range(110000000, 130000000, 1, 122880000, 200)
        self._sample_rate_win = RangeWidget(self._sample_rate_range, self.set_sample_rate, "Sample_rate", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._sample_rate_win)
        self._reference_level_range = Range(-50, 23, 1, 0, 200)
        self._reference_level_win = RangeWidget(self._reference_level_range, self.set_reference_level, "Reference_Level", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._reference_level_win)
        # Create the options list
        self._decimate_factor_options = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096]
        # Create the labels list
        self._decimate_factor_labels = map(str, self._decimate_factor_options)
        # Create the combo box
        self._decimate_factor_tool_bar = Qt.QToolBar(self)
        self._decimate_factor_tool_bar.addWidget(Qt.QLabel("Decimate_Factor" + ": "))
        self._decimate_factor_combo_box = Qt.QComboBox()
        self._decimate_factor_tool_bar.addWidget(self._decimate_factor_combo_box)
        for _label in self._decimate_factor_labels: self._decimate_factor_combo_box.addItem(_label)
        self._decimate_factor_callback = lambda i: Qt.QMetaObject.invokeMethod(self._decimate_factor_combo_box, "setCurrentIndex", Qt.Q_ARG("int", self._decimate_factor_options.index(i)))
        self._decimate_factor_callback(self.decimate_factor)
        self._decimate_factor_combo_box.currentIndexChanged.connect(
            lambda i: self.set_decimate_factor(self._decimate_factor_options[i]))
        # Create the radio buttons
        self.top_grid_layout.addWidget(self._decimate_factor_tool_bar, 2, 0, 1, 1)
        for r in range(2, 3):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self._center_freq_range = Range(9000, 6300000000, 100, 1000000000, 200)
        self._center_freq_win = RangeWidget(self._center_freq_range, self.set_center_freq, "Center_Frequency", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_layout.addWidget(self._center_freq_win)
        self.qtgui_sink_x_0 = qtgui.sink_c(
            1024, #fftsize
            window.WIN_BLACKMAN_hARRIS, #wintype
            center_freq, #fc
            sample_rate/decimate_factor, #bw
            "", #name
            True, #plotfreq
            True, #plotwaterfall
            True, #plottime
            True, #plotconst
            None # parent
        )
        self.qtgui_sink_x_0.set_update_time(1.0/1000)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.qwidget(), Qt.QWidget)

        self.qtgui_sink_x_0.enable_rf_freq(True)

        self.top_layout.addWidget(self._qtgui_sink_x_0_win)
        self.htra_source_0 = htra_source.htra_source(center_freq, sample_rate, decimate_factor, reference_level)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_gr_complex*1, 32768,True)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_throttle_0, 0), (self.qtgui_sink_x_0, 0))
        self.connect((self.htra_source_0, 0), (self.blocks_throttle_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "IQ_streaming")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_sample_rate(self):
        return self.sample_rate

    def set_sample_rate(self, sample_rate):
        self.sample_rate = sample_rate
        self.htra_source_0.set_sample_rate(self.sample_rate)
        self.qtgui_sink_x_0.set_frequency_range(self.center_freq, self.sample_rate/self.decimate_factor)

    def get_reference_level(self):
        return self.reference_level

    def set_reference_level(self, reference_level):
        self.reference_level = reference_level
        self.htra_source_0.set_ref_level(self.reference_level)

    def get_decimate_factor(self):
        return self.decimate_factor

    def set_decimate_factor(self, decimate_factor):
        self.decimate_factor = decimate_factor
        self._decimate_factor_callback(self.decimate_factor)
        self.htra_source_0.set_decim_factor(self.decimate_factor)
        self.qtgui_sink_x_0.set_frequency_range(self.center_freq, self.sample_rate/self.decimate_factor)

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        self.htra_source_0.set_center_freq(self.center_freq)
        self.qtgui_sink_x_0.set_frequency_range(self.center_freq, self.sample_rate/self.decimate_factor)




def main(top_block_cls=IQ_streaming, options=None):

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
