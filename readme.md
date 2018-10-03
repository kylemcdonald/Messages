# Messages

In late 2011, Finnish demoscene artist Viznut released a collection of simple musical programs in the C programming language. Each program looked like `main(t){for(t=0;;t++)putchar(t*(((t>>12)|(t>>8))&(63&(t>>4))));}` and generated glitchy sonic environments. After realizing the potential for these simple bit-shifting programs to produce complex melodies, harmonies, and rhythms, many other programmers and artists created their own variations on this basic code. "Messages" represents ongoing experiments with the automatic generation of these simple programs that simultaneously produce sound and visuals. Every image on the screen is made from a short "sentence" of code that is executed on the computer's graphics card, then read back to the sound card and interpreted directly as sound. Programs are ranked by their salience using compression algorithms, and the more interesting images are given more time on screen. The overall effect is something like divining a signal in noise, a series of messages buried in chaos.

## History

- This code was written in December 2011 for a video [posted to Vimeo](https://vimeo.com/34184475).
- Used while teaching glitch for "[Appropriating New Technologies](https://github.com/kylemcdonald/AppropriatingNewTechnologies/tree/master/week4/BytebeatExample/src)" at ITP in 2012.
- Updated for display and shown at the 2nd Tehran Annual Digital Art Exhibition (TADAEX) in June 2012.
- Updated in October 2018 for Future Tellers in Istanbul.