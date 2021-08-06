import sys
import numpy as np
import scipy.signal as sp
import matplotlib.pyplot as plt

cols  = int(sys.argv[1])
rows  = int(sys.argv[2])
frows = int(sys.argv[3])
path  = sys.argv[4]

signal_raw = np.fromfile(path, dtype='float32')
signal = np.reshape(signal_raw, (rows, len(signal_raw) // rows))

cwt_raw = np.fromfile("/home/dirog/Projects/cwtstd_test/source/test/plot/output.bin", dtype='float32')
cwt = np.reshape(cwt_raw, (rows, frows, cols))

plt.rcParams["font.family"] = "Courier New"
for i in range(rows):
	fig, axs = plt.subplots(2, 1, figsize=(13, 9), gridspec_kw={'height_ratios': [1, 3]})
	axs[0].plot(signal[i,:], 'black')
	axs[0].set_title("Сигнал")
	axs[0].set_ylabel("s[n]")

	axs[1].imshow(cwt[i,:,:], aspect='auto')
	axs[1].set_title("НВП")
	axs[1].set_xlabel("b")
	axs[1].set_ylabel("a")
	

fig.savefig('/home/dirog/Desktop/cwt.eps', format='eps')
plt.show()