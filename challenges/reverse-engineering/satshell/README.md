# Satshell
### Category: Reverse Engineering
### Author: Petiole (e-seng)

## Description
During one of the planned scavenges within the desolate Martian Omniflags
headquarters, the team that was sent out located one of Omniflags' communication
server rooms. Although greatly damaged from the *The War of the Martians* in
2189, a team of technicians were able to restart their server and hijack its
network to be under our control. We understand that this server room
once transmitted crucial data to the interplanetary operations of Omniflags and
wish to retrieve it.

Upon further investigation, no vital information could be found within the
server itself. However, as the scouting team were highly meticulous, they
managed to find and scan pages of the attached internal user manual. Although we
do not have the client program that the user manual discusses, the data recovery
team was able to recover a binary program which seems to be associated with it.

We trust that you will be able to determine methods to use what we have given
you to determine a way to read the secrets that Omniflags has been holding onto
all this time. Please connect to `<HOST>:<PORT>`, the address where the program
is hosted when you are ready.

## Hints
1. Machine code should be sent in *big endian*.
2. Float toy is a great way to see IEEE 754-standardized floats
3. Define a trigonometric equation to determine `dx`, `dy`, `dz`

## Solution
The solution to this challenge is a little vague to write admittedly, but I will
try to be as verbose as I can.

1. Understand the process to make a connection
  - Specified within the user manual, the provided binary is used to send
    machine code to satellites in order to create a network out of them. Each
    satellite must be connected in a lexographically increasing order. That is,
    satellite A connects to satellite B, B to C and C to D.
  - Once the satellites have been connected, data can be transmitted between the
    "Central Communication Server" and the "Extra-Terrestial Dataserver", which
    is where the secrets we are looking for are hosted.

2. Obtain command keywords to perform certain actions
  - `POSN` is provided within the user manual, with a brief description
  - `STAT`, `ORNT` and `CONN` can be determined by decompiling the provided
    binary.
    - note: these four commands each call a function,
      `get_satellite_positions(...)`, `get_satellite_orientation(...)`,
      `reorient_satellite(...)` and `sig_spawn_shell(...)` respectively.
    - these functions can be individually reversed, however, some functions,
      `sig_send_msg(...)`, `sig_lstn_msg(...)` and `sig_spawn_shell(...)` are
      not defined within this binary, but individually gives an *idea* of what
      goes on within those functions.
  - For some satellite, R, its positional coordinates can be given through the
    `POSN` command. The bottom-left coordinate is indexed at (0, 0) for each 2D
    array returned by the server. Its angular orientation can be given through
    the `STAT` command.  This is relative to the cartesian coordinates from POSN
    and are in degrees.

3. Understand the satellite processor and create associated machine code.
  - By viewing the "Reorientation" block diagram on page 9 of the
    `recovered-user-man` file, the following conclusions can be made:
    - Instructions are 16 bits in length
    - The two most significant bits, bits 15 and 14, determine the value of
      `ALU_Ctrl`. The associated *operation*, or what the ALU will do given that
      value is *mostly* given below the diagram. This is similar to *opcodes*
    - bits 13 to 11 of the instruction determine the address of the source
      register, each provided on the page prior
    - bits 10 to 8 of the instruction determine the address of the destination
      register, similar to the last 3 bits
    - bits 7 to 0 of the instruction define an immediate value, treated as an
      unsigned integer.
  - Using these parts, 16 bit values within each reg can be set despite only
    passing 8 bit immediate values.
    - This can be achieved through a combination of `ori` or `addi` to set the
      least significant bits (0-7), and the `lui` or `sll` instructions to set
      the most significant bits (8-15).
    - eg. using only `ori` and `lui` to set `$dx = 0xabcd`
      ```as
      lui   $dx 0xab      ; 0b 11 XXX 101 1010 1011 -> XXX are not-cares as no register is read
      ori   $dx $dx 0xcd  ; 0b 01 101 101 1100 1101 -> $dx = $dx | 0x cd, where $dx is 0xab00
      ```
    - eg. using only `sll` and `addi` to set `$dy = 0x9876`
      ```as
      addi  $dy $zp 0x98  ; 0b 00 000 110 1001 1000 -> $dy = $zp + 0x98, where $zp = 0
      sll   $dy $dy 8     ; 0b 10 110 110 0000 1000 -> $dy = $dy << 8
      addi  $dy $zp 0x76  ; 0b 00 110 110 0111 0110 -> $dy = $dy + 0x98, where $dy = 0x9800
      ```

4. Perform the necessary trigonometry to determine the needed change in angle
   required to point one satellite to the next.
  - this is achievable by determining the desired net angle to point one
    satellite `R` so its following satellite `S`. To do this, vector `R->S` can
    be determined by subtracting the coordinates of point `S` by point `R`.
  - Using this, the desired angular component can be given from the angle
    between vector `R->S` and the associated axis (eg. X-component would be the
    X-axis)
  - As axes X, Y and Z are perpendicular to one another, a right angle can be
    determined by dropping the altitute of the vector to the desired axis.
    - For example, if the angular X-component is desired, then a line from the
      tip of vector `R->S` perpendicularly to the X-axis defines a right
      triangle between the vector, its altitute and the X-axis.
    - the *lengths* of those lines can now be determined through Pythagoras's
      Theorem.
      ```c
      |R->S| = sqrt( (S.x - R.x) ^ 2 + (S.y - R.y) ^ 2 + (S.z - R.z) ^ 2 )
      |altitude| = sqrt( (S.y - R.y) ^ 2 + (S.z - R.z) ^ 2 )
      |along x-axis| = (S.x - R.x)

      // where R.i and S.i are the i-th positional component of R and S respectively
      ```
  - Since a right triangle has been defined with known lengths, the desired
    angle can also be determined
    ```c
    // the altitude and the length along the x-axis respectively form the
    // opposite and adjacent lines to the angle. arctan can then be used to
    // determine the angle

                        sqrt( (S.y - R.y) ^ 2 + (S.z - R.z) ^ 2 )
    desired_x = arctan(-------------------------------------------);
                                       (S.x - R.x)
    ```
  - As the *change in angle* is desired to be set, the obtained value can be
    subtracted by the current angle.
    ```c
    delta_x = desired_x - R.theta_x; // where R.theta_x is the angular X-component of R
    ```
  - lastly, the expected value is to be in *degrees*. Languages like python3 or
    C calculate angles in radians and need to be converted.
    ```c
    delta_x = delta_x * (180 / MATH.pi);
    ```

5. Convert the identified angle into an IEEE 754-standard half-precision
   floating point value.
  - see websites like [Float Toy](http://evanw.github.io/float-toy/) which
    demonstrate this much better than I can.
  - This is achievable using `numpy` in python3.
    ```py
    dx_hex = hex(int.from_bytes(np.array([delta_x], '>f2').tobytes(), "big"))
    ```
  - note that values *are parsed to big endian*

6. Create and send the necessary machine code for each component
  - This uses the understanding of setting register values in step 3
  - As the floating point value is 16 bits in length, both the least and most
    significant bits need to be set to work
  - note: multiple angular components can be changed in the same communication
    session, but each angular component can be changed wihtin their own session
    if desired.
  - This needs to be done for satellites `A`, `B` and `C`.

7. Once all satellites have been correctly oriented, send `CONN` to spawn a
   shell.
  - Assuming everything went correctly, sending the command `CONN` will spawn an
    interactive shell containing the flag.

For additional reference, feel free to [contact me](https://e-seng.github.io) or
view the solve script of the challenge.

Note: within the solve script, I defined a messy assembler to assemble the
machine code within step 2.

## Flag
`magpie{1'v3-c0nn3c+3d-t43-d0ts}`
