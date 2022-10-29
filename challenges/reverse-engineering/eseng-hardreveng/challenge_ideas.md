# main idea:
Deep Space Networking thing, reverse the binary running on the server to determine the commands, and then move satellites until they rebuild the network or something

## the challenge:
as this binary acts like the server, receiving requests from the user, the
attacker must reverse engineer the server binary to determine what input the
server binary expects. to aid in their goal, a "user manual" with unhelpful
client-side commands will be provided, however this user manual will explain
that data transmission will only occur when there is a link between each
satellite.

note: the binary will have an alarm of some reasonable amount. this is to
      enforce the fact that this needs to be programmatically solved. a story
      reason for this could be that if too much time is taken, the satellites
      have shifted by some amount.

given commands will include:
a.  a 3d representation (3 2d panes to represent (x,y), (y,z) and (x,z)
    coordinates) of satellite spacial positions
b.  a command to receive the current angle orientation of satellite dishes (using
    x,y,z components)
c.  commands to adjust the angle of a given satellite (again, using x,y,z
    components)
  - likely using individual components to emulate constant signals from a user's
    joystick (maybe not :/ idrk)
d.  a command to "send to the command server" requesting a shell.
  - this will pop a limited shell for the attacker to read the flag.

yes, the plan is to combine reverse engineering with some fun trig. it's not
vector calc.

to make this spicy, create the user manual as a pdf, but have it slightly
corrupted. they need to make reference to this manual, but not all the
information is provided.

### if there's time
- adjust the challenge to not move the dishes, but move the satellites themselves
  until the dishes line up. for extra pain, define the force vector the satellite
  will need to propel by. this would be hell to attack, but also hell to program.

- enforce a rule that requires satellites to effectively discover the location
  of other satellites. (ie. control center will need to search for a satellite,
  connect to said satellite and request that satellite to search for others
  manually). then evenually the entire network could be established
  - add a function that will search the area around the currently focused item