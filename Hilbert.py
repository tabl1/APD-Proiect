import numpy as np

from scipy.signal import hilbert

with open("semnal.txt", "r") as file:
    signal_data = file.read()

signal = np.array([float(value) for value in signal_data.split()])

analytic_signal = hilbert(signal) #signal este vectorul aferent semnalului analizat
amplitude_envelope = np.abs(analytic_signal) #amplitude_envelope este anvelopa semnalului

with open("anvelopa.txt", "w") as file:
    np.savetxt(file, amplitude_envelope)
