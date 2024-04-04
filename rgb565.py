def dstorgb(rgb565):
    # Shift the red value to the right by 11 bits.
    red5 = rgb565 >> 11
    # Shift the green value to the right by 5 bits and extract the lower 6 bits.
    green6 = (rgb565 >> 5) & 0b111111
    # Extract the lower 5 bits.
    blue5 = rgb565 & 0b11111

    # Convert 5-bit red to 8-bit red.
    red8 = round(red5 / 31 * 255)
    # Convert 6-bit green to 8-bit green.
    green8 = round(green6 / 63 * 255)
    # Convert 5-bit blue to 8-bit blue.
    blue8 = round(blue5 / 31 * 255)

    print(red8, green8, blue8)
    
def rgbtods(red8, green8, blue8):
    # Convert 8-bit red to 5-bit red.
    red5 = round(red8 / 255 * 31)
    # Convert 8-bit green to 6-bit green.
    green6 = round(green8 / 255 * 63)
    # Convert 8-bit blue to 5-bit blue.
    blue5 = round(blue8 / 255 * 31)

    # Shift the red value to the left by 11 bits.
    red5_shifted = red5 << 11
    # Shift the green value to the left by 5 bits.
    green6_shifted = green6 << 5

    # Combine the red, green, and blue values.
    rgb565 = red5_shifted | green6_shifted | blue5

    print(rgb565)

print("Okay, you can now run dstorgb(rgb565) and rgbtods(red,blue,green)")
import code; code.interact(local=locals())
