import pygame


pygame.init()

# Loop until the user clicks the close button.
done = False

old_joystick_count = -1
old_jid = -1
old_name = -1
old_guid = -1
old_axes = -1
# Used to manage how fast the screen updates.
clock = pygame.time.Clock()

# Initialize the joysticks.
pygame.joystick.init()

# -------- Main Program Loop -----------
while not done:

    joystick_count = pygame.joystick.get_count()
    
    if old_joystick_count != joystick_count:
        old_joystick_count = joystick_count
        print("Number of joysticks: {}".format(joystick_count))

    # For each joystick:
    for i in range(joystick_count):
        joystick = pygame.joystick.Joystick(i)
        joystick.init()

        try:
            jid = joystick.get_instance_id()
        except AttributeError:
            # get_instance_id() is an SDL2 method
            jid = joystick.get_id()
        
        if old_jid != jid:
            old_jid = jid
            print("Joystick {}".format(jid))


        # Get the name from the OS for the controller/joystick.
        name = joystick.get_name()
        if old_name != name:
            old_name = name
            print("Joystick name: {}".format(name))
            
        try:
            guid = joystick.get_guid()
        except AttributeError:
            # get_guid() is an SDL2 method
            pass
        else:
            if old_guid != guid:
                old_guid = guid
                print("GUID: {}".format(guid))

        # Usually axis run in pairs, up/down for one, and left/right for
        # the other.
        axes = joystick.get_numaxes()
        if old_axes != axes:
            old_axes = axes
            print("Number of axes: {}".format(axes))

        # for i in range(axes):
        #     axis = joystick.get_axis(i)
        #     textPrint.tprint(screen, "Axis {} value: {:>6.3f}".format(i, axis))


        # buttons = joystick.get_numbuttons()
        # textPrint.tprint(screen, "Number of buttons: {}".format(buttons))

        # for i in range(buttons):
        #     button = joystick.get_button(i)
        #     textPrint.tprint(screen, "Button {:>2} value: {}".format(i, button))


        # hats = joystick.get_numhats()
        # textPrint.tprint(screen, "Number of hats: {}".format(hats))
 

        # # Hat position. All or nothing for direction, not a float like
        # # get_axis(). Position is a tuple of int values (x, y).
        # for i in range(hats):
        #     hat = joystick.get_hat(i)
        #     textPrint.tprint(screen, "Hat {} value: {}".format(i, str(hat)))

    # Limit to 20 frames per second.
    clock.tick(20)

# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit()
