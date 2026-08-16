#!/usr/bin/env python3
"""
send_profile_serial.py

Streams a flight profile CSV to the ESP32 over USB serial and prints ESP32
responses while the replay is running.

Expected input CSV:
    time_s,altitude_m

Protocol sent to ESP32:
    PROFILE_START
    R,<time_ms>,<altitude_m>
    ...
    PROFILE_END

This version waits for the ESP32 to finish setup() before sending PROFILE_START.
The ESP32 must print:

    [REPLAY] READY

before the script starts sending samples.

Example:
    python tools/send_profile_serial.py test_profiles/low_apogee.csv --port COM5 --speed 1
"""

import argparse
import csv
import queue
import threading
import time

import serial


READY_MARKERS = [
    "[REPLAY] READY",
]


def load_profile(path):
    samples = []

    with open(path, newline="") as f:
        reader = csv.DictReader(f)

        for row in reader:
            t = float(row["time_s"])
            a = float(row["altitude_m"])
            samples.append((t, a))

    return samples


def reader_thread(ser, stop_flag, line_queue):
    """
    Continuously read ESP32 serial output and print it.
    Runs in a background thread while the main thread sends samples.
    """
    while not stop_flag["stop"]:
        try:
            raw = ser.readline()

            if raw:
                text = raw.decode("utf-8", errors="replace").rstrip()

                if text:
                    print(text)
                    line_queue.put(text)

        except serial.SerialException:
            break


def wait_for_ready(line_queue, timeout_s):
    """
    Wait until the ESP32 prints a known ready marker.
    """
    deadline = time.time() + timeout_s

    while time.time() < deadline:
        try:
            line = line_queue.get(timeout=0.1)
        except queue.Empty:
            continue

        for marker in READY_MARKERS:
            if marker in line:
                return True

    return False


def main():
    parser = argparse.ArgumentParser(
        description="Send a flight profile to the ESP32 over serial."
    )

    parser.add_argument(
        "csv",
        help="Profile CSV in internal format: time_s,altitude_m"
    )

    parser.add_argument(
        "--port",
        required=True,
        help="Serial port, e.g. COM5 or /dev/ttyUSB0"
    )

    parser.add_argument(
        "--baud",
        type=int,
        default=115200,
        help="Serial baud rate"
    )

    parser.add_argument(
        "--speed",
        type=float,
        default=1.0,
        help="Replay speed multiplier. 1.0 = real time, 5.0 = five times faster"
    )

    parser.add_argument(
        "--ready-timeout",
        type=float,
        default=45.0,
        help="Seconds to wait for ESP32 ready message before sending PROFILE_START"
    )

    parser.add_argument(
        "--post-wait",
        type=float,
        default=5.0,
        help="Seconds to keep reading after PROFILE_END"
    )

    parser.add_argument(
        "--no-wait-ready",
        action="store_true",
        help="Send immediately without waiting for ESP32 ready message"
    )

    args = parser.parse_args()

    samples = load_profile(args.csv)

    if not samples:
        raise RuntimeError("No samples found in profile.")

    print(f"Loaded {len(samples)} samples from {args.csv}")
    print(f"Opening {args.port} @ {args.baud} baud")
    print(f"Replay speed: {args.speed}x")
    print()

    stop_flag = {
        "stop": False
    }

    line_queue = queue.Queue()

    with serial.Serial(args.port, args.baud, timeout=0.1) as ser:
        # Opening the serial port may reset the ESP32.
        # Start reading immediately, then wait until the firmware says it is ready.
        reader = threading.Thread(
            target=reader_thread,
            args=(ser, stop_flag, line_queue),
            daemon=True
        )

        reader.start()

        if not args.no_wait_ready:
            print("[PC] Waiting for ESP32 ready message...")

            ready = wait_for_ready(
                line_queue,
                args.ready_timeout
            )

            if not ready:
                print("[PC] WARNING: ESP32 ready message not seen before timeout.")
                print("[PC] Sending anyway. If replay does not start, check:")
                print("[PC]  - SERIAL_PROFILE_REPLAY_MODE is set to 1")
                print("[PC]  - firmware was uploaded after changing Config.h")
                print("[PC]  - main.cpp prints [REPLAY] READY at the end of setup()")
            else:
                print("[PC] ESP32 ready. Starting replay.")

        print("[PC] Sending PROFILE_START")

        ser.write(b"PROFILE_START\n")
        ser.flush()

        previous_t = samples[0][0]

        for t, altitude in samples:
            dt = max(
                0.0,
                t - previous_t
            )

            if args.speed > 0:
                time.sleep(
                    dt / args.speed
                )

            time_ms = int(
                round(t * 1000.0)
            )

            line = f"R,{time_ms},{altitude:.2f}\n"

            ser.write(
                line.encode("ascii")
            )

            ser.flush()

            previous_t = t

        print("[PC] Sending PROFILE_END")

        ser.write(b"PROFILE_END\n")
        ser.flush()

        time.sleep(args.post_wait)

        stop_flag["stop"] = True

        time.sleep(0.2)

    print()
    print("[PC] Done.")


if __name__ == "__main__":
    main()