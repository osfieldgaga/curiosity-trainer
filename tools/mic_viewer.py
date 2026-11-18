#!/usr/bin/env python3
"""
Live viewer for the Curiosity Trainer microphone frames (example 17, mic.c).

Reads the frames sent by mic_send_frame() over the debugger's virtual COM port
and plots the waveform and its spectrum.

Frame format (little endian):
    0xA5 0x5A | seq (u16) | len (u16) | sample rate Hz (u16)
    | len samples (u16, 0..4095) | checksum (u16, sum of the samples)

Requirements:  pip install pyserial numpy matplotlib

Usage:
    python mic_viewer.py                       # first serial port found
    python mic_viewer.py --port COM5           # Windows
    python mic_viewer.py --port /dev/ttyACM0   # Linux
    python mic_viewer.py --check 20            # no plot, print stats of 20 frames
    python mic_viewer.py --record frames.csv   # also save every frame to a CSV file
"""

import argparse
import struct
import sys
import time

import numpy as np
import serial
import serial.tools.list_ports

SYNC = b"\xA5\x5A"
HEADER = struct.Struct("<HHH")  # seq, len, sample rate
VREF = 3.3
ADC_MAX = 4095


class FrameReader:
    """Finds and validates frames in the serial byte stream."""

    def __init__(self, port, baud):
        self.ser = serial.Serial(port, baud, timeout=0.5)
        self.ser.reset_input_buffer()
        self.buf = bytearray()
        self.bad = 0

    def _fill(self, n):
        while len(self.buf) < n:
            data = self.ser.read(max(n - len(self.buf), self.ser.in_waiting or 1))
            if not data:
                return False
            self.buf += data
        return True

    def read_frame(self):
        """Returns (seq, sample_rate, samples) or None on timeout."""
        while True:
            if not self._fill(2 + HEADER.size):
                return None
            i = self.buf.find(SYNC)
            if i < 0:
                del self.buf[:-1]  # keep the last byte, it may be the first sync byte
                continue
            del self.buf[:i]
            if not self._fill(2 + HEADER.size):
                return None
            seq, n, fs = HEADER.unpack_from(self.buf, 2)
            if n == 0 or n > 8192 or fs == 0:
                del self.buf[:1]  # false sync, keep searching
                continue
            total = 2 + HEADER.size + 2 * n + 2
            if not self._fill(total):
                return None
            samples = np.frombuffer(bytes(self.buf[8:8 + 2 * n]), dtype="<u2").astype(np.int32)
            (checksum,) = struct.unpack_from("<H", self.buf, 8 + 2 * n)
            if (int(samples.sum()) & 0xFFFF) != checksum or samples.max() > ADC_MAX:
                self.bad += 1
                del self.buf[:1]
                continue
            del self.buf[:total]
            return seq, fs, samples


def default_port():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        if "Curiosity" in (p.description or "") or "nEDBG" in (p.description or "") or "ACM" in p.device:
            return p.device
    return ports[0].device if ports else None


def spectrum(volts, fs):
    x = volts - volts.mean()
    win = np.hanning(len(x))
    mag = np.abs(np.fft.rfft(x * win)) / (win.sum() / 2)
    freqs = np.fft.rfftfreq(len(x), 1.0 / fs)
    return freqs, 20 * np.log10(mag + 1e-9)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--port", default=None, help="serial port (default: first Curiosity/ACM port)")
    ap.add_argument("--baud", type=int, default=115200, help="baud rate (MIC_UART_BAUD in mic.h, default 115200)")
    ap.add_argument("--check", type=int, metavar="N", help="read N frames, print statistics and exit (no plot)")
    ap.add_argument("--record", metavar="FILE", help="append every frame to a CSV file (seq, fs, samples...)")
    args = ap.parse_args()

    port = args.port or default_port()
    if not port:
        sys.exit("No serial port found, use --port")
    reader = FrameReader(port, args.baud)
    rec = open(args.record, "a") if args.record else None
    print(f"Reading {port} at {args.baud} baud. Set the selector switch to the microphone "
          f"and run example 17 (\"Microphone\") on the board.")

    def handle(frame):
        seq, fs, samples = frame
        if rec:
            rec.write(f"{seq},{fs}," + ",".join(map(str, samples)) + "\n")
        return seq, fs, samples * VREF / ADC_MAX

    if args.check:
        t0, got, last = time.time(), 0, None
        while got < args.check:
            frame = reader.read_frame()
            if frame is None:
                sys.exit(f"Timeout: no frame received ({got} so far). Is example 17 running?")
            seq, fs, v = handle(frame)
            lost = 0 if last is None else (seq - last - 1) & 0xFFFF
            last = seq
            got += 1
            f, db = spectrum(v, fs)
            peak = f[1 + np.argmax(db[1:])]
            print(f"seq {seq:5d}  n {len(v)}  fs {fs} Hz  mean {v.mean():.3f} V  "
                  f"p-p {v.max() - v.min():.3f} V  peak {peak:7.1f} Hz  lost {lost}")
        dt = time.time() - t0
        print(f"{got} frames in {dt:.2f} s ({got / dt:.1f} frames/s), {reader.bad} bad frames")
        return

    import matplotlib.pyplot as plt
    from matplotlib.animation import FuncAnimation

    fig, (ax_t, ax_f) = plt.subplots(2, 1, figsize=(9, 6))
    fig.canvas.manager.set_window_title("Curiosity Trainer - Microphone")
    (line_t,) = ax_t.plot([], [], lw=1)
    (line_f,) = ax_f.plot([], [], lw=1)
    ax_t.set_xlabel("Time (ms)")
    ax_t.set_ylabel("Voltage (V)")
    ax_t.set_ylim(0, VREF)
    ax_t.grid(True)
    ax_f.set_xlabel("Frequency (Hz)")
    ax_f.set_ylabel("Magnitude (dBV)")
    ax_f.set_ylim(-100, 0)
    ax_f.grid(True)
    title = ax_t.set_title("Waiting for frames...")  # text before tight_layout() keeps room for it
    fig.tight_layout()

    def update(_):
        frame = reader.read_frame()
        if frame is None:
            title.set_text("Waiting for frames...")
            return line_t, line_f, title
        seq, fs, v = handle(frame)
        t = np.arange(len(v)) * 1000.0 / fs
        f, db = spectrum(v, fs)
        line_t.set_data(t, v)
        line_f.set_data(f, db)
        ax_t.set_xlim(0, t[-1])
        ax_f.set_xlim(0, fs / 2)
        title.set_text(f"Frame {seq}  |  {len(v)} samples at {fs} Hz  |  "
                       f"mean {v.mean():.2f} V, p-p {v.max() - v.min():.3f} V")
        return line_t, line_f, title

    anim = FuncAnimation(fig, update, interval=1, blit=False, cache_frame_data=False)  # noqa: F841
    plt.show()
    if rec:
        rec.close()


if __name__ == "__main__":
    main()
