import numpy as np
from gnuradio import gr, digital

class OptimizedDDPLL(gr.sync_block):
    def __init__(self, Loop_BandWidth=0.01, Damping_Factor=1, Constellation_Object=digital.bpsk_constellation()):
        gr.sync_block.__init__(
            self,
            name='DDPLL',
            in_sig=[np.complex64],
            out_sig=[np.complex64]
        )
        
        self.Loop_BandWidth = Loop_BandWidth
        self.Damping_Factor = Damping_Factor
        self.Constellation_Object = Constellation_Object

 
        if hasattr(Constellation_Object, 'points'):
            points = Constellation_Object.points()
        else:
            points = Constellation_Object
            

        self.real_levels = np.unique(np.real(points))
        self.imag_levels = np.unique(np.imag(points))
        

        self.phase = 0.0
        self.freq = 0.0
        self.update_loop_coefficients()
        

        self.max_freq_step = 0.05

    def update_loop_coefficients(self):
        bw = self.Loop_BandWidth
        damp = self.Damping_Factor
        K = 2.0
        denom = 1.0 + 2.0 * damp * bw + bw * bw
        self.alpha = (4.0 * damp * bw * K) / denom
        self.beta = (4.0 * bw * bw * K) / denom

    def fast_decision(self, symbol):
        real_decision = self.real_levels[np.argmin(np.abs(self.real_levels - np.real(symbol)))]
        imag_decision = self.imag_levels[np.argmin(np.abs(self.imag_levels - np.imag(symbol)))]
        return real_decision + 1j * imag_decision

    def work(self, input_items, output_items):
        inputs = input_items[0]
        outputs = output_items[0]
        
        for i in range(len(inputs)):
      
            rotated = inputs[i] * np.exp(-1j * self.phase)

            decision = self.fast_decision(rotated)

            if abs(decision) > 1e-10:
                error = np.angle(rotated * np.conj(decision))
                error = max(min(error, np.pi/4), -np.pi/4)
            else:
                error = 0.0
            

            freq_update = self.beta * error
            if abs(freq_update) > self.max_freq_step:
                freq_update = np.sign(freq_update) * self.max_freq_step
            
            self.freq += freq_update
            self.phase += self.freq + self.alpha * error
            
            if self.phase > np.pi:
                self.phase -= 2 * np.pi
            elif self.phase < -np.pi:
                self.phase += 2 * np.pi
            
            outputs[i] = rotated
        
        return len(outputs)
