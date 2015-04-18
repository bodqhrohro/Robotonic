# Robotonic
pipeline music generator

Installation
====
Clone this and just run `make`.

How it works
====
Robotonic takes 4 bytes from stdin on each iteration and generates sound pattern using them as parameters: frequency, wave type (triangle, square, sin or saw), LFO wave type (no LFO, normal fade-in, parabolic fade-in or parabolic fade-out), interval and offset from the beginning of the pattern. Each new pattern overlaps the previous which makes illusion of continuity.

Usage
====
`cat $file|./robotonic|$player`

, where `$file` is, f. e., `/dev/urandom` or anything else you want to playback, `$player` is any program able to read raw sound data from the stdin and direct them to speakers or maybe even to sound file (f. e., `aplay`).

Configuration
====
Change constants near the top of source file and then recompile the program

* BUF_SIZE — number of samples generated for each pattern
* SAMP_LENGTH — length of samples (shorter is glitchier because this does not affect the frequency)
* RATE — output sampling rate
* NOISE_LEVEL — from 0 to 255, sets the volume of the noise mixed up with the generated sound
