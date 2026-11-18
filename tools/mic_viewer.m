function frames = mic_viewer(port, baud, nFrames)
%MIC_VIEWER Live viewer for the Curiosity Trainer microphone frames.
%   Reads the frames sent by mic_send_frame() (example 17, mic.c) over the
%   debugger's virtual COM port and plots the waveform and its spectrum.
%
%   mic_viewer                     first serial port found, 115200 baud
%   mic_viewer("COM5")             Windows
%   mic_viewer("/dev/ttyACM0")     Linux
%   mic_viewer(port, baud)         other baud rate (MIC_UART_BAUD in mic.h)
%   frames = mic_viewer(port, baud, N)  read N frames without plotting,
%                                  frames{k}.samples holds the raw ADC values
%
%   Close the figure window to stop.
%
%   Frame format (little endian):
%     0xA5 0x5A | seq (uint16) | len (uint16) | sample rate Hz (uint16)
%     | len samples (uint16, 0..4095) | checksum (uint16, sum of the samples)

arguments
    port string = ""
    baud double = 115200
    nFrames double = Inf
end

VREF = 3.3;
ADC_MAX = 4095;

if port == ""
    ports = serialportlist("available");
    if isempty(ports)
        error("No serial port found, pass the port name: mic_viewer(""COM5"")");
    end
    port = ports(end);
end

sp = serialport(port, baud);
cleanup = onCleanup(@() delete(sp));
flush(sp);
fprintf("Reading %s at %d baud. Set the selector switch to the microphone and run example 17 (""Microphone"").\n", port, baud);

plotting = isinf(nFrames);
if plotting
    fig = figure("Name", "Curiosity Trainer - Microphone", "NumberTitle", "off");
    axT = subplot(2, 1, 1, "Parent", fig);
    hT = plot(axT, NaN, NaN);
    xlabel(axT, "Time (ms)"); ylabel(axT, "Voltage (V)"); ylim(axT, [0 VREF]); grid(axT, "on");
    axF = subplot(2, 1, 2, "Parent", fig);
    hF = plot(axF, NaN, NaN);
    xlabel(axF, "Frequency (Hz)"); ylabel(axF, "Magnitude (dBV)"); ylim(axF, [-100 0]); grid(axF, "on");
end

buf = uint8([]);
count = 0;
frames = {};
while count < nFrames && (~plotting || isvalid(fig))
    [frame, buf] = readFrame(sp, buf);
    if plotting && ~isvalid(fig)
        break % the window was closed while waiting for data
    end
    if isempty(frame) % no frame within 0.5 s, keep waiting
        if plotting && isvalid(fig)
            title(axT, "Waiting for frames... (is example 17 running?)");
            drawnow
        end
        continue
    end
    count = count + 1;
    v = double(frame.samples) * VREF / ADC_MAX;

    if ~plotting
        frames{end + 1} = frame; %#ok<AGROW>
        fprintf("seq %5d  n %d  fs %d Hz  mean %.3f V  p-p %.3f V\n", ...
            frame.seq, numel(v), frame.fs, mean(v), max(v) - min(v));
        continue
    end

    t = (0:numel(v) - 1) * 1000 / frame.fs;
    [f, db] = spectrumDb(v, frame.fs);
    set(hT, "XData", t, "YData", v);
    set(hF, "XData", f, "YData", db);
    xlim(axT, [0 t(end)]);
    xlim(axF, [0 frame.fs / 2]);
    title(axT, sprintf("Frame %d  |  %d samples at %d Hz  |  mean %.2f V, p-p %.3f V", ...
        frame.seq, numel(v), frame.fs, mean(v), max(v) - min(v)));
    drawnow
end

if ~plotting
    fprintf("%d frames read\n", numel(frames));
end
end

function [frame, buf] = readFrame(sp, buf)
% Returns the next valid frame, or [] on timeout. buf keeps the bytes not used yet.
frame = [];
while true
    if numel(buf) < 8
        [buf, ok] = fill(sp, buf, 8);
        if ~ok, return, end
    end
    i = find(buf(1:end - 1) == 0xA5 & buf(2:end) == 0x5A, 1);
    if isempty(i)
        buf = buf(end); % the last byte may be the first sync byte
        continue
    end
    buf = buf(i:end);
    [buf, ok] = fill(sp, buf, 8);
    if ~ok, return, end
    hdr = typecast(buf(3:8), "uint16"); % seq, len, fs (little endian host)
    n = double(hdr(2));
    if n == 0 || n > 8192 || hdr(3) == 0
        buf = buf(2:end); % false sync
        continue
    end
    total = 8 + 2 * n + 2;
    [buf, ok] = fill(sp, buf, total);
    if ~ok, return, end
    samples = typecast(buf(9:8 + 2 * n), "uint16");
    checksum = typecast(buf(9 + 2 * n:10 + 2 * n), "uint16");
    if mod(sum(double(samples)), 65536) ~= double(checksum) || max(samples) > 4095
        buf = buf(2:end); % corrupted frame, search the next sync
        continue
    end
    frame = struct("seq", double(hdr(1)), "fs", double(hdr(3)), "samples", samples(:)');
    buf = buf(total + 1:end);
    return
end
end

function [buf, ok] = fill(sp, buf, n)
% Read from the port until buf holds at least n bytes. Only the bytes already
% received are read, so it never blocks: ok = false after 0.5 s without data.
ok = true;
lastData = tic;
while numel(buf) < n
    k = sp.NumBytesAvailable;
    if k == 0
        if toc(lastData) > 0.5
            ok = false;
            return
        end
        pause(0.005);
        continue
    end
    buf = [buf, uint8(read(sp, k, "uint8"))]; %#ok<AGROW>
    lastData = tic;
end
end

function [f, db] = spectrumDb(v, fs)
x = v(:) - mean(v);
n = numel(x);
w = 0.5 - 0.5 * cos(2 * pi * (0:n - 1)' / (n - 1)); % Hann window
X = fft(x .* w);
mag = abs(X(1:floor(n / 2) + 1)) / (sum(w) / 2);
f = (0:floor(n / 2)) * fs / n;
db = 20 * log10(mag + 1e-9);
end
